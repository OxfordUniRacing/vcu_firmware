#include "atmel_start.h"

void vApplicationStackOverflowHook( TaskHandle_t xTask,
                                    signed char *pcTaskName ) {
	ASSERT(false);
	while (1) __BKPT(0);
}

void vApplicationMallocFailedHook( void ) {
	ASSERT(false);
	while (1) __BKPT(0);
}
