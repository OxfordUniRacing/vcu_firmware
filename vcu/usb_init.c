#include "atmel_start.h"
#include "usb_init.h"

/* Max LUN number */
#define CONF_USB_MSC_MAX_LUN 0

#if CONF_USBD_HS_SP
static uint8_t single_desc_bytes[] = {
	MSC_DEV_DESC, MSC_DEV_QUAL_DESC, MSC_CFG_DESC, MSC_IFACE_DESCES, MSC_OTH_SPD_CFG_DESC, MSC_IFACE_DESCES_HS, MSC_STR_DESCES};

static uint8_t single_desc_bytes_hs[] = {
	MSC_CFG_DESC, MSC_IFACE_DESCES_HS, MSC_OTH_SPD_CFG_DESC, MSC_IFACE_DESCES};
#else
static uint8_t single_desc_bytes[] = {
	MSC_DEV_DESC, MSC_CFG_DESC, MSC_IFACE_DESCES, MSC_STR_DESCES};
#endif

static struct usbd_descriptors single_desc[] = {
	{single_desc_bytes, single_desc_bytes + sizeof(single_desc_bytes)},
#if CONF_USBD_HS_SP
	{single_desc_bytes_hs, single_desc_bytes_hs + sizeof(single_desc_bytes_hs)},
#endif
};

/* Ctrl endpoint buffer */
static uint8_t ctrl_buffer[64] __attribute__ ((aligned (4)));

void usb_start(void) {
	usbdc_start(single_desc);
	usbdc_attach();
}

void usb_init(void) {
	/* usb stack init */
	usbdc_init(ctrl_buffer);

	/* usbdc_register_funcion inside */
	mscdf_init(CONF_USB_MSC_MAX_LUN);
}
