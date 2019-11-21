#include "atmel_start.h"

#include "eeprom/eeprom_emu.h"
#include "log_msg.h"

#include "FreeRTOS.h"
#include "task.h"

static void test_task(void *unused) {
	(void) unused;
	
	log_info("eeprom emulation test");

	uint32_t x;
	const uint32_t addr1 = 0x1337;
	const uint32_t addr2 = 0x1333;
	const uint32_t val1 = 0xDEADBEEF;
	const uint32_t val2 = 0xCAFEBABE;

	flash_erase(&FLASH, 0x1fc000, 32);
	eeprom_emu_init();

	//eeprom_emu_read(addr1, &x, 4);
	//log_info("current value: 0x%x", x);
	//vTaskDelay(100);

	eeprom_emu_write(addr1, &val1, 4);
	x = 0; eeprom_emu_read(addr1, &x, 4);
	ASSERT(x == val1);

	eeprom_emu_commit();
	eeprom_emu_init();
	x = 0; eeprom_emu_read(addr1, &x, 4);
	ASSERT(x == val1);

	eeprom_emu_write(addr2, &val2, 4);
	x = 0; eeprom_emu_read(addr2, &x, 4);
	ASSERT(x == val2);
	x = 0; eeprom_emu_read(addr1, &x, 4);
	ASSERT(x == val1);

	eeprom_emu_commit();
	eeprom_emu_init();
	x = 0; eeprom_emu_read(addr2, &x, 4);
	ASSERT(x == val2);
	x = 0; eeprom_emu_read(addr1, &x, 4);
	ASSERT(x == val1);

	log_info("test passed");
	vTaskDelay(portMAX_DELAY);
}

int main(void) {
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	log_init();
	xTaskCreate(test_task, "", 1024, NULL, tskIDLE_PRIORITY + 2, NULL);

	vTaskStartScheduler();
}
