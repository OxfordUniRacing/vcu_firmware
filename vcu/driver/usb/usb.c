#include "atmel_start.h"

#include "usb.h"
#include "usb_desc.h"

/* Max LUN number */
#define CONF_USB_MSC_MAX_LUN 0

/* Ctrl endpoint buffer */
static uint8_t ctrl_buffer[64] __attribute__ ((aligned (4)));

void usb_init(void) {
	/* usb stack init */
	usbdc_init(ctrl_buffer);
	usbdc_start(single_desc);

	// Register cdc acm before msc because msc init routines are broken
	cdcdf_acm_init();
	mscdf_init(CONF_USB_MSC_MAX_LUN);
}

void usb_start(void) {
	usbdc_attach();
}
