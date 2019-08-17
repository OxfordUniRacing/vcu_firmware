#ifndef USB_DRIVE_H
#define USB_DRIVE_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "FreeRTOS.h"

extern TaskHandle_t usb_drive_msc_task_h;
extern TaskHandle_t usb_drive_sdmmc_task_h;

void usb_drive_init(void);
void usb_drive_msc_task(void *p);
void usb_drive_sdmmc_task(void *p);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // USB_DRIVE_H
