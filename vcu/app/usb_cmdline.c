#include "atmel_start.h"
#include "FreeRTOS.h"
#include "task.h"

#include <string.h>

#include "driver/log_msg.h"
#include "driver/usb/usb_console.h"
#include "driver/eeprom_emu.h"
#include "framework/param/param.h"
#include "app/motor_controller.h"

#define RET_CHECK(x) if (x < 0) return x;

static char *stok; // strtok state

static int cmd_ping(void) {
	int r;
	r = cprintf("pong\n");
	return r;
}

static int cmd_echo(void) {
	int r;
	char *s = strtok_r(NULL, "", &stok);
	r = cprintf("%s\n", s);
	return r;
}

#define CMD_MC_HELP "mc <num> <command>"
static int cmd_mc(void) {
	int r;
	struct mc_inst_t *mc = mc_get_inst(1);

	//char *inst = strtok_r(NULL, " ", &stok);
	//if (!inst) return 0;

	char *subcmd = strtok_r(NULL, " ", &stok);
	if (!subcmd) return 0;
	else if (!strcmp(subcmd, "passthru")) {
		static uint8_t buf[64];

		mc_passthru_enable(mc);
		while (1) {
			if ((r = mc_uart_read(mc, buf, sizeof(buf)))) {
				cwrite(buf, r);
			} else if ((r = cgetc_async())) {
				RET_CHECK(r);
				if (r == 0x1b) { // esc
					cwrite("\n", 1);
					break;
				} else {
					cwrite(&r, 1);
					mc_uart_write(mc, (uint8_t *)&r, 1);
				}
			} else {
				vTaskDelay(1);
			}
		}
		mc_passthru_disable(mc);
	}

	return 0;
}

static int cmd_param(void) {
	int r;

	char *subcmd = strtok_r(NULL, " ", &stok);
	if (!subcmd) return 0;

	if (!strcmp(subcmd, "commit")) {
		eeprom_emu_commit();
		return 0;
	}

	char *name = strtok_r(NULL, " ", &stok);
	if (!name) return 0;

	int addr = param_find_by_name(name);
	if (addr < 0) {
		r = cprintf("Unknown param: %s\n", name);
		return r;
	}

	if (!strcmp(subcmd, "getf")) {
		float x = param_read_float(addr);
		r = cprintf("%s = %f\n", name, x);
		return r;
	} else if (!strcmp(subcmd, "setf")) {
		char *val = strtok_r(NULL, " ", &stok);
		if (!val) return cprintf("error\n");
		float x;
		r = sscanf(val, "%f", &x);
		if (r < 1) return cprintf("error\n");
		param_write_float(addr, x);
		r = cprintf("%s = %f\n", name, x);
		return r;
	}

	return 0;
}

int cmdline_main(void) {
	static char line_buf[256];
	int r;

	cgetc();

	while (1) {
		r = cwrite("> ", 3);
		RET_CHECK(r);

		r = creadline(line_buf, sizeof(line_buf));
		RET_CHECK(r);

		char *cmd = strtok_r(line_buf, " ", &stok);

		if (!cmd) {
		} else if (!strcmp(cmd, "ping")) {
			r = cmd_ping();
			RET_CHECK(r);
		} else if (!strcmp(cmd, "echo")) {
			r = cmd_echo();
			RET_CHECK(r);
		} else if (!strcmp(cmd, "mc")) {
			r = cmd_mc();
			RET_CHECK(r);
		} else if (!strcmp(cmd, "param")) {
			r = cmd_param();
			RET_CHECK(r);
		} else {
			r = cprintf("unrecognized command: %s\n", cmd);
			RET_CHECK(r);
		}
	}
}
