/**
 * \file
 *
 * \brief USART related functionality declaration.
 *
 * Copyright (c) 2014-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */

#ifndef _HPL_USART_ASYNC_H_INCLUDED
#define _HPL_USART_ASYNC_H_INCLUDED

/**
 * \addtogroup HPL USART
 *
 * \section hpl_usart_rev Revision History
 * - v1.0.0 Initial Release
 *
 *@{
 */

#include "hpl_usart.h"
#include "hpl_irq.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief USART callback types
 */
enum _usart_async_callback_type { USART_ASYNC_BYTE_SENT, USART_ASYNC_RX_DONE, USART_ASYNC_TX_DONE, USART_ASYNC_ERROR };

/**
 * \brief USART device structure
 *
 * The USART device structure forward declaration.
 */
struct _usart_async_device;

/**
 * \brief USART interrupt callbacks
 */
struct _usart_async_callbacks {
	void (*tx_byte_sent)(struct _usart_async_device *device);
	void (*rx_done_cb)(struct _usart_async_device *device, uint8_t data);
	void (*tx_done_cb)(struct _usart_async_device *device);
	void (*error_cb)(struct _usart_async_device *device);
};

/**
 * \brief USART descriptor device structure
 */
struct _usart_async_device {
	struct _usart_async_callbacks usart_cb;
	struct _irq_descriptor        irq;
	void *                        hw;
};
/**
 * \brief Usart functions, pointers to low-level functions
 */
struct _usart_async_hpl_interface {
	int32_t (*async_init)(struct _usart_async_device *const device, void *const hw);
	void (*async_deinit)(struct _usart_async_device *const device);
	void (*async_enable)(struct _usart_async_device *const device);
	void (*async_disable)(struct _usart_async_device *const device);
	uint16_t (*async_calculate_baud_rate)(const uint32_t baud, const uint32_t clock_rate, const uint8_t samples,
	                                      const enum usart_baud_rate_mode mode, const uint8_t fraction);
	void (*async_set_baud_rate)(struct _usart_async_device *const device, const uint32_t baud_rate);
	void (*async_set_data_order)(struct _usart_async_device *const device, const enum usart_data_order order);
	void (*async_set_mode)(struct _usart_async_device *const device, const enum usart_mode mode);
	void (*async_set_parity)(struct _usart_async_device *const device, const enum usart_parity parity);
	void (*async_set_stop_bits)(struct _usart_async_device *const device, const enum usart_stop_bits stop_bits);
	void (*async_set_character_size)(struct _usart_async_device *const device, const enum usart_character_size size);
	uint32_t (*async_get_status)(const struct _usart_async_device *const device);
	void (*async_write_byte)(struct _usart_async_device *const device, uint8_t data);
	bool (*async_is_byte_sent)(const struct _usart_async_device *const device);
	void (*async_set_flow_control_state)(struct _usart_async_device *const    device,
	                                     const union usart_flow_control_state state);
	union usart_flow_control_state (*async_get_flow_control_state)(const struct _usart_async_device *const device);
	uint8_t (*async_get_hardware_index)(const struct _usart_async_device *const device);
	void (*async_enable_byte_sent_irq)(struct _usart_async_device *const device);
	void (*async_enable_tx_done_irq)(struct _usart_async_device *const device);
	void (*async_set_irq_state)(struct _usart_async_device *const device, const enum _usart_async_callback_type type,
	                            const bool state);
};
//@}

#ifdef __cplusplus
}
#endif
/**@}*/
#endif /* _HPL_USART_ASYNC_H_INCLUDED */
