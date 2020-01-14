/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_examples.h"
#include "driver_init.h"
#include "utils.h"

/**
 * Example of using ADC_0 to generate waveform.
 */
void ADC_0_example(void)
{

	adc_async_start_conversion(&ADC_0);
}

/**
 * Example of using ADC_1 to generate waveform.
 */
void ADC_1_example(void)
{

	adc_async_start_conversion(&ADC_1);
}

static uint8_t src_data[IFLASH_PAGE_SIZE];
static uint8_t chk_data[IFLASH_PAGE_SIZE];

/**
 * Example of using FLASH to read and write buffer.
 */
void FLASH_example(void)
{
	uint32_t page_size;
	uint16_t i;

	/* Init source data */
	page_size = flash_get_page_size(&FLASH);

	for (i = 0; i < page_size; i++) {
		src_data[i] = i;
	}

	/* Write data to flash */
	flash_write(&FLASH, 0x3200, src_data, page_size);

	/* Read data from flash */
	flash_read(&FLASH, 0x3200, chk_data, page_size);
}

/**
 * Example of using UART_MC_1 to write "Hello World" using the IO abstraction.
 *
 * Since the driver is asynchronous we need to use statically allocated memory for string
 * because driver initiates transfer and then returns before the transmission is completed.
 *
 * Once transfer has been completed the tx_cb function will be called.
 */

static uint8_t example_UART_MC_1[12] = "Hello World!";

static void tx_cb_UART_MC_1(const struct usart_async_descriptor *const io_descr)
{
	/* Transfer completed */
}

void UART_MC_1_example(void)
{
	struct io_descriptor *io;

	usart_async_register_callback(&UART_MC_1, USART_ASYNC_TXC_CB, tx_cb_UART_MC_1);
	/*usart_async_register_callback(&UART_MC_1, USART_ASYNC_RXC_CB, rx_cb);
	usart_async_register_callback(&UART_MC_1, USART_ASYNC_ERROR_CB, err_cb);*/
	usart_async_get_io_descriptor(&UART_MC_1, &io);
	usart_async_enable(&UART_MC_1);

	io_write(io, example_UART_MC_1, 12);
}

/**
 * Example of using UART_MC_2 to write "Hello World" using the IO abstraction.
 *
 * Since the driver is asynchronous we need to use statically allocated memory for string
 * because driver initiates transfer and then returns before the transmission is completed.
 *
 * Once transfer has been completed the tx_cb function will be called.
 */

static uint8_t example_UART_MC_2[12] = "Hello World!";

static void tx_cb_UART_MC_2(const struct usart_async_descriptor *const io_descr)
{
	/* Transfer completed */
}

void UART_MC_2_example(void)
{
	struct io_descriptor *io;

	usart_async_register_callback(&UART_MC_2, USART_ASYNC_TXC_CB, tx_cb_UART_MC_2);
	/*usart_async_register_callback(&UART_MC_2, USART_ASYNC_RXC_CB, rx_cb);
	usart_async_register_callback(&UART_MC_2, USART_ASYNC_ERROR_CB, err_cb);*/
	usart_async_get_io_descriptor(&UART_MC_2, &io);
	usart_async_enable(&UART_MC_2);

	io_write(io, example_UART_MC_2, 12);
}

#define TASK_USART_EDBG_STACK_SIZE (300 / sizeof(portSTACK_TYPE))
#define TASK_USART_EDBG_STACK_PRIORITY (tskIDLE_PRIORITY + 1)

static TaskHandle_t xCreatedusart_edbgTask;
static uint8_t      example_USART_EDBG[12] = "Hello World!";

/**
 * Example task of using USART_EDBG to echo using the IO abstraction.
 */
static void USART_EDBG_example_task(void *param)
{
	struct io_descriptor *io;
	uint16_t              data;

	(void)param;

	usart_os_get_io(&USART_EDBG, &io);
	io_write(io, example_USART_EDBG, 12);

	for (;;) {
		if (io_read(io, (uint8_t *)&data, 2) == 2) {
			io_write(io, (uint8_t *)&data, 2);
		}
		os_sleep(10);
	}
}

/**
 * \brief Create OS task for USART_EDBG
 */
void task_usart_edbg_create()
{
	/* Create task for USART_EDBG */
	if (xTaskCreate(USART_EDBG_example_task,
	                "usart_edbg",
	                TASK_USART_EDBG_STACK_SIZE,
	                NULL,
	                TASK_USART_EDBG_STACK_PRIORITY,
	                &xCreatedusart_edbgTask)
	    != pdPASS) {
		while (1) {
			/* Please checkup stack and FreeRTOS configuration. */
		}
	}
	/* Call vTaskStartScheduler() function in main function. Place vTaskStartScheduler function call after creating all
	 * tasks and before while(1) in main function */
}

void CAN_0_tx_callback(struct can_async_descriptor *const descr)
{
	(void)descr;
}
void CAN_0_rx_callback(struct can_async_descriptor *const descr)
{
	struct can_message msg;
	uint8_t            data[64];
	msg.data = data;
	can_async_read(descr, &msg);
	return;
}

/**
 * Example of using CAN_0 to Encrypt/Decrypt datas.
 */
void CAN_0_example(void)
{
	struct can_message msg;
	struct can_filter  filter;
	uint8_t            send_data[4];
	send_data[0] = 0x00;
	send_data[1] = 0x01;
	send_data[2] = 0x02;
	send_data[3] = 0x03;

	msg.id   = 0x45A;
	msg.type = CAN_TYPE_DATA;
	msg.data = send_data;
	msg.len  = 4;
	msg.fmt  = CAN_FMT_STDID;
	can_async_register_callback(&CAN_0, CAN_ASYNC_TX_CB, (FUNC_PTR)CAN_0_tx_callback);
	can_async_enable(&CAN_0);
	/**
	 * CAN_0_tx_callback callback should be invoked after call
	 * can_async_write, and remote device should recieve message with ID=0x45A
	 */
	can_async_write(&CAN_0, &msg);

	msg.id  = 0x100000A5;
	msg.fmt = CAN_FMT_EXTID;
	/**
	 * remote device should recieve message with ID=0x100000A5
	 */
	can_async_write(&CAN_0, &msg);

	/**
	 * CAN_0_rx_callback callback should be invoked after call
	 * can_async_set_filter and remote device send CAN Message with the same
	 * content as the filter.
	 */
	can_async_register_callback(&CAN_0, CAN_ASYNC_RX_CB, (FUNC_PTR)CAN_0_rx_callback);
	filter.id   = 0x469;
	filter.mask = 0;
	can_async_set_filter(&CAN_0, 0, CAN_FMT_STDID, &filter);

	filter.id   = 0x10000096;
	filter.mask = 0;
	can_async_set_filter(&CAN_0, 1, CAN_FMT_EXTID, &filter);
}
