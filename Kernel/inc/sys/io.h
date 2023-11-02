#ifndef __KERNEL_IO_H
#define __KERNEL_IO_H

#include "stdint.h"

void outb(uint16_t port, uint8_t val);
void outw(uint16_t port, uint16_t val);
void outd(uint16_t port, uint32_t val);
uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);
uint32_t ind(uint16_t port);

static inline uint64_t __readcr3(void) {
  uint64_t value;
  asm volatile("mov %[value], cr3" : [value] "=q"(value));
  return value;
}

static inline void __writecr3(uint64_t value) {
  asm volatile("mov cr3, %[value]" ::[value] "q"(value));
}

#endif // __KERNEL_IO_H
