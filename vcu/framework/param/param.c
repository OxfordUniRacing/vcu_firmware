#include <stdint.h>
#include <string.h>

#include "param.h"
#include "param_db.h"
#include "driver/eeprom_emu.h"

float param_read_float(int addr) {
	uint32_t d = eeprom_emu_read32((uint32_t)addr);
	return *((float *)&d);
}

int param_find_by_name(char *name) {
	for (int i = 0; i < PARAM_INFO_COUNT; i++) {
		if (!strcmp(name, param_info[i].name)) {
			return param_info[i].offset;
		}
	}
	return -1;
}
