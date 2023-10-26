#ifndef __KERNEL_DRIVERS_TTY_TTY_H
#define __KERNEL_DRIVERS_TTY_TTY_H

#include "stdint.h"

typedef struct {
  void (*putchar)(char c);
} tty_driver_t;

int tty_register_driver(const tty_driver_t *driver);

const tty_driver_t *tty_get_driver(int idx);
int tty_get_driver_count(void);

void _putchar(char a);

#endif // __KERNEL_DRIVERS_TTY_TTY_H
