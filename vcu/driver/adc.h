#ifndef DRIVER_ADC_H
#define DRIVER_ADC_H

void adc_init(int dev, SemaphoreHandle_t done_sem,
		const uint8_t channel_nums[], uint8_t channel_amount);

void adc_start(int dev);

uint16_t adc_read_channel_data(int dev, uint8_t channel);

#endif
