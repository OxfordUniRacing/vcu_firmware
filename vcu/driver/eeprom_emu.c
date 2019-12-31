#include "eeprom_emu.h"
#include "atmel_start.h"

#define BLOCK_MAGIC 0x6d703265
#define FLASH_BLOCK_SIZE 8192
#define BLOCK_SIZE (FLASH_BLOCK_SIZE-4)

struct block_t {
	uint8_t d[BLOCK_SIZE];
	uint32_t magic;
};
static struct block_t flash_blocks[2] __attribute__ ((section (".eeprom")));

static struct block_t buffer;
static int cur_block = -1;

void eeprom_emu_init() {
	if (flash_blocks[0].magic == BLOCK_MAGIC) {
		// block 0 is valid
		memcpy(&buffer, &flash_blocks[0], FLASH_BLOCK_SIZE);
		cur_block = 0;

	} else if (flash_blocks[1].magic == BLOCK_MAGIC) {
		// block 1 is valid
		memcpy(&buffer, &flash_blocks[1], FLASH_BLOCK_SIZE);
		cur_block = 1;

	} else {
		// flash is uninitialized
		// erase block 1 and write magic to block 0
		flash_erase(&FLASH, ((uint32_t) &flash_blocks[1]) - IFLASH_ADDR, 16);
		buffer.magic = BLOCK_MAGIC;
		cur_block = 1;
		eeprom_emu_commit();
	}
}

void eeprom_emu_read(uint32_t src_addr, uint8_t *p, uint32_t length) {
	ASSERT(cur_block >= 0);
	ASSERT(src_addr + length <= BLOCK_SIZE);

	memcpy(p, buffer.d + src_addr, length);
}

uint32_t eeprom_emu_read32(uint32_t src_addr) {
	ASSERT(cur_block >= 0);
	ASSERT(src_addr+4 <= BLOCK_SIZE);

	return *((uint32_t *)(buffer.d + src_addr));
}

void eeprom_emu_write(uint32_t dest_addr, const uint8_t *p, uint32_t length) {
	ASSERT(cur_block >= 0);
	ASSERT(dest_addr + length <= BLOCK_SIZE);

	memcpy(buffer.d + dest_addr, p, length);
}

void eeprom_emu_write32(uint32_t src_addr, uint32_t d) {
	ASSERT(cur_block >= 0);
	ASSERT(src_addr+4 <= BLOCK_SIZE);

	*((uint32_t *)(buffer.d + src_addr)) = d;
}

void eeprom_emu_commit() {
	ASSERT(cur_block >= 0);

	int next_block = cur_block^1;
	// next block should already be erased
	flash_write(&FLASH, ((uint32_t) &flash_blocks[next_block]) - IFLASH_ADDR,
			(uint8_t *) &buffer, FLASH_BLOCK_SIZE);
	flash_erase(&FLASH, ((uint32_t) &flash_blocks[cur_block]) - IFLASH_ADDR, 16);
	cur_block = next_block;
}
