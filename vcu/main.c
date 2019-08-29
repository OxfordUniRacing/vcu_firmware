#include "atmel_start.h"
#include "hpl_pmc.h"

#include "usb_drive.h"
#include "usb_init.h"
#include "log/msglog.h"

static void CAN_0_tx_callback(struct can_async_descriptor *const descr) {
	gpio_toggle_pin_level(PIN_PC8);
	(void)descr;
}

static void CAN_0_rx_callback(struct can_async_descriptor *const descr) {
	struct can_message msg;
	uint8_t data[8];
	msg.data = data;
	can_async_read(descr, &msg);
}

extern uint8_t *can0_rx_fifo;
static void setup_can(void) {

	can_async_register_callback(&CAN_0, CAN_ASYNC_RX_CB, CAN_0_rx_callback);
	can_async_register_callback(&CAN_0, CAN_ASYNC_TX_CB, CAN_0_tx_callback);
	can_async_enable(&CAN_0);

	struct can_filter filter;
	filter.id   = 0x555;
	filter.mask = 0;
	can_async_set_filter(&CAN_0, 0, CAN_FMT_STDID, &filter);
}

static void do_can_tx(void) {
	static uint32_t counter;

	struct can_message msg;
	uint32_t data = __builtin_bswap32(counter);
	//uint8_t data[8];
	//data[0] = counter + 42;

	msg.id   = 0x123;
	msg.type = CAN_TYPE_DATA;
	msg.data = &data;
	msg.len  = 4;
	msg.fmt  = CAN_FMT_STDID;

	can_async_write(&CAN_0, &msg);

	counter++;
}

static void can_task(void *p) {
	(void)p;

	setup_can();

	//hri_mcan_set_CCCR_TEST_bit(MCAN0);
	//hri_mcan_set_TEST_LBCK_bit(MCAN0);

	while (1) {
		do_can_tx();
		for (int i = 0; i < 1000000; i++);
	}
}

static void test_task(void *unused) {
	(void) unused;

	while (1) {
		log_debug("x");
		vTaskDelay(100);
	}
}

int main(void) {
	hri_matrix_write_CCFG_CAN0_CAN0DMABA_bf(MATRIX, (uint32_t)0x2042);

	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	NVIC_SetPriority(HSMCI_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
	NVIC_SetPriority(USBHS_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
	NVIC_SetPriority(MCAN0_INT0_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
	NVIC_SetPriority(MCAN0_INT1_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);

	_pmc_enable_periph_clock(ID_PIOB);

	gpio_set_pin_direction(PIN_PC8, GPIO_DIRECTION_OUT);
	gpio_set_pin_level(PIN_PC8, 0);

	//int pin = PIN_PB3;

	//gpio_set_pin_direction(pin, GPIO_DIRECTION_IN);
	//gpio_set_pin_pull_mode(pin, GPIO_PULL_DOWN);
	//gpio_set_pin_function(pin, GPIO_PIN_FUNCTION_OFF);

	//while (1) {
	//	int x = gpio_get_pin_level(pin);
	//	gpio_set_pin_level(PIN_PC8, x);
	//}

	//usb_drive_init();
	//usb_start();

	//can_task(0);

	//xTaskCreate(can_task, "", 1024, NULL, 1, NULL);
	
	log_init();
	xTaskCreate(test_task, "test", 1024, NULL, tskIDLE_PRIORITY + 1, NULL);

	vTaskStartScheduler();
}
