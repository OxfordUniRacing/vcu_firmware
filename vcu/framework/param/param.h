#ifndef APP_PARAM_H
#define APP_PARAM_H

float param_read_float(int addr);

void param_write_float(int addr, float f);

int param_find_by_name(char *name);

#endif
