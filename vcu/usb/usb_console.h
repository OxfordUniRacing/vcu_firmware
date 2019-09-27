#ifndef USB_CONSOLE_H
#define USB_CONSOLE_H

int cwrite(const void *src, unsigned int count);
int cprintf(const char *format, ...);
int cgetc(void);
int cgets(char *s, int size);

void usb_console_init(void);
int cmdline_main(void);

#endif // USB_CONSOLE_H
