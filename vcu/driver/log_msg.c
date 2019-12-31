#include <stdarg.h>
#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"
#include "stream_buffer.h"

#include "atmel_start.h"
#include "utils.h"

#include "log_msg.h"

#include "config.h"


StreamBufferHandle_t msglog_buffer;
static char *log_level_name[] = { "ERROR", "WARN", "INFO", "DEBUG" };

static void log_uart_task(void *);

void log_init() {
	msglog_buffer = xStreamBufferCreate(4096, 1);
	NVIC_SetPriority(USART1_IRQn, 7);
	xTaskCreate(log_uart_task, "log_uart", 1024, NULL, 2, NULL);
}

void log_log(int level, const char *fmt, ...) {
	static char buf[256];
	unsigned int len;
	int r;

	uint32_t time = xTaskGetTickCount(); // time in ms
	len = sprintf(buf, "[%04lu", time);

	// insert decimal point
	buf[len] = buf[len-1];
	buf[len-1] = buf[len-2];
	buf[len-2] = buf[len-3];
	buf[len-3] = '.';
	len += 1;

	len += sprintf(buf+len, "] %s ", log_level_name[level]);

	va_list args;
	va_start(args, fmt);
	r = vsnprintf(buf+len, sizeof(buf) - len, fmt, args);
	va_end(args);

	ASSERT(r >= 0); // format error
	if (r < 0) return;
	len += r;
	if (len >= sizeof(buf)) len = sizeof(buf)-1;
	
	buf[len] = '\n';
	len += 1;

	
	if (xPortIsInsideInterrupt()) {
		UBaseType_t uxSavedInterruptStatus = 0;
		uxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();

		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		xStreamBufferSendFromISR(msglog_buffer, buf, len, &xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

		taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptStatus);
	} else {
		taskENTER_CRITICAL();

		xStreamBufferSend(msglog_buffer, buf, len, 0);

		taskEXIT_CRITICAL();
	}
}

static void log_uart_task(void *unused) {
	(void) unused;

	ASSERT(msglog_buffer != NULL);

	struct io_descriptor *io;
	usart_os_get_io(&USART_EDBG, &io);

	static uint8_t buf[256];
	while (1) {
		int len = xStreamBufferReceive(
				msglog_buffer, buf, sizeof(buf), portMAX_DELAY);
		if (len > 0) {
			io_write(io, buf, len);
		}
	}
}
