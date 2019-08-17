/*
 * Override memcpy to work around bug caused by unaligned access to usb sram
 *
 * Thanks to atmel for their excellent quality drivers
 *
 * Hours wasted: 8
 */

#include <stddef.h>

static inline void *memcpy_usb(void *dest, const void *src, size_t len) {
	while (len) {
		*(char *)dest = *(char *)src;
		dest++;
		src++;
		len--;
	}
	return NULL;
}

#define memcpy memcpy_usb
