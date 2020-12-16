#include "atmel_start.h"
#include "hpl_pmc.h"

#include "driver/usb/usb.h"
#include "driver/usb/usb_drive.h"
#include "driver/usb/usb_console.h"
#include "driver/log_msg.h"
#include "driver/adc.h"
#include "driver/eeprom_emu.h"
#include "framework/sensor/sensor.h"
#include "framework/sensor/sensor_db.h"
#include "app/motor_controller.h"
#include "app/uart_term.h"
#include "app/analog_poll.h"

static void test_task(void *p) {
	(void)p;

	while (1) {
		vTaskDelay(789);
		float val = sensor_get_f(SENS_PRES_R_OUT);
		uint32_t ts = sensor_get_ts(SENS_PRES_R_OUT);
		log_info("%d %f", ts, val);
	}
}

int main(void) {
	// TODO don't hardcode address
	hri_matrix_write_CCFG_CAN0_CAN0DMABA_bf(MATRIX, (uint32_t)0x2042);

	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	NVIC_SetPriority(HSMCI_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
	NVIC_SetPriority(USBHS_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
	NVIC_SetPriority(MCAN0_INT0_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
	NVIC_SetPriority(MCAN0_INT1_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
	NVIC_SetPriority(UART1_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
	NVIC_SetPriority(UART2_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
	NVIC_SetPriority(UART3_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
	NVIC_SetPriority(AFEC0_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);
	NVIC_SetPriority(AFEC1_IRQn, configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY);

	_pmc_enable_periph_clock(ID_PIOB);

	gpio_set_pin_direction(PIN_PC8, GPIO_DIRECTION_OUT);
	gpio_set_pin_level(PIN_PC8, 0);

	log_init();
	log_debug("startup");

	eeprom_emu_init();

	mc_init();
	term_init();
	analog_poll_init();

	usb_drive_init();
	usb_console_init();
	usb_start();

	xTaskCreate(test_task, "test", 1024, NULL, 1, NULL);

	vTaskStartScheduler();

	while (1);
}
