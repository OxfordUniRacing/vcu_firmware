/**
 * \file
 *
 * \brief USART related functionality declaration.
 *
 * Copyright (c) 2016-2018 Microchip Technology Inc. and its subsidiaries.
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

#ifndef _HPL_UART_BASE_H_INCLUDED
#define _HPL_UART_BASE_H_INCLUDED

#include <hpl_usart_sync.h>
#include <hpl_usart_async.h>

/**
 * \addtogroup HPL USART
 *
 * \section hpl_usart_rev Revision History
 * - v1.0.0 Initial Release
 *
 *@{
 */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \name HPL functions
 */
//@{
/**
 * \brief Initialize asynchronous USART
 *
 * This function does low level USART configuration.
 *
 * \param[in] device The pointer to USART device instance
 * \param[in] hw The pointer to hardware instance
 *
 * \return Initialization status
 */
int32_t _uart_usart_async_init(struct _usart_async_device *const device, void *const hw);

/**
 * \brief De-initialize USART
 *
 * This function closes the given USART by disabling its clock.
 *
 * \param[in] device The pointer to USART device instance
 */
void _uart_usart_async_deinit(struct _usart_async_device *const device);

/**
 * \brief Enable usart module
 *
 * This function will enable the usart module
 *
 * \param[in] device The pointer to USART device instance
 */
void _uart_usart_async_enable(struct _usart_async_device *const device);

/**
 * \brief Disable usart module
 *
 * This function will disable the usart module
 *
 * \param[in] device The pointer to USART device instance
 */
void _uart_usart_async_disable(struct _usart_async_device *const device);

/**
 * \brief Calculate baud rate register value
 *
 * \param[in] baud Required baud rate
 * \param[in] clock_rate clock frequency
 * \param[in] samples The number of samples
 * \param[in] mode USART mode
 * \param[in] fraction A fraction value
 *
 * \return Calculated baud rate register value
 */
uint16_t _uart_usart_async_calculate_baud_rate(const uint32_t baud, const uint32_t clock_rate, const uint8_t samples,
                                               const enum usart_baud_rate_mode mode, const uint8_t fraction);

/**
 * \brief Set baud rate
 *
 * \param[in] device The pointer to USART device instance
 * \param[in] baud_rate A baud rate to set
 */
void _uart_usart_async_set_baud_rate(struct _usart_async_device *const device, const uint32_t baud_rate);

/**
 * \brief Set data order
 *
 * \param[in] device The pointer to USART device instance
 * \param[in] order A data order to set
 */
void _uart_usart_async_set_data_order(struct _usart_async_device *const device, const enum usart_data_order order);

/**
 * \brief Set mode
 *
 * \param[in] device The pointer to USART device instance
 * \param[in] mode A mode to set
 */
void _uart_usart_async_set_mode(struct _usart_async_device *const device, const enum usart_mode mode);

/**
 * \brief Set parity
 *
 * \param[in] device The pointer to USART device instance
 * \param[in] parity A parity to set
 */
void _uart_usart_async_set_parity(struct _usart_async_device *const device, const enum usart_parity parity);

/**
 * \brief Set stop bits mode
 *
 * \param[in] device The pointer to USART device instance
 * \param[in] stop_bits A stop bits mode to set
 */
void _uart_usart_async_set_stop_bits(struct _usart_async_device *const device, const enum usart_stop_bits stop_bits);

/**
 * \brief Set character size
 *
 * \param[in] device The pointer to USART device instance
 * \param[in] size A character size to set
 */
void _uart_usart_async_set_character_size(struct _usart_async_device *const device,
                                          const enum usart_character_size   size);

/**
 * \brief Retrieve usart status
 *
 * \param[in] device The pointer to USART device instance
 */
uint32_t _uart_usart_async_get_status(const struct _usart_async_device *const device);

/**
 * \brief Write a byte to the given USART instance
 *
 * \param[in] device The pointer to USART device instance
 * \param[in] data Data to write
 */
void _uart_usart_async_write_byte(struct _usart_async_device *const device, uint8_t data);

/**
 * \brief Check if USART is ready to send next byte
 *
 * \param[in] device The pointer to USART device instance
 *
 * \return Status of the ready check.
 * \retval true if the USART is ready to send next byte
 * \retval false if the USART is not ready to send next byte
 */
bool _uart_usart_async_is_byte_sent(const struct _usart_async_device *const device);

/**
 * \brief Set the state of flow control pins
 *
 * \param[in] device The pointer to USART device instance
 * \param[in] state - A state of flow control pins to set
 */
void _uart_usart_async_set_flow_control_state(struct _usart_async_device *const    device,
                                              const union usart_flow_control_state state);

/**
 * \brief Retrieve the state of flow control pins
 *
 * This function retrieves the of flow control pins.
 *
 * \return USART_FLOW_CONTROL_STATE_UNAVAILABLE.
 */
union usart_flow_control_state _uart_usart_async_get_flow_control_state(const struct _usart_async_device *const device);

/**
 * \brief Enable data register empty interrupt
 *
 * \param[in] device The pointer to USART device instance
 */
void _uart_usart_async_enable_byte_sent_irq(struct _usart_async_device *const device);

/**
 * \brief Enable transmission complete interrupt
 *
 * \param[in] device The pointer to USART device instance
 */
void _uart_usart_async_enable_tx_done_irq(struct _usart_async_device *const device);

/**
 * \brief Retrieve ordinal number of the given USART hardware instance
 *
 * \param[in] device The pointer to USART device instance
 *
 * \return The ordinal number of the given USART hardware instance
 */
uint8_t _uart_usart_async_get_hardware_index(const struct _usart_async_device *const device);

/**
 * \brief Enable/disable USART interrupt
 *
 * param[in] device The pointer to USART device instance
 * param[in] type The type of interrupt to disable/enable if applicable
 * param[in] state Enable or disable
 */
void _uart_usart_async_set_irq_state(struct _usart_async_device *const     device,
                                     const enum _usart_async_callback_type type, const bool state);

/**
 * \brief Retrieve timer helper functions
 *
 * \return A pointer to set of timer helper functions
 */
void *_uart_get_usart_sync(void);
void *_uart_get_usart_async(void);
void *_uart_get_usart_dma(void);

//@}

#ifdef __cplusplus
}
#endif
/**@}*/
#endif /* _HPL_UART_BASE_H_INCLUDED */
