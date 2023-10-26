#ifndef __KERNEL_PANIC_H
#define __KERNEL_PANIC_H

#include <sys/isr.h>

__attribute__((noreturn)) void panic(const char *reason, registers_t *r);

void nmi_vector(registers_t *registers);

#endif // __KERNEL_PANIC_H
