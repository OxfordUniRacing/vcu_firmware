#ifndef UART_TERM_H
#define UART_TERM_H

struct term_inst_t;
void term_init(void);

int term_uart_read(struct term_inst_t *term, uint8_t *dst, int size);
void term_uart_write(struct term_inst_t *term, uint8_t *src, int count);

#endif
