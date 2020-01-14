#include <stdint.h>

#include "utils_assert.h"
#include "FreeRTOS.h"
#include "task.h"

#include "sensor.h"
#include "sensor_db.h"
#include "framework/param/param.h"
#include "framework/param/param_db.h"

#define BOUNDS_CHECK(i) ASSERT(((unsigned int)i) < SENSOR_MAX_NUM)

sensor_state_t sensor_states[SENSOR_MAX_NUM];
uint32_t sensor_timestamps[SENSOR_MAX_NUM];

float sensor_get_f(int ind) {
	BOUNDS_CHECK(ind);
	return sensor_states[ind].f;
}

uint32_t sensor_get_ts(int ind) {
	BOUNDS_CHECK(ind);
	return sensor_timestamps[ind];
}

void sensor_update_b(int ind, bool val) {
	BOUNDS_CHECK(ind);
	sensor_states[ind].b = val;
	sensor_timestamps[ind] = xTaskGetTickCount();
}

void sensor_update_raw(int ind, int val) {
	BOUNDS_CHECK(ind);
	ASSERT(sensor_trans_fns[ind] != NULL);
	sensor_states[ind].f = sensor_trans_fns[ind](ind, val);
	sensor_timestamps[ind] = xTaskGetTickCount();
}


float sensor_trans_linear(int ind, int val) {
	int param_addr = PARAM_CALIB_START + ind*8;
	float m = param_read_float(param_addr);
	float c = param_read_float(param_addr+4);
	return m*val + c;
}
