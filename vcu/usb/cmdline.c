#include "atmel_start.h"
#include "FreeRTOS.h"
#include "task.h"

#include <string.h>

#include "log/msglog.h"
#include "usb/usb_console.h"

#define guard(x) if (x < 0) return -1;

static int ping(void) {
	int r;
	r = cwrite("pong\n", 5);
	guard(r);
	return 0;
}

static int echo(void) {
	int r;
	char *s = strtok(NULL, "");
	r = cprintf("%s\n", s);
	guard(r);
	return 0;
}

int cmdline_main(void) {
	static char line_buf[256];
	int r;

	r = cwrite("\n", 1);
	guard(r);

	while (1) {
		r = cwrite("> ", 3);
		guard(r);

		r = cgets(line_buf, sizeof(line_buf));
		guard(r);

		char *cmd = strtok(line_buf, " ");

		if (!strcmp(cmd, "ping")) {
			r = ping();
			guard(r);
		} else if (!strcmp(cmd, "echo")) {
			r = echo();
			guard(r);
		} else {
			r = cprintf("unrecognized command: %s\n", cmd);
			guard(r);
		}
	}
}
