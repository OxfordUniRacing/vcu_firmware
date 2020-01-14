/* motor.c
 *
 * Provides high level interface to motor controllers
 *
 * Starts 1 rtos task per mc
 *
 * We don't use the usart hal
 *
 * Only one task at a time may call mc_uart_{read,write}
 * The mc task is allowed to use them when !mc->passthru
 * The usb serial task is allowed to use them when mc->passthru
 */

#include "atmel_start.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "driver/log_msg.h"
#include "motor_controller.h"

// TODO verify newline char and line len with motor controller hardware
#define MC_NEWLINE '\n'
// status line length including newline
#define MC_STATUS_LEN 77

#define MC_COUNT 2
#define MC_BUF_SIZE 256

struct usart_async_descriptor *const uart_hal_descs[] =
	{ &UART_MC_1, &UART_MC_2 };


extern TaskHandle_t usb_console_task_h;

struct mc_cmd_t {
	enum {
		MC_WAKE,
		MC_ENABLE,
		MC_DISABLE,
		MC_PASSTHRU_ON,
		MC_PASSTHRU_OFF,
		MC_THROTTLE,
	} op;
	int d;
};

struct mc_inst_t {
	volatile bool enabled;
	volatile bool passthru;

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

static void mc_uart_tx_byte_sent(struct _usart_async_device *device);
static void mc_uart_tx_done_cb(struct _usart_async_device *device);
static void mc_uart_rx_done_cb(struct _usart_async_device *device, uint8_t data);
static void mc_uart_error(struct _usart_async_device *device);


static struct mc_inst_t mc_inst[MC_COUNT];

struct mc_inst_t *mc_get_inst(unsigned int i) {
	ASSERT(i < MC_COUNT);
	return &mc_inst[i];
}

void mc_passthru_enable(struct mc_inst_t *mc) {
	struct mc_cmd_t cmd;
	cmd.op = MC_PASSTHRU_ON;
	xQueueSendToBack(mc->cmd_q, &cmd, portMAX_DELAY);
	while (!mc->passthru) {
		vTaskDelay(1);
	}
}

void mc_passthru_disable(struct mc_inst_t *mc) {
	struct mc_cmd_t cmd;
	cmd.op = MC_PASSTHRU_OFF;
	xQueueSendToBack(mc->cmd_q, &cmd, portMAX_DELAY);
}

static void mc_parse_stat(struct mc_inst_t *mc, uint8_t *s) {
}

static void mc_internal_set_throttle(struct mc_inst_t *mc, int throttle) {
}

static bool mc_poll_cmd_queue(struct mc_inst_t *mc) {
	struct mc_cmd_t cmd;
	xQueueReceive(mc->cmd_q, &cmd, portMAX_DELAY);

	switch (cmd.op) {
	case MC_WAKE:
		break;

	case MC_ENABLE:
		if (mc->passthru) {
			log_error("mc: cannot enable when in passthrough");
		} else {
			if (mc->enabled) log_warn("mc: already enabled");
			mc->enabled = true;
		}
		break;

	case MC_DISABLE:
		if (!mc->enabled) log_warn("mc: already disabled");
		mc->enabled = false;
		break;

	case MC_PASSTHRU_ON:
		if (mc->enabled) {
			log_error("mc: cannot passthrough uart when motor enabled");
		} else {
			log_info("mc: passthrough enabled");
			mc->passthru = true;
		}
		break;

	case MC_PASSTHRU_OFF:
		log_info("mc: passthrough disabled");
		mc->passthru = false;
		break;

	case MC_THROTTLE:
		if (mc->passthru) {
			log_error("mc: cannot set throttle when in passthrough");
		} else if (!mc->enabled) {
			log_error("mc: cannot set throttle when disabled");
		} else {
			mc_internal_set_throttle(mc, cmd.d);
		}
		break;

	default:
		log_error("mc: bad command %d", cmd.op);
	}

}

static void mc_task(void *p) {
	static uint8_t buf[MC_STATUS_LEN+1];
	struct mc_inst_t *mc = p;

	// Initialization

	mc->enabled = false;
	mc->passthru = false;

	mc->cmd_q = xQueueCreate(4, sizeof(struct mc_cmd_t));

	mc->tx_cnt = 0;
	mc->tx_sem = xSemaphoreCreateBinary();

	ringbuffer_init(&mc->rb, mc->buffer, sizeof(mc->buffer));
	mc->rx_rdy = 0;

	mc->uart->usart_cb.tx_byte_sent = mc_uart_tx_byte_sent;
	mc->uart->usart_cb.tx_done_cb = mc_uart_tx_done_cb;
	mc->uart->usart_cb.rx_done_cb = mc_uart_rx_done_cb;
	mc->uart->usart_cb.error_cb = mc_uart_error;

	_uart_usart_async_enable_byte_sent_irq(mc->uart);
	_uart_usart_async_set_irq_state(mc->uart, USART_ASYNC_RX_DONE, true);
	_uart_usart_async_set_irq_state(mc->uart, USART_ASYNC_ERROR, true);

	_uart_usart_async_enable(mc->uart);

	 // Main loop

	while (1) {
		mc_poll_cmd_queue(mc);

		if (mc->rx_rdy && !mc->passthru) {
			int l = mc_uart_read(mc, buf, sizeof(buf)-1);
			buf[l+1] = '\0';
			mc_parse_stat(mc, buf);
		}
	}
}

void mc_init(void) {
	for (int i = 0; i < MC_COUNT; i++) {
		struct mc_inst_t *mc = &mc_inst[i];
		mc->uart = &uart_hal_descs[i]->device;
		xTaskCreate(mc_task, "mc", 1024, mc, 3, &mc->task);
	}
}


// Read a line from uart
int mc_uart_read(struct mc_inst_t *mc, uint8_t *dst, int size) {
	if (!mc->rx_rdy) return 0;

	int count = 0;
	while (count != size) {
		int r;
		uint8_t d;

		taskENTER_CRITICAL();
		r = ringbuffer_get(&mc->rb, &d);
		taskEXIT_CRITICAL();

		ASSERT(r == ERR_NONE);
		dst[count++] = d;
		if (d == MC_NEWLINE) break;
	}

	taskENTER_CRITICAL();
	mc->rx_rdy--;
	taskEXIT_CRITICAL();

	return count;
}

// Write to uart
void mc_uart_write(struct mc_inst_t *mc, uint8_t *src, int count) {
	xSemaphoreTake(mc->tx_sem, portMAX_DELAY);

	mc->tx_ptr = src;
	mc->tx_cnt = count;
	_uart_usart_async_enable_byte_sent_irq(mc->uart);
}

static void mc_uart_tx_byte_sent(struct _usart_async_device *device) {
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	struct mc_inst_t *mc = NULL;
	for (int i = 0; i < MC_COUNT; i++) {
		if (mc_inst[i].uart == device) mc = &mc_inst[i];
	}
	ASSERT(mc != NULL);

	if (mc->tx_cnt) {
		_uart_usart_async_write_byte(device, *mc->tx_ptr);
		_uart_usart_async_enable_byte_sent_irq(device);
		mc->tx_ptr++;
		mc->tx_cnt--;
	} else {
		xSemaphoreGiveFromISR(mc->tx_sem, &xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
}

static void mc_uart_tx_done_cb(struct _usart_async_device *device) {
	// this callback should not be called
	(void)device;
	ASSERT(false);
}

static void mc_uart_rx_done_cb(struct _usart_async_device *device, uint8_t data) {
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

	struct mc_inst_t *mc = NULL;
	for (int i = 0; i < MC_COUNT; i++) {
		if (mc_inst[i].uart == device) mc = &mc_inst[i];
	}
	ASSERT(mc != NULL);

	/*
	if (mc->rb.write_index - mc->rb.read_index == mc->rb.size) {
		log_error("mc: uart ringbuffer overrun");
	}
	*/

	ringbuffer_put(&mc->rb, data);

	if (data == MC_NEWLINE) {
		mc->rx_rdy++; // no atomic necessary because irq is higher priority
		struct mc_cmd_t cmd = { MC_WAKE };
		xQueueSendToBackFromISR(mc->cmd_q, &cmd, &xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	}
}

static void mc_uart_error(struct _usart_async_device *device) {
	(void)device;
	log_error("mc: uart error");
}
