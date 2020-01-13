#ifndef FRAMEWORK_SENSOR_SENSOR_H
#define FRAMEWORK_SENSOR_SENSOR_H

#include <stdint.h>
#include <stdbool.h>

enum { SENSOR_TYPE_FLOAT, SENSOR_TYPE_BOOL };

typedef float (*sensor_trans_fn)(int, int);

extern char *sensor_names[];
extern sensor_trans_fn sensor_trans_fns[];

typedef union {
	float f;
	int i;
	bool b;
} sensor_state_t;

float sensor_get_f(int ind);
uint32_t sensor_get_ts(int ind);
void sensor_update_b(int ind, bool val);
void sensor_update_raw(int ind, int val);

float sensor_trans_linear(int ind, int val);

#endif
