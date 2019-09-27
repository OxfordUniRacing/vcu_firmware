/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_init.h"
#include <hal_init.h>
#include <hpl_pmc.h>
#include <peripheral_clk_config.h>
#include <utils.h>
#include <hpl_usart_base.h>

/* The priority of the peripheral should be between the low and high interrupt priority set by chosen RTOS,
 * Otherwise, some of the RTOS APIs may fail to work inside interrupts
 * In case of FreeRTOS,the Lowest Interrupt priority is set by configLIBRARY_LOWEST_INTERRUPT_PRIORITY and
 * Maximum interrupt priority by configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY, So Interrupt Priority of the peripheral
 * should be between configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY and configLIBRARY_LOWEST_INTERRUPT_PRIORITY
 */
#define PERIPHERAL_INTERRUPT_PRIORITY (configLIBRARY_LOWEST_INTERRUPT_PRIORITY - 1)

struct can_async_descriptor CAN_0;

struct flash_descriptor FLASH;

struct mci_os_desc IO_BUS;

struct usart_os_descriptor UART_MC_1;
uint8_t                    UART_MC_1_buffer[UART_MC_1_BUFFER_SIZE];

struct usart_os_descriptor UART_MC_2;
uint8_t                    UART_MC_2_buffer[UART_MC_2_BUFFER_SIZE];

struct usart_os_descriptor USART_EDBG;
uint8_t                    USART_EDBG_buffer[USART_EDBG_BUFFER_SIZE];

void FLASH_CLOCK_init(void)
{
}

void FLASH_init(void)
{
	FLASH_CLOCK_init();
	flash_init(&FLASH, EFC);
}

void IO_BUS_PORT_init(void)
{

	gpio_set_pin_direction(PA28,
	                       // <y> Pin direction
	                       // <id> pad_direction
	                       // <GPIO_DIRECTION_OFF"> Off
	                       // <GPIO_DIRECTION_IN"> In
	                       // <GPIO_DIRECTION_OUT"> Out
	                       GPIO_DIRECTION_OUT);

	gpio_set_pin_level(PA28,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	gpio_set_pin_pull_mode(PA28,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PA28,
	                      // <y> Pin function
	                      // <id> pad_function
	                      // <i> Auto : use driver pinmux if signal is imported by driver, else turn off function
	                      // <MUX_PA28C_HSMCI_MCCDA"> Auto
	                      // <GPIO_PIN_FUNCTION_OFF"> Off
	                      // <GPIO_PIN_FUNCTION_A"> A
	                      // <GPIO_PIN_FUNCTION_B"> B
	                      // <GPIO_PIN_FUNCTION_C"> C
	                      // <GPIO_PIN_FUNCTION_D"> D
	                      MUX_PA28C_HSMCI_MCCDA);

	gpio_set_pin_direction(PA25,
	                       // <y> Pin direction
	                       // <id> pad_direction
	                       // <GPIO_DIRECTION_OFF"> Off
	                       // <GPIO_DIRECTION_IN"> In
	                       // <GPIO_DIRECTION_OUT"> Out
	                       GPIO_DIRECTION_OUT);

	gpio_set_pin_level(PA25,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	gpio_set_pin_pull_mode(PA25,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PA25,
	                      // <y> Pin function
	                      // <id> pad_function
	                      // <i> Auto : use driver pinmux if signal is imported by driver, else turn off function
	                      // <MUX_PA25D_HSMCI_MCCK"> Auto
	                      // <GPIO_PIN_FUNCTION_OFF"> Off
	                      // <GPIO_PIN_FUNCTION_A"> A
	                      // <GPIO_PIN_FUNCTION_B"> B
	                      // <GPIO_PIN_FUNCTION_C"> C
	                      // <GPIO_PIN_FUNCTION_D"> D
	                      MUX_PA25D_HSMCI_MCCK);

	gpio_set_pin_direction(PA30,
	                       // <y> Pin direction
	                       // <id> pad_direction
	                       // <GPIO_DIRECTION_OFF"> Off
	                       // <GPIO_DIRECTION_IN"> In
	                       // <GPIO_DIRECTION_OUT"> Out
	                       GPIO_DIRECTION_OUT);

	gpio_set_pin_level(PA30,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	gpio_set_pin_pull_mode(PA30,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PA30,
	                      // <y> Pin function
	                      // <id> pad_function
	                      // <i> Auto : use driver pinmux if signal is imported by driver, else turn off function
	                      // <MUX_PA30C_HSMCI_MCDA0"> Auto
	                      // <GPIO_PIN_FUNCTION_OFF"> Off
	                      // <GPIO_PIN_FUNCTION_A"> A
	                      // <GPIO_PIN_FUNCTION_B"> B
	                      // <GPIO_PIN_FUNCTION_C"> C
	                      // <GPIO_PIN_FUNCTION_D"> D
	                      MUX_PA30C_HSMCI_MCDA0);

	gpio_set_pin_direction(PA31,
	                       // <y> Pin direction
	                       // <id> pad_direction
	                       // <GPIO_DIRECTION_OFF"> Off
	                       // <GPIO_DIRECTION_IN"> In
	                       // <GPIO_DIRECTION_OUT"> Out
	                       GPIO_DIRECTION_OUT);

	gpio_set_pin_level(PA31,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	gpio_set_pin_pull_mode(PA31,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PA31,
	                      // <y> Pin function
	                      // <id> pad_function
	                      // <i> Auto : use driver pinmux if signal is imported by driver, else turn off function
	                      // <MUX_PA31C_HSMCI_MCDA1"> Auto
	                      // <GPIO_PIN_FUNCTION_OFF"> Off
	                      // <GPIO_PIN_FUNCTION_A"> A
	                      // <GPIO_PIN_FUNCTION_B"> B
	                      // <GPIO_PIN_FUNCTION_C"> C
	                      // <GPIO_PIN_FUNCTION_D"> D
	                      MUX_PA31C_HSMCI_MCDA1);

	gpio_set_pin_direction(PA26,
	                       // <y> Pin direction
	                       // <id> pad_direction
	                       // <GPIO_DIRECTION_OFF"> Off
	                       // <GPIO_DIRECTION_IN"> In
	                       // <GPIO_DIRECTION_OUT"> Out
	                       GPIO_DIRECTION_OUT);

	gpio_set_pin_level(PA26,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	gpio_set_pin_pull_mode(PA26,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PA26,
	                      // <y> Pin function
	                      // <id> pad_function
	                      // <i> Auto : use driver pinmux if signal is imported by driver, else turn off function
	                      // <MUX_PA26C_HSMCI_MCDA2"> Auto
	                      // <GPIO_PIN_FUNCTION_OFF"> Off
	                      // <GPIO_PIN_FUNCTION_A"> A
	                      // <GPIO_PIN_FUNCTION_B"> B
	                      // <GPIO_PIN_FUNCTION_C"> C
	                      // <GPIO_PIN_FUNCTION_D"> D
	                      MUX_PA26C_HSMCI_MCDA2);

	gpio_set_pin_direction(PA27,
	                       // <y> Pin direction
	                       // <id> pad_direction
	                       // <GPIO_DIRECTION_OFF"> Off
	                       // <GPIO_DIRECTION_IN"> In
	                       // <GPIO_DIRECTION_OUT"> Out
	                       GPIO_DIRECTION_OUT);

	gpio_set_pin_level(PA27,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	gpio_set_pin_pull_mode(PA27,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(PA27,
	                      // <y> Pin function
	                      // <id> pad_function
	                      // <i> Auto : use driver pinmux if signal is imported by driver, else turn off function
	                      // <MUX_PA27C_HSMCI_MCDA3"> Auto
	                      // <GPIO_PIN_FUNCTION_OFF"> Off
	                      // <GPIO_PIN_FUNCTION_A"> A
	                      // <GPIO_PIN_FUNCTION_B"> B
	                      // <GPIO_PIN_FUNCTION_C"> C
	                      // <GPIO_PIN_FUNCTION_D"> D
	                      MUX_PA27C_HSMCI_MCDA3);
}

void IO_BUS_CLOCK_init(void)
{
	_pmc_enable_periph_clock(ID_HSMCI);
}

void IO_BUS_init(void)
{
	IO_BUS_CLOCK_init();
	mci_os_init(&IO_BUS, HSMCI);
	NVIC_SetPriority(HSMCI_IRQn, PERIPHERAL_INTERRUPT_PRIORITY);
	IO_BUS_PORT_init();
}

void UART_MC_1_PORT_init(void)
{

	gpio_set_pin_function(PA5, MUX_PA5C_UART1_URXD1);

	gpio_set_pin_function(PA6, MUX_PA6C_UART1_UTXD1);
}

void UART_MC_1_CLOCK_init(void)
{
	_pmc_enable_periph_clock(ID_UART1);
}

void UART_MC_1_init(void)
{
	UART_MC_1_CLOCK_init();
	usart_os_init(&UART_MC_1, UART1, UART_MC_1_buffer, UART_MC_1_BUFFER_SIZE, (void *)_uart_get_usart_async());
	usart_os_enable(&UART_MC_1);
	UART_MC_1_PORT_init();
	NVIC_SetPriority(UART1_IRQn, PERIPHERAL_INTERRUPT_PRIORITY);
}

void UART_MC_2_PORT_init(void)
{

	gpio_set_pin_function(PD25, MUX_PD25C_UART2_URXD2);

	gpio_set_pin_function(PD26, MUX_PD26C_UART2_UTXD2);
}

void UART_MC_2_CLOCK_init(void)
{
	_pmc_enable_periph_clock(ID_UART2);
}

void UART_MC_2_init(void)
{
	UART_MC_2_CLOCK_init();
	usart_os_init(&UART_MC_2, UART2, UART_MC_2_buffer, UART_MC_2_BUFFER_SIZE, (void *)_uart_get_usart_async());
	usart_os_enable(&UART_MC_2);
	UART_MC_2_PORT_init();
	NVIC_SetPriority(UART2_IRQn, PERIPHERAL_INTERRUPT_PRIORITY);
}

void USART_EDBG_PORT_init(void)
{

	gpio_set_pin_function(PA21, MUX_PA21A_USART1_RXD1);

	gpio_set_pin_function(PB4, MUX_PB4D_USART1_TXD1);
}

void USART_EDBG_CLOCK_init(void)
{
	_pmc_enable_periph_clock(ID_USART1);
}

void USART_EDBG_init(void)
{
	USART_EDBG_CLOCK_init();
	usart_os_init(&USART_EDBG, USART1, USART_EDBG_buffer, USART_EDBG_BUFFER_SIZE, (void *)_usart_get_usart_async());
	NVIC_SetPriority(USART1_IRQn, PERIPHERAL_INTERRUPT_PRIORITY);
	usart_os_enable(&USART_EDBG);
	USART_EDBG_PORT_init();
}

/* The USB module requires a GCLK_USB of 48 MHz ~ 0.25% clock
 * for low speed and full speed operation. */
#if (CONF_USBHS_SRC == CONF_SRC_USB_48M)
#if (CONF_USBHS_FREQUENCY > (48000000 + 48000000 / 400)) || (CONF_USBHS_FREQUENCY < (48000000 - 48000000 / 400))
#warning USB clock should be 48MHz ~ 0.25% clock, check your configuration!
#endif
#endif

void USB_DEVICE_INSTANCE_CLOCK_init(void)
{
	_pmc_enable_periph_clock(ID_USBHS);
}

void USB_DEVICE_INSTANCE_init(void)
{
	USB_DEVICE_INSTANCE_CLOCK_init();
	usb_d_init();
}

/**
 * \brief MCAN Clock initialization function
 *
 * Enables register interface and peripheral clock
 */
void CAN_0_CLOCK_init()
{
	_pmc_enable_periph_clock(ID_MCAN0);
}

/**
 * \brief MCAN pinmux initialization function
 *
 * Set each required pin to MCAN functionality
 */
void CAN_0_PORT_init(void)
{

	gpio_set_pin_function(PB3, MUX_PB3A_MCAN0_CANRX0);

	gpio_set_pin_function(PB2, MUX_PB2A_MCAN0_CANTX0);
}
/**
 * \brief CAN initialization function
 *
 * Enables CAN peripheral, clocks and initializes CAN driver
 */
void CAN_0_init(void)
{
	CAN_0_CLOCK_init();
	CAN_0_PORT_init();
	can_async_init(&CAN_0, MCAN0);
}

void system_init(void)
{
	init_mcu();

	_pmc_enable_periph_clock(ID_PIOC);

	/* Disable Watchdog */
	hri_wdt_set_MR_WDDIS_bit(WDT);

	/* GPIO on PC16 */

	// Set pin direction to input
	gpio_set_pin_direction(CARD_DETECT_0, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(CARD_DETECT_0,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(CARD_DETECT_0, GPIO_PIN_FUNCTION_OFF);

	FLASH_init();

	IO_BUS_init();

	UART_MC_1_init();

	UART_MC_2_init();

	USART_EDBG_init();

	USB_DEVICE_INSTANCE_init();

	CAN_0_init();
}
