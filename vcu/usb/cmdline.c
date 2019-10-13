#include "atmel_start.h"
#include "FreeRTOS.h"
#include "task.h"

#include <string.h>

#include "log/msglog.h"
#include "motor_controller.h"
#include "usb/usb_console.h"

#define guard(x) if (x < 0) return x;

static char *stok; // strtok state

static int cmd_ping(void) {
	int r;
	r = cwrite("pong\n", 5);
	guard(r);
	return 0;
}

static int cmd_echo(void) {
	int r;
	char *s = strtok_r(NULL, "", &stok);
	r = cprintf("%s\n", s);
	guard(r);
	return 0;
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
				guard(r);
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

int cmdline_main(void) {
	static char line_buf[256];
	int r;

	cgetc();

	while (1) {
		r = cwrite("> ", 3);
		guard(r);

		r = creadline(line_buf, sizeof(line_buf));
		guard(r);

		char *cmd = strtok_r(line_buf, " ", &stok);

		if (!cmd) {
		} else if (!strcmp(cmd, "ping")) {
			r = cmd_ping();
			guard(r);
		} else if (!strcmp(cmd, "echo")) {
			r = cmd_echo();
			guard(r);
		} else if (!strcmp(cmd, "mc")) {
			r = cmd_mc();
			guard(r);
		} else {
			r = cprintf("unrecognized command: %s\n", cmd);
			guard(r);
		}
	}
}
