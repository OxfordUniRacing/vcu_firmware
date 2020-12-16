#include "atmel_start.h"

#include <stdarg.h>
#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"
#include "stream_buffer.h"

#include "driver/log_msg.h"
#include "utils.h"

#include "config.h"

#include <string.h>

#include "driver/usb/usb_console.h"
#include "driver/eeprom_emu.h"
#include "framework/param/param.h"
#include "app/motor_controller.h"

#define RET_CHECK(x) if (x < 0) return x;

#define TERM_MAX_LEN 256


static int cmd_ping(char** stok) {
	int r;
	r = 0;
  log_debug("pong\n");
	return r;
}

static int cmd_echo(char** stok) {
	int r;
	char *s = strtok_r(NULL, "", stok);
	r = 0;
  log_debug("%s\n", s);
	return r;
}

#define CMD_MC_HELP "mc <num> <command>"
static int cmd_mc(char** stok) {
	int r;
	struct mc_inst_t *mc = mc_get_inst(1);

	//char *inst = strtok_r(NULL, " ", stok);
	//if (!inst) return 0;

	char *subcmd = strtok_r(NULL, " ", stok);
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

static int cmd_param(char** stok) {
	int r;

	char *subcmd = strtok_r(NULL, " ", stok);
	if (!subcmd) return 0;

	if (!strcmp(subcmd, "commit")) {
		eeprom_emu_commit();
		return 0;
	}

	char *name = strtok_r(NULL, " ", stok);
	if (!name) return 0;

	int addr = param_find_by_name(name);
	if (addr < 0) {
		r = 0;
    log_debug("Unknown param: %s\n", name);
		return r;
	}

	if (!strcmp(subcmd, "getf")) {
		float x = param_read_float(addr);
		r = 0;
    log_debug("%s = %f\n", name, x);
		return r;
	} else if (!strcmp(subcmd, "setf")) {
		char *val = strtok_r(NULL, " ", stok);
		if (!val) return 0;
    log_debug("error\n");
		float x;
		r = sscanf(val, "%f", &x);
		if (r < 1) return 0;
    log_debug("error\n");
		param_write_float(addr, x);
		r = 0;
    log_debug("%s = %f\n", name, x);
		return r;
	}

	return 0;
}

int terminal_run(char* line_buf) {
  char *stok;
  int r;

  char *cmd = strtok_r(line_buf, " ", &stok);

  if (!cmd) {
  } else if (!strcmp(cmd, "ping")) {
    r = cmd_ping(&stok);
    RET_CHECK(r);
  } else if (!strcmp(cmd, "echo")) {
    r = cmd_echo(&stok);
    RET_CHECK(r);
  } else if (!strcmp(cmd, "mc")) {
    r = cmd_mc(&stok);
    RET_CHECK(r);
  } else if (!strcmp(cmd, "param")) {
    r = cmd_param(&stok);
    RET_CHECK(r);
  } else {
    r = 0;
    log_debug("unrecognized command: %s\n", cmd);
    RET_CHECK(r);
  }
}


void uart_term_task(void *unused) {
	(void) unused;

	static char term_buf[TERM_MAX_LEN];
	struct io_descriptor *io;
	usart_os_get_io(&UART_TERM, &io);


	
	while (1)
	{
		char* curr = term_buf;
		while (curr - term_buf < TERM_MAX_LEN-1 && *(curr-1) != '\n') {
			io_read(io, curr++, 1);
		}
		*(curr-1) = '\0';
		int r = terminal_run(term_buf);
		if (r != 0) log_error("terminal command %s resulted in error code %d", term_buf, r);
	}
}
