#include "atmel_start.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "driver/log_msg.h"
#include "uart_term.h"

#include <stdlib.h>

// TODO verify newline char and line len with motor controller hardware
#define TERM_NEWLINE '\n'
// status line length including newline
#define MC_STATUS_LEN 77

#define TERM_COUNT 1
#define MC_BUF_SIZE 256


struct usart_async_descriptor *const term_uart_hal_descs[] =
	{ &UART_TERM };

struct term_cmd_t {
	enum {
		TERM_WAKE,
	} op;
	int d;
};

struct term_inst_t {

  QueueHandle_t cmd_q;
	TaskHandle_t task;

	// uart

	SemaphoreHandle_t uart_sem;

	struct _usart_async_device *uart;

	volatile uint8_t *tx_ptr;
	volatile int tx_cnt;
	SemaphoreHandle_t tx_sem;

	volatile int rx_rdy;
	struct ringbuffer rb;
	uint8_t buffer[MC_BUF_SIZE];
};


static void term_uart_tx_byte_sent(struct _usart_async_device *device);
static void term_uart_tx_done_cb(struct _usart_async_device *device);
static void term_uart_rx_done_cb(struct _usart_async_device *device, uint8_t data);
static void term_uart_error(struct _usart_async_device *device);


static struct term_inst_t term_inst[TERM_COUNT];


static bool term_poll_cmd_queue(struct term_inst_t *term) {
	struct term_cmd_t cmd;
	xQueueReceive(term->cmd_q, &cmd, portMAX_DELAY);

	switch (cmd.op) {
	case TERM_WAKE:
		break;

	default:
		log_error("term: bad command %d", cmd.op);
	}

}

static void term_parse_cmd(struct term_inst_t *term, uint8_t *s) {
	
	
	
	log_info("terminal data: %s", s);
	
	return;

	
}

static void term_task(void *p) {
	static uint8_t buf[MC_STATUS_LEN+1];
	struct term_inst_t *term = p;

	// Initialization

	term->cmd_q = xQueueCreate(4, sizeof(struct term_cmd_t));


	term->tx_cnt = 0;
	term->tx_sem = xSemaphoreCreateBinary();

	ringbuffer_init(&term->rb, term->buffer, sizeof(term->buffer));
	term->rx_rdy = 0;

	term->uart->usart_cb.tx_byte_sent = term_uart_tx_byte_sent;
	term->uart->usart_cb.tx_done_cb = term_uart_tx_done_cb;
	term->uart->usart_cb.rx_done_cb = term_uart_rx_done_cb;
	term->uart->usart_cb.error_cb = term_uart_error;

	_uart_usart_async_enable_byte_sent_irq(term->uart);
	_uart_usart_async_set_irq_state(term->uart, USART_ASYNC_RX_DONE, true);
	_uart_usart_async_set_irq_state(term->uart, USART_ASYNC_ERROR, true);

	_uart_usart_async_enable(term->uart);

	 // Main loop

	while (1) {
    term_poll_cmd_queue(term);

		if (term->rx_rdy) {
			int l = term_uart_read(term, buf, sizeof(buf)-1);
			buf[l+1] = '\0';
			term_parse_cmd(term, buf);
		}
	}
}

void term_init(void) {
	for (int i = 0; i < TERM_COUNT; i++) {
		struct term_inst_t *term = &term_inst[i];
		term->uart = &term_uart_hal_descs[i]->device;
		xTaskCreate(term_task, "term", 1024, term, 3, &term->task);
	}
}


// Read a line from uart
int term_uart_read(struct term_inst_t *term, uint8_t *dst, int size) {
	if (!term->rx_rdy) return 0;

	int count = 0;
	while (count != size) {
		int r;
		uint8_t d;

		taskENTER_CRITICAL();
		r = ringbuffer_get(&term->rb, &d);
		taskEXIT_CRITICAL();

		ASSERT(r == ERR_NONE);
		dst[count++] = d;
		if (d == TERM_NEWLINE) break;
	}

	taskENTER_CRITICAL();
	term->rx_rdy--;
	taskEXIT_CRITICAL();

	return count;
}

// Write to uart
void term_uart_write(struct term_inst_t *term, uint8_t *src, int count) {
	xSemaphoreTake(term->tx_sem, portMAX_DELAY);

	term->tx_ptr = src;
	term->tx_cnt = count;
	_uart_usart_async_enable_byte_sent_irq(term->uart);
}

static void term_uart_tx_byte_sent(struct _usart_async_device *device) {
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	struct term_inst_t *term = NULL;
	for (int i = 0; i < TERM_COUNT; i++) {
		if (term_inst[i].uart == device) term = &term_inst[i];
	}
	ASSERT(term != NULL);

	if (term->tx_cnt) {
		_uart_usart_async_write_byte(device, *term->tx_ptr);
		_uart_usart_async_enable_byte_sent_irq(device);
		term->tx_ptr++;
		term->tx_cnt--;
	} else {
		xSemaphoreGiveFromISR(term->tx_sem, &xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
}

static void term_uart_tx_done_cb(struct _usart_async_device *device) {
	// this callback should not be called
	(void)device;
	ASSERT(false);
}

static void term_uart_rx_done_cb(struct _usart_async_device *device, uint8_t data) {
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	struct term_inst_t *term = NULL;
	for (int i = 0; i < TERM_COUNT; i++) {
		if (term_inst[i].uart == device) term = &term_inst[i];
	}
	ASSERT(term != NULL);

	/*
	if (term->rb.write_index - term->rb.read_index == term->rb.size) {
		log_error("term: uart ringbuffer overrun");
	}
	*/

	ringbuffer_put(&term->rb, data);

	if (data == TERM_NEWLINE) {
		term->rx_rdy++; // no atomic necessary because irq is higher priority
		struct term_cmd_t cmd = { TERM_WAKE };
		xQueueSendToBackFromISR(term->cmd_q, &cmd, &xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
}

static void term_uart_error(struct _usart_async_device *device) {
	(void)device;
	log_error("term: uart error");
}
