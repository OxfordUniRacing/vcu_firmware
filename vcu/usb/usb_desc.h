#include "atmel_start.h"

#define USB_CFG_DESC \
	USB_CONFIG_DESC_BYTES(98, 3, 0x01, CONF_USB_MSC_ICONFIG, CONF_USB_MSC_BMATTRI, CONF_USB_MSC_BMAXPOWER)

#define USB_OTH_SPD_CFG_DESC \
	USB_OTH_SPD_CFG_DESC_BYTES(98, 3, 0x01, CONF_USB_MSC_ICONFIG, CONF_USB_MSC_BMATTRI, CONF_USB_MSC_BMAXPOWER)

#define CDCD_ACM_IAD_DESC \
	USB_IAD_DESC_BYTES(CONF_USB_CDCD_ACM_COMM_BIFCNUM, 2, 0x2, 0x2, 0x0, CONF_USB_CDCD_ACM_COMM_IIFC)

// Fix broken library definition
#undef CDCD_ACM_COMM_IFACE_DESCES
#define CDCD_ACM_COMM_IFACE_DESCES                                                                                     \
	USB_IFACE_DESC_BYTES(CONF_USB_CDCD_ACM_COMM_BIFCNUM,                                                               \
	                     CONF_USB_CDCD_ACM_COMM_BALTSET,                                                               \
	                     1,                                                                                            \
	                     0x2,                                                                                          \
	                     0x2,                                                                                          \
	                     0x0,                                                                                          \
	                     CONF_USB_CDCD_ACM_COMM_IIFC),                                                                 \
	    USB_CDC_HDR_DESC_BYTES(0x1001), USB_CDC_CALL_MGMT_DESC_BYTES(0x01, 0x00), USB_CDC_ACM_DESC_BYTES(0x02),        \
	    USB_CDC_UNION_DESC_BYTES(CONF_USB_CDCD_ACM_COMM_BIFCNUM, CONF_USB_CDCD_ACM_DATA_BIFCNUM),                      \
	    USB_ENDP_DESC_BYTES(CONF_USB_CDCD_ACM_COMM_INT_EPADDR,                                                         \
	                        3,                                                                                         \
	                        CONF_USB_CDCD_ACM_COMM_INT_MAXPKSZ,                                                        \
	                        CONF_USB_CDCD_ACM_COMM_INT_INTERVAL)


#if CONF_USBD_HS_SP
static uint8_t single_desc_bytes[] = {
	MSC_DEV_DESC, MSC_DEV_QUAL_DESC,
	USB_CFG_DESC,
		MSC_IFACE_DESCES,
		CDCD_ACM_IAD_DESC, CDCD_ACM_COMM_IFACE_DESCES, CDCD_ACM_DATA_IFACE_DESCES,
	USB_OTH_SPD_CFG_DESC,
		MSC_IFACE_DESCES_HS,
		CDCD_ACM_IAD_DESC, CDCD_ACM_COMM_IFACE_DESCES, CDCD_ACM_DATA_IFACE_DESCES_HS,
	MSC_STR_DESCES};

static uint8_t single_desc_bytes_hs[] = {
	USB_CFG_DESC,
		MSC_IFACE_DESCES_HS,
		CDCD_ACM_IAD_DESC, CDCD_ACM_COMM_IFACE_DESCES, CDCD_ACM_DATA_IFACE_DESCES_HS,
	USB_OTH_SPD_CFG_DESC,
		MSC_IFACE_DESCES,
		CDCD_ACM_IAD_DESC, CDCD_ACM_COMM_IFACE_DESCES, CDCD_ACM_DATA_IFACE_DESCES};
#else
static uint8_t single_desc_bytes[] = {
	MSC_DEV_DESC,
	USB_CFG_DESC,
		MSC_IFACE_DESCES,
		CDCD_ACM_IAD_DESC, CDCD_ACM_COMM_IFACE_DESCES, CDCD_ACM_DATA_IFACE_DESCES,
	MSC_STR_DESCES};
#endif

static struct usbd_descriptors single_desc[] = {
	{single_desc_bytes, single_desc_bytes + sizeof(single_desc_bytes)},
#if CONF_USBD_HS_SP
	{single_desc_bytes_hs, single_desc_bytes_hs + sizeof(single_desc_bytes_hs)},
#endif
};
