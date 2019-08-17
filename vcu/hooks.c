#include "atmel_start.h"

void vApplicationStackOverflowHook( TaskHandle_t xTask,
                                    signed char *pcTaskName ) {
	while (1) __BKPT(0);
}

void vApplicationMallocFailedHook( void ) {
	while (1) __BKPT(0);
}
