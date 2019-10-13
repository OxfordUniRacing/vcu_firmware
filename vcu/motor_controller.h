#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

struct mc_inst_t;
void mc_init(void);

struct mc_inst_t *mc_get_inst(unsigned int i);

int mc_uart_read(struct mc_inst_t *mc, uint8_t *dst, int size);
void mc_uart_write(struct mc_inst_t *mc, uint8_t *src, int count);

void mc_passthru_enable(struct mc_inst_t *mc);
void mc_passthru_disable(struct mc_inst_t *mc);

#endif
