#include "atmel_start.h"
#include "FreeRTOS.h"
#include "task.h"

#include <stdio.h>
#include <stdarg.h>

#include "driver/usb/usb_console.h"
#include "driver/log_msg.h"

#define BUF_SIZE CONF_USB_CDCD_ACM_DATA_BULKIN_MAXPKSZ_HS

TaskHandle_t usb_console_task_h;

static uint8_t read_buf[BUF_SIZE] __attribute__ ((aligned (4)));
static uint8_t write_buf[BUF_SIZE] __attribute__ ((aligned (4)));
static int read_index;
static volatile int read_avail;
static volatile int write_stat;

static SemaphoreHandle_t read_sem;
static SemaphoreHandle_t write_sem;

static bool usb_device_cb_bulk_out(const uint8_t ep, const enum usb_xfer_code rc, const uint32_t count) {
	(void)ep;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	if (rc == USB_XFER_DONE) {
		read_avail = count;
	} else {
		read_avail = -1;
		log_warn("cdc_acm: out ep error: %d", rc);
	}

	xSemaphoreGiveFromISR(read_sem, &xHigherPriorityTaskWoken);
	//vTaskNotifyGiveFromISR(usb_console_task_h, &xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

	return false;
}

static bool usb_device_cb_bulk_in(const uint8_t ep, const enum usb_xfer_code rc, const uint32_t count) {
	(void)ep;
	(void)count;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	if (rc != USB_XFER_DONE) {
		log_warn("cdc_acm: in ep error: %d", rc);
	}

	xSemaphoreGiveFromISR(write_sem, &xHigherPriorityTaskWoken);
	//vTaskNotifyGiveFromISR(usb_console_task_h, &xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

	return false;
}

int cwrite(const void *src, int count) {
	while (count) {
		int cnt = count;
		if (cnt > BUF_SIZE) cnt = BUF_SIZE;

		xSemaphoreTake(write_sem, portMAX_DELAY);

		memcpy(write_buf, src, cnt);

		int r = cdcdf_acm_write(write_buf, cnt);
		if (r) {
			log_warn("cdc_acm: cdcdf_acm_write failed: %d", r);
			xSemaphoreGive(write_sem);
			return -1;
		}

		src += cnt;
		count -= cnt;
	}
	return 0;
}

int cprintf(const char *format, ...) {
	static char buf[256];
	va_list args;
	va_start(args, format);
	int r = vsnprintf(buf, sizeof(buf), format, args);
	if (r < 0) {
		log_error("cprintf: vsnprintf error");
		return -1;
	}
	r = min(r, (int)sizeof(buf)-1);
	r = cwrite(buf, r);
	va_end(args);
	return r;
}

/*
int cread_async(uint8_t *dst, int size) {
	if (!read_avail) {
		read_index = 0;
		int r = cdcdf_acm_read(read_buf, BUF_SIZE);
		if (r && r != USB_BUSY) {
			log_warn("cdc_acm: cdcdf_acm_read failed: %d", r);
			return -1;
		} else {
			return 0;
		}
	} else {
		int cnt = min(read_avail, size);
		memcpy(dst, read_buf + read_index, cnt);
		read_index += cnt;
		read_avail -= cnt;
		return cnt;
	}
}

int cread(uint8_t *dst, int size) {
	int r = cread_async(dst, size);
	if (r) return r;

	while (!read_avail)
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

	return cread_async(dst, size);
}
*/

static int read_start(void) {
	if (pdTRUE == xSemaphoreTake(read_sem, 0)) {
		if (read_avail) {
			xSemaphoreGive(read_sem);
		} else {
			read_index = 0;
			read_avail = 0;
			int r = cdcdf_acm_read(read_buf, BUF_SIZE);
			if (r) {
				xSemaphoreGive(read_sem);
				log_warn("cdc_acm: cdcdf_acm_read failed: %d", r);
				return -1;
			}
		}
	}
	return 0;
}

static void read_wait(void) {
	xSemaphoreTake(read_sem, portMAX_DELAY);
	xSemaphoreGive(read_sem);
}

int cgetc_async(void) {
	if (read_avail > 0) {
		read_avail--;
		return read_buf[read_index++];
	} else if (read_avail < 0) {
		return -1;
	} else {
		return read_start();
	}
}

int cgetc(void) {
	if (!read_avail) {
		int r = read_start();
		if (r < 0) return r;
		read_wait();
		if (read_avail <= 0)
			return -1;
	}

	read_avail--;
	return read_buf[read_index++];
}

int creadline(char *s, int size) {
	int cursor = 0;
	while (1) {
		int c = cgetc();
		if (c < 0) return c;

		if (c == 0x7f) { // del
			if (cursor > 0)	{
				cursor--;
				cwrite("\b \b", 3);
			}
		} else if (c == '\n' || c == '\r') {
			s[cursor] = '\0';
			cwrite(&c, 1);
			return cursor;
		} else if (cursor < size-1) {
			s[cursor] = c;
			cursor++;
			cwrite(&c, 1);
		}
	};
}

static void usb_console_task(void *p) {
	(void)p;

	while (1) {
		while (!cdcdf_acm_is_enabled()) {
			vTaskDelay(100);
		}
		log_info("cdc_acm: enabled"); 

		read_index = 0;
		read_avail = 0;
		write_stat = 0;

		cdcdf_acm_register_callback(CDCDF_ACM_CB_READ, (FUNC_PTR)usb_device_cb_bulk_out);
		cdcdf_acm_register_callback(CDCDF_ACM_CB_WRITE, (FUNC_PTR)usb_device_cb_bulk_in);

		cmdline_main();
	}
}

void usb_console_init(void) {
	read_sem = xSemaphoreCreateBinary();
	write_sem = xSemaphoreCreateBinary();
	xSemaphoreGive(read_sem);
	xSemaphoreGive(write_sem);
	xTaskCreate(usb_console_task, "usb_console", 1024, NULL, 1, &usb_console_task_h);
}
