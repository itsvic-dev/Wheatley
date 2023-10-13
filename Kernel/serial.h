#ifndef __KERNEL_SERIAL_H
#define __KERNEL_SERIAL_H

#define COM1_PORT 0x3f8

#include "stdint.h"

bool init_serial();
void write_serial(char a);
void puts(const char *str);

#endif // __KERNEL_SERIAL_H
