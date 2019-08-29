#include "utils_assert.h"
#include "cmsis_gcc.h"
#include "hri_usart_e70b.h"
#include <stdio.h>

static void write(const char *s) {
	while (*s) {
		while (!hri_usart_get_US_CSR_TXRDY_bit(USART1));
		hri_usart_write_US_THR_reg(USART1, (uint32_t) *s++);
	}
}

void assert_failed(const char *const file, const int line) {
	char buf[16];

	__disable_irq();

	write("\n");
	write("assertion failed: ");
	write(file);
	write(":");
	snprintf(buf, sizeof(buf), "%d", line);
	write(buf);
	write("\n");

	while (1) __BKPT(0);
}
