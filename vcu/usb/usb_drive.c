/*
 * Expose SD card as a USB mass storage device
 */

#include "atmel_start.h"
#include "sd_mmc.h"
#include "usb_drive.h"

static struct scsi_inquiry_data inquiry_info = {
	SCSI_INQ_DT_DIR_ACCESS,
	SCSI_INQ_RMB,
	0,
	SCSI_INQ_RSP_SPC2,
	31,
	0,
	0,
	0,
	"ATMEL",
	"SAM E70 SD CARD",
	"0.1",
};
static struct sbc_read_capacity10_data capacity_info = {
	0x00000000, /* length to be filled */
	0x00020000, /* 512 byte block size */
};

#define BUFFER_SIZE 64
#define BLOCK_SIZE 512
static uint8_t block_buf[BUFFER_SIZE * BLOCK_SIZE] __attribute__ ((aligned (4)));

static uint32_t max_lba; // sd card max lba

static enum { DIR_WRITE, DIR_READ } trans_dir;
static uint32_t trans_addr;
static uint32_t trans_nblocks;

static SemaphoreHandle_t buf_free_sem;
static SemaphoreHandle_t buf_filled_sem;
static SemaphoreHandle_t msc_cmd_sem;
//static SemaphoreHandle_t msc_busy_sem;
static SemaphoreHandle_t sdmmc_done_sem;

TaskHandle_t usb_drive_msc_task_h;
TaskHandle_t usb_drive_sdmmc_task_h;

/**
 * \brief Eject Disk
 * \param[in] lun logic unit number
 * \return Operation status.
 */
static int32_t msc_disk_eject(uint8_t lun) {
	if (lun != 0) {
		return ERR_NOT_FOUND;
	} else {
		return ERR_NONE;
	}
}

/**
 * \brief Inquiry whether Disk is ready
 * \param[in] lun logic unit number
 * \return Operation status.
 */
static int32_t msc_disk_is_ready(uint8_t lun) {
	if (lun != 0) {
		return ERR_NOT_FOUND;
	} else {
		return ERR_NONE;
	}
}

/**
 * \brief Callback invoked when inquiry data command received
 * \param[in] lun logic unit number
 * \return Operation status.
 */
static uint8_t *msc_inquiry_info(uint8_t lun) {
	if (lun > 0) {
		return NULL;
	} else {
		return (uint8_t *)&inquiry_info;
	}
}

/**
 * \brief Callback invoked when read format capacities command received
 * \param[in] lun logic unit number
 * \return Operation status.
 */
static uint8_t *msc_get_capacity(uint8_t lun) {
	if (lun > 0) {
		return NULL;
	} else {
		return (uint8_t *)&capacity_info;
	}
}

/**
 * \brief Callback invoked when a new read blocks command received
 * \param[in] lun logic unit number
 * \param[in] addr start address of disk to be read
 * \param[in] nblocks block amount to be read
 * \return Operation status.
 */
static int32_t msc_new_read(uint8_t lun, uint32_t addr, uint32_t nblocks) {

	uint32_t rc = msc_disk_is_ready(lun);
	if (rc != ERR_NONE) {
		return rc;
	}
	if (addr > max_lba || addr + nblocks > max_lba + 1) {
		return ERR_BAD_ADDRESS;
	}

	trans_dir = DIR_READ;
	trans_addr = addr;
	trans_nblocks = nblocks;

	BaseType_t woken;
	rc = xSemaphoreGiveFromISR(msc_cmd_sem, &woken);

	portYIELD_FROM_ISR(woken);
	return ERR_NONE;
}

/**
 * \brief Callback invoked when a new write blocks command received
 * \param[in] lun logic unit number
 * \param[in] addr start address of disk to be written
 * \param[in] nblocks block amount to be written
 * \return Operation status.
 */
static int32_t msc_new_write(uint8_t lun, uint32_t addr, uint32_t nblocks) {
	uint32_t rc = msc_disk_is_ready(lun);
	if (rc != ERR_NONE) {
		return rc;
	}
	if (addr > max_lba || addr + nblocks > max_lba + 1) {
		return ERR_BAD_ADDRESS;
	}

	trans_dir = DIR_WRITE;
	trans_addr = addr;
	trans_nblocks = nblocks;

	BaseType_t woken;
	rc = xSemaphoreGiveFromISR(msc_cmd_sem, &woken);

	portYIELD_FROM_ISR(woken);
	return ERR_NONE;
}

/**
 * \brief Callback invoked when a blocks transfer is done
 * \param[in] lun logic unit number
 * \return Operation status.
 */
static int32_t msc_xfer_done(uint8_t lun) {
	if (lun != 0) {
		return ERR_NOT_FOUND;
	}

	BaseType_t woken;
	vTaskNotifyGiveFromISR(usb_drive_msc_task_h, &woken);
	//xSemaphoreGiveFromISR(msc_busy_sem, &woken);

	portYIELD_FROM_ISR(woken);
	return ERR_NONE;
}

void usb_drive_init(void) {
	buf_free_sem = xSemaphoreCreateCounting(BUFFER_SIZE, BUFFER_SIZE);
	buf_filled_sem = xSemaphoreCreateCounting(BUFFER_SIZE, 0);
	msc_cmd_sem = xSemaphoreCreateBinary();
	//msc_busy_sem = xSemaphoreCreateBinary();
	sdmmc_done_sem = xSemaphoreCreateBinary();

	xTaskCreate(usb_drive_msc_task, "", 1024, NULL, 1, &usb_drive_msc_task_h);
	xTaskCreate(usb_drive_sdmmc_task, "", 1024, NULL, 1, &usb_drive_sdmmc_task_h);
}

void usb_drive_msc_task(void *p) {
	(void)p;
	uint32_t rc;
	uint32_t buf_ind;

	// wait until sd card is ready
	ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

	mscdf_register_callback(MSCDF_CB_INQUIRY_DISK, (FUNC_PTR)msc_inquiry_info);
	mscdf_register_callback(MSCDF_CB_GET_DISK_CAPACITY, (FUNC_PTR)msc_get_capacity);
	mscdf_register_callback(MSCDF_CB_START_READ_DISK, (FUNC_PTR)msc_new_read);
	mscdf_register_callback(MSCDF_CB_START_WRITE_DISK, (FUNC_PTR)msc_new_write);
	mscdf_register_callback(MSCDF_CB_XFER_BLOCKS_DONE, (FUNC_PTR)msc_xfer_done);
	mscdf_register_callback(MSCDF_CB_EJECT_DISK, (FUNC_PTR)msc_disk_eject);
	mscdf_register_callback(MSCDF_CB_TEST_DISK_READY, (FUNC_PTR)msc_disk_is_ready);

	// wait until usb is ready
	//while (!mscdf_is_enabled()) {
	//	vTaskDelay(1);
	//}

	buf_ind = 0;

	while (1) {
		xSemaphoreTake(msc_cmd_sem, portMAX_DELAY);

		// notify that a command is received
		xTaskNotifyGive(usb_drive_sdmmc_task_h);

		if (trans_dir == DIR_WRITE) {
			/* Write Request */
			for (uint32_t i = 0; i < trans_nblocks; i++) {
				xSemaphoreTake(buf_free_sem, portMAX_DELAY);

				rc = mscdf_xfer_blocks(false, block_buf + buf_ind * BLOCK_SIZE, 1);
				ASSERT(rc == ERR_NONE);
				ulTaskNotifyTake(pdFALSE, portMAX_DELAY);
				//xSemaphoreTake(msc_busy_sem, portMAX_DELAY);

				buf_ind = (buf_ind + 1) % BUFFER_SIZE;

				rc = xSemaphoreGive(buf_filled_sem);
				ASSERT(rc == pdTRUE);
			}

			xSemaphoreTake(sdmmc_done_sem, portMAX_DELAY);

			// terminate write and send csw
			mscdf_xfer_blocks(false, NULL, 0);
		} else {
			/* Read Request */
			for (uint32_t i = 0; i < trans_nblocks; i++) {
				xSemaphoreTake(buf_filled_sem, portMAX_DELAY);

				rc = mscdf_xfer_blocks(true, block_buf + buf_ind * BLOCK_SIZE, 1);
				ASSERT(rc == ERR_NONE);
				ulTaskNotifyTake(pdFALSE, portMAX_DELAY);
				//xSemaphoreTake(msc_busy_sem, portMAX_DELAY);

				buf_ind = (buf_ind + 1) % BUFFER_SIZE;

				rc = xSemaphoreGive(buf_free_sem);
				ASSERT(rc == pdTRUE);
			}
		}
	}
}

void usb_drive_sdmmc_task(void *p) {
	(void)p;
	uint32_t rc;
	uint32_t buf_ind;

	// wait until sd card is ready
	while (sd_mmc_check(0) != SD_MMC_OK) {
		vTaskDelay(1);
	}

	max_lba = sd_mmc_get_capacity(0) * 2 - 1;
	capacity_info.max_lba = __builtin_bswap32(max_lba);

	// notify that sd card is ready
	xTaskNotifyGive(usb_drive_msc_task_h);

	buf_ind = 0;

	while (1) {
		// wait until a msc command is issued
		ulTaskNotifyTake(pdFALSE, portMAX_DELAY);

		if (trans_dir == DIR_WRITE) {
			/* Write Disk */
			rc = sd_mmc_init_write_blocks(0, trans_addr, trans_nblocks);
			ASSERT(rc == SD_MMC_OK);

			for (uint32_t i = 0; i < trans_nblocks; i++) {
				xSemaphoreTake(buf_filled_sem, portMAX_DELAY);

				rc = sd_mmc_start_write_blocks(block_buf + buf_ind * BLOCK_SIZE, 1);
				ASSERT(rc == SD_MMC_OK);
				rc = sd_mmc_wait_end_of_write_blocks(false);
				ASSERT(rc == SD_MMC_OK);

				buf_ind = (buf_ind + 1) % BUFFER_SIZE;

				rc = xSemaphoreGive(buf_free_sem);
				ASSERT(rc == pdTRUE);
			}

			// Workaround for driver bug
			// We need to wait until XFRDONE is set before sending another command
			while (!(hri_hsmci_read_SR_reg(HSMCI) & HSMCI_SR_XFRDONE)) {
				taskYIELD();
			}

			// notify that write completed
			xSemaphoreGive(sdmmc_done_sem);
		} else {
			/* Read Disk */
			rc = sd_mmc_init_read_blocks(0, trans_addr, trans_nblocks);
			ASSERT(rc == SD_MMC_OK);

			for (uint32_t i = 0; i < trans_nblocks; i++) {
				xSemaphoreTake(buf_free_sem, portMAX_DELAY);

				rc = sd_mmc_start_read_blocks(block_buf + buf_ind * BLOCK_SIZE, 1);
				ASSERT(rc == SD_MMC_OK);
				rc = sd_mmc_wait_end_of_read_blocks(false);
				ASSERT(rc == SD_MMC_OK);

				buf_ind = (buf_ind + 1) % BUFFER_SIZE;

				rc = xSemaphoreGive(buf_filled_sem);
				ASSERT(rc == pdTRUE);
			}
		}
	}
}
