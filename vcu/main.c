#include "atmel_start.h"
#include "hpl_pmc.h"

#include "usb/usb.h"
#include "usb/usb_drive.h"
#include "usb/usb_console.h"
#include "motor_controller.h"
#include "log/msglog.h"

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

	_pmc_enable_periph_clock(ID_PIOB);

	gpio_set_pin_direction(PIN_PC8, GPIO_DIRECTION_OUT);
	gpio_set_pin_level(PIN_PC8, 0);

	log_init();
	log_debug("startup");

	mc_init();

	usb_drive_init();
	usb_console_init();
	usb_start();

	vTaskStartScheduler();

	while (1);
}
