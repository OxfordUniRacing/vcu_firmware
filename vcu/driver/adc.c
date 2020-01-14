#include <stdint.h>

#include "atmel_start.h"

#include "driver_init.h"
#include "hpl_adc_async.h"

#include "adc.h"


static struct adc_async_descriptor * const descrs[2] =
	{ &ADC_0, &ADC_1 };
static struct _adc_async_device * const devices[2] =
	{ &ADC_0.device, &ADC_1.device };
static void * const hws[2] =
	{ AFEC0, AFEC1 };
static SemaphoreHandle_t sems[2];


static void adc_done_cb(struct _adc_async_device *device,
		uint8_t channel, uint16_t data) {
	(void)data;
	BaseType_t xHigherPriorityTaskWoken;

	int dev = -1;
	if (device == devices[0]) dev = 0;
	if (device == devices[1]) dev = 1;
	ASSERT(dev >= 0);

	if (channel == descrs[dev]->channel_max) {
		xSemaphoreGiveFromISR(sems[dev], &xHigherPriorityTaskWoken);
	}

	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

/*
static void adc_error_cb(struct _adc_async_device *device, uint8_t channel) {
	(void)device;
	(void)channel;
}
*/

void adc_init(int dev, SemaphoreHandle_t done_sem,
		const uint8_t channel_nums[], uint8_t channel_amount) {
	ASSERT(((unsigned int)dev) < 2);

	sems[dev] = done_sem;

	struct adc_async_descriptor *descr = descrs[dev];

	_adc_async_init(devices[dev], hws[dev]);

	descr->channel_amount = channel_amount;
	descr->channel_max = 0;

	devices[dev]->adc_async_ch_cb.convert_done = adc_done_cb;	
	devices[dev]->adc_async_cb.window_cb = NULL;
	devices[dev]->adc_async_cb.error_cb = NULL;

	for (int i = 0; i < channel_amount; i++) {
		if (channel_nums[i] > descr->channel_max) {
			descr->channel_max = channel_nums[i];
		}

		_adc_async_enable_channel(devices[dev], channel_nums[i]);
		_adc_async_set_irq_state(devices[dev], channel_nums[i],
				ADC_ASYNC_DEVICE_CONVERT_CB, true);
	}
}

void adc_start(int dev) {
	ASSERT(((unsigned int)dev) < 2);

	_adc_async_convert(devices[dev]);
}

uint16_t adc_read_channel_data(int dev, uint8_t channel) {
	ASSERT(((unsigned int)dev) < 2);

	return _adc_async_read_channel_data(devices[dev], channel);
}
