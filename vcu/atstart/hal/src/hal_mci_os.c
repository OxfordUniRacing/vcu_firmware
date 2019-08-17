/**
 * \file
 *
 * \brief Multimedia Card/ Memory Card Interface HAL.
 *
 * Copyright (c) 2017-2018 Microchip Technology Inc. and its subsidiaries.
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

#include <hal_mci_os.h>
#include <utils_assert.h>

/** MCI HAL OS driver version. */
#define MCI_DRIVER_OS_VERSION 0x00000001u

/**
 *  \brief Callback for read done
 *  \param[in] dev Pointer to the MCI device instance.
 */
static void mci_os_read_done(struct _mci_async_device *dev)
{
	struct mci_os_desc *desc = CONTAINER_OF(dev, struct mci_os_desc, dev);

	_mci_async_set_irq_state(dev, MCI_ASYNC_READ_DONE, false);
	sem_up(&desc->xfer_sem);
}

/**
 *  \brief Callback for write done
 *  \param[in] dev Pointer to the MCI device instance.
 */
static void mci_os_write_done(struct _mci_async_device *dev)
{
	struct mci_os_desc *desc = CONTAINER_OF(dev, struct mci_os_desc, dev);

	_mci_async_set_irq_state(dev, MCI_ASYNC_WRITE_DONE, false);
	sem_up(&desc->xfer_sem);
}

/**
 *  \brief Callback for error
 *  \param[in] dev Pointer to the MCI device instance.
 */
static void mci_os_error(struct _mci_async_device *dev)
{
	struct mci_os_desc *desc = CONTAINER_OF(dev, struct mci_os_desc, dev);

	_mci_async_set_irq_state(&desc->dev, MCI_ASYNC_READ_DONE, false);
	_mci_async_set_irq_state(&desc->dev, MCI_ASYNC_WRITE_DONE, false);

	desc->error = ERR_ABORTED;

	sem_up(&desc->xfer_sem);
}

/**
 *  \brief Initialize MCI low level driver.
 */
int32_t mci_os_init(struct mci_os_desc *const desc, void *const hw)
{
	int32_t rc = 0;

	ASSERT(desc && hw);

	rc = _mci_async_init(&desc->dev, hw);
	if (rc == ERR_NONE) {
		_mci_async_register_callback(&desc->dev, MCI_ASYNC_READ_DONE, (FUNC_PTR)mci_os_read_done);
		_mci_async_register_callback(&desc->dev, MCI_ASYNC_WRITE_DONE, (FUNC_PTR)mci_os_write_done);
		_mci_async_register_callback(&desc->dev, MCI_ASYNC_ERROR, (FUNC_PTR)mci_os_error);
	} else {
		return rc;
	}

	rc = sem_init(&desc->xfer_sem, 0);
	if (rc < 0) {
		_mci_async_deinit(&desc->dev);
		return rc;
	}

	desc->error = ERR_NONE;

	return ERR_NONE;
}

/**
 *  \brief Deinitialize MCI low level driver.
 */
int32_t mci_os_deinit(struct mci_os_desc *const desc)
{
	ASSERT(desc);

	sem_deinit(&desc->xfer_sem);
	return _mci_async_deinit(&desc->dev);
}

/**
 *  \brief Select a device and initialize it
 */
int32_t mci_os_select_device(struct mci_os_desc *const desc, uint8_t slot, uint32_t clock, uint8_t bus_width,
                             bool high_speed)
{
	ASSERT(desc);
	return _mci_async_select_device(&desc->dev, slot, clock, bus_width, high_speed);
}

/**
 *  \brief Deselect a device by an assigned slot
 */
int32_t mci_os_deselect_device(struct mci_os_desc *const desc, uint8_t slot)
{
	ASSERT(desc);
	return _mci_async_deselect_device(&desc->dev, slot);
}

/**
 *  \brief Get the maximum bus width of a device
 */
uint8_t mci_os_get_bus_width(struct mci_os_desc *const desc, uint8_t slot)
{
	ASSERT(desc);
	return _mci_async_get_bus_width(&desc->dev, slot);
}

/**
 *  \brief Get the high speed capability of the device.
 */
bool mci_os_is_high_speed_capable(struct mci_os_desc *const desc)
{
	ASSERT(desc);
	return _mci_async_is_high_speed_capable(&desc->dev);
}

/**
 *  \brief Send 74 clock cycles on the line.
 */
void mci_os_send_init_sequence(struct mci_os_desc *const desc)
{
	ASSERT(desc);
	_mci_async_send_init_sequence(&desc->dev);
}

/**
 *  \brief Send a command on the selected slot
 */
bool mci_os_send_cmd(struct mci_os_desc *const desc, uint32_t cmd, uint32_t arg)
{
	ASSERT(desc);
	return _mci_async_send_cmd(&desc->dev, cmd, arg);
}

/**
 *  \brief Get 32 bits response of the last command.
 */
uint32_t mci_os_get_response(struct mci_os_desc *const desc)
{
	ASSERT(desc);
	return _mci_async_get_response(&desc->dev);
}

/**
 *  \brief Get 128 bits response of the last command.
 */
void mci_os_get_response_128(struct mci_os_desc *const desc, uint8_t *response)
{
	ASSERT(desc && response);
	_mci_async_get_response_128(&desc->dev, response);
}

/**
 *  \brief Send an ADTC command on the selected slot
 */
bool mci_os_adtc_start(struct mci_os_desc *const desc, uint32_t cmd, uint32_t arg, uint16_t block_size,
                       uint16_t nb_block, bool access_block)
{
	ASSERT(desc);

	desc->dev.block_size = block_size;
	return _mci_async_adtc_start(&desc->dev, cmd, arg, block_size, nb_block, access_block);
}

/**
 *  \brief Send a command to stop an ADTC command on the selected slot.
 */
bool mci_os_adtc_stop(struct mci_os_desc *const desc, uint32_t cmd, uint32_t arg)
{
	ASSERT(desc);
	return _mci_async_adtc_stop(&desc->dev, cmd, arg);
}

/**
 *  \brief Read a word on the line.
 */
bool mci_os_read_bytes(struct mci_os_desc *const desc, uint32_t *value)
{
	ASSERT(desc && value);
	return _mci_async_read_bytes(&desc->dev, value);
}

/**
 *  \brief Write a word on the line
 */
bool mci_os_write_bytes(struct mci_os_desc *const desc, uint32_t value)
{
	ASSERT(desc);
	return _mci_async_write_bytes(&desc->dev, value);
}

/**
 *  \brief Start a read blocks transfer on the line
 */
bool mci_os_start_read_blocks(struct mci_os_desc *const desc, void *dst, uint16_t nb_block)
{
	ASSERT(desc && dst);

	desc->error          = ERR_NONE;
	desc->dev.xfer_count = 0;
	desc->dev.xfer_buf   = (uint8_t *)dst;
	desc->dev.xfer_size  = nb_block * desc->dev.block_size;
	_mci_async_set_irq_state(&desc->dev, MCI_ASYNC_READ_DONE, true);

	if (0 != sem_down(&desc->xfer_sem, ~0)) {
		return false;
	}

	return (desc->error == ERR_NONE) ? true : false;
}

/**
 *  \brief Start a write blocks transfer on the line
 */
bool mci_os_start_write_blocks(struct mci_os_desc *const desc, const void *src, uint16_t nb_block)
{
	ASSERT(desc && src);

	desc->error          = ERR_NONE;
	desc->dev.xfer_count = 0;
	desc->dev.xfer_buf   = (uint8_t *)src;
	desc->dev.xfer_size  = nb_block * desc->dev.block_size;
	_mci_async_set_irq_state(&desc->dev, MCI_ASYNC_WRITE_DONE, true);

	if (0 != sem_down(&desc->xfer_sem, ~0)) {
		return false;
	}

	return (desc->error == ERR_NONE) ? true : false;
}

/**
 *  \brief Wait the end of transfer initiated by mci_start_read_blocks()
 */
bool mci_os_wait_end_of_read_blocks(struct mci_os_desc *const desc)
{
	ASSERT(desc);

	/* Always return true for os read blocks */
	return true;
}

/**
 *  \brief Wait the end of transfer initiated by mci_start_write_blocks()
 */
bool mci_os_wait_end_of_write_blocks(struct mci_os_desc *const desc)
{
	ASSERT(desc);

	/* Always return true for os read blocks */
	return true;
}

/**
 *  \brief Retrieve the current driver version
 */
uint32_t mci_os_get_version(void)
{
	return MCI_DRIVER_OS_VERSION;
}
