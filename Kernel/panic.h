#ifndef __KERNEL_PANIC_H
#define __KERNEL_PANIC_H

__attribute__((noreturn)) void panic(const char *reason);

#endif // __KERNEL_PANIC_H
