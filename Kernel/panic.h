#ifndef __KERNEL_PANIC_H
#define __KERNEL_PANIC_H

#include "idt.h"

__attribute__((noreturn))
void panic(const char *reason);

__attribute__((noreturn, no_caller_saved_registers))
void panic_interrupt(const char *reason, interrupt_frame_t *int_frame);

#endif // __KERNEL_PANIC_H
