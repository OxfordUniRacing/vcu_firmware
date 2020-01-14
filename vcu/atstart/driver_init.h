/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef DRIVER_INIT_H_INCLUDED
#define DRIVER_INIT_H_INCLUDED

#include "atmel_start_pins.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <hal_atomic.h>
#include <hal_delay.h>
#include <hal_gpio.h>
#include <hal_init.h>
#include <hal_io.h>
#include <hal_sleep.h>

#include <hal_adc_async.h>
#include <hal_adc_async.h>

#include <hal_flash.h>

#include <hal_mci_os.h>

#include <hal_usart_async.h>
#include <hpl_uart_base.h>
#include <hal_usart_async.h>
#include <hpl_uart_base.h>

#include <hal_usart_os.h>

#include "hal_usb_device.h"

#include <hal_can_async.h>

#define USART_EDBG_BUFFER_SIZE 16

extern struct adc_async_descriptor ADC_0;

/* The enabled channel for ADC */

extern struct adc_async_descriptor ADC_1;

/* The enabled channel for ADC */

extern struct flash_descriptor FLASH;

extern struct mci_os_desc            IO_BUS;
extern struct usart_async_descriptor UART_MC_1;
extern struct usart_async_descriptor UART_MC_2;

extern struct usart_os_descriptor USART_EDBG;
extern uint8_t                    USART_EDBG_buffer[];

extern struct can_async_descriptor CAN_0;

void FLASH_init(void);
void FLASH_CLOCK_init(void);

void IO_BUS_PORT_init(void);
void IO_BUS_CLOCK_init(void);
void IO_BUS_init(void);

void UART_MC_1_PORT_init(void);
void UART_MC_1_CLOCK_init(void);
void UART_MC_1_init(void);
void UART_MC_1_example(void);

void UART_MC_2_PORT_init(void);
void UART_MC_2_CLOCK_init(void);
void UART_MC_2_init(void);
void UART_MC_2_example(void);

void USART_EDBG_PORT_init(void);
void USART_EDBG_CLOCK_init(void);
void USART_EDBG_init(void);

void USB_DEVICE_INSTANCE_CLOCK_init(void);
void USB_DEVICE_INSTANCE_init(void);

void CAN_0_PORT_init(void);
void CAN_0_CLOCK_init(void);
void CAN_0_init(void);
void CAN_0_example(void);

/**
 * \brief Perform system initialization, initialize pins and clocks for
 * peripherals
 */
void system_init(void);

#ifdef __cplusplus
}
#endif
#endif // DRIVER_INIT_H_INCLUDED
