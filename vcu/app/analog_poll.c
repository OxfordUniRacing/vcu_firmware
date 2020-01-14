#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "timers.h"

#include "config.h"
#include "driver/adc.h"
#include "framework/sensor/sensor.h"
#include "framework/sensor/sensor_db.h"
#include "analog_poll.h"


static TaskHandle_t task_handle;

static const uint8_t adc0_chns[] = {
	0, 8, 10, 11
};
static const int adc0_sens[] = {
	SENS_PRES_R_OUT, SENS_PRES_R_IN, SENS_PRES_L_IN, SENS_VCU_MCU_TEMP
};
static const uint8_t adc1_chns[] = {
	0, 1, 3, 5, 6
};
static const int adc1_sens[] = {
	SENS_PRES_L_OUT, SENS_TEMP_R_OUT, SENS_TEMP_L_IN, SENS_TEMP_L_OUT, SENS_TEMP_R_IN
};

static void analog_poll_timer_cb(TimerHandle_t timer) {
	(void)timer;

	xTaskNotifyGive(task_handle);
}

static void analog_poll_task(void *p) {
	(void)p;

	SemaphoreHandle_t sem_0 = xSemaphoreCreateBinary();
	SemaphoreHandle_t sem_1 = xSemaphoreCreateBinary();

	adc_init(0, sem_0, adc0_chns, sizeof(adc0_chns));
	adc_init(1, sem_1, adc1_chns, sizeof(adc1_chns));

	while (1) {
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

		adc_start(0);
		adc_start(1);

		xSemaphoreTake(sem_0, portMAX_DELAY);
		for (unsigned int i = 0; i < sizeof(adc0_chns); i++) {
			uint16_t x = adc_read_channel_data(0, adc0_chns[i]);
			sensor_update_raw(adc0_sens[i], x);
		}

		xSemaphoreTake(sem_1, portMAX_DELAY);
		for (unsigned int i = 0; i < sizeof(adc1_chns); i++) {
			uint16_t x = adc_read_channel_data(1, adc1_chns[i]);
			sensor_update_raw(adc1_sens[i], x);
		}
	}
}

void analog_poll_init(void) {
	xTaskCreate(analog_poll_task, "analog_p", 256, NULL, 2, &task_handle);

	TimerHandle_t timer = xTimerCreate(
			"analog_p", ANALOG_POLL_PERIOD, pdTRUE, NULL,
			analog_poll_timer_cb);

	xTimerStart(timer, portMAX_DELAY);
}
