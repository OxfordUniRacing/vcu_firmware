#include "atmel_start.h"
#include "FreeRTOS.h"
#include "task.h"

#include <stdio.h>
#include <stdarg.h>

#include "log/msglog.h"
#include "usb/usb_console.h"

#define BUF_SIZE CONF_USB_CDCD_ACM_DATA_BULKIN_MAXPKSZ_HS

TaskHandle_t usb_console_task_h;

static uint8_t read_buf[BUF_SIZE] __attribute__ ((aligned (4)));
static uint8_t write_buf[BUF_SIZE] __attribute__ ((aligned (4)));
static int read_index;
static volatile int read_avail;
static volatile int write_stat;

static bool usb_device_cb_bulk_out(const uint8_t ep, const enum usb_xfer_code rc, const uint32_t count) {
	(void)ep;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	if (rc == USB_XFER_DONE) {
		read_avail = count;
	} else {
		read_avail = -rc;
	}

	vTaskNotifyGiveFromISR(usb_console_task_h, &xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

	return false;
}

static bool usb_device_cb_bulk_in(const uint8_t ep, const enum usb_xfer_code rc, const uint32_t count) {
	(void)ep;
	(void)count;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	if (rc == USB_XFER_DONE) {
		write_stat = 0;
	} else {
		write_stat = -rc;
	}

	vTaskNotifyGiveFromISR(usb_console_task_h, &xHigherPriorityTaskWoken);
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

	return false;
}

int cwrite(const void *src, unsigned int count) {
	while (count > 0) {
		int c = count;
		if (c > BUF_SIZE) c = BUF_SIZE;

		while (write_stat > 0)
			ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		if (write_stat < 0) {
			log_warn("cdc_acm out ep error: %d", -write_stat);
			return -1;
		}

		memcpy(write_buf, src, c);

		write_stat = 1;
		int r = cdcdf_acm_write(write_buf, c);
		if (r) {
			log_warn("cdcdf_acm_write failed: %d", r);
			return -1;
		}

		count -= c;
		src += c;
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
		return r;
	}
	if (r >= (int)sizeof(buf))
		r = sizeof(buf)-1;
	cwrite(buf, r);
	va_end(args);
	return r;
}

int cgetc(void) {
	if (!read_avail) {
		int r = cdcdf_acm_read(read_buf, BUF_SIZE);
		if (r) {
			log_warn("cdcdf_acm_read failed: %d", r);
			return -1;
		}

		while (!read_avail)
			ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		if (read_avail < 0) {
			log_warn("cdc_acm in ep error: %d", -read_avail);
			return -1;
		}

		read_index = 0;
	}

	read_avail--;
	return read_buf[read_index++];
}

int cgets(char *s, int size) {
	int count = 0;
	int c;
	do {
		c = cgetc();
		if (c < 0) return -1;
		if (count < size) {
			s[count] = c;
			count++;
		}
	} while (c != '\r' && c != '\n');

	s[count-1] = '\0';
	return count-1;
}

static void usb_console_task(void *p) {
	(void)p;

	while (1) {
		while (!cdcdf_acm_is_enabled()) {
			vTaskDelay(100);
		}
		log_info("usb cdc_acm enabled"); 

		cdcdf_acm_register_callback(CDCDF_ACM_CB_READ, (FUNC_PTR)usb_device_cb_bulk_out);
		cdcdf_acm_register_callback(CDCDF_ACM_CB_WRITE, (FUNC_PTR)usb_device_cb_bulk_in);

		cmdline_main();
	}
}

void usb_console_init(void) {
	xTaskCreate(usb_console_task, "", 1024, NULL, 1, &usb_console_task_h);
}
