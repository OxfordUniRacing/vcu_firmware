#ifndef EEPROM_EMU_H
#define EEPROM_EMU_H

#include <stdint.h>

void eeprom_emu_init(void);
void eeprom_emu_read(uint32_t src_addr, uint8_t *p, uint32_t length);
uint32_t eeprom_emu_read32(uint32_t src_addr);
void eeprom_emu_write(uint32_t dest_addr, const uint8_t *p, uint32_t length);
void eeprom_emu_write32(uint32_t src_addr, uint32_t d);
void eeprom_emu_commit(void);

#endif
