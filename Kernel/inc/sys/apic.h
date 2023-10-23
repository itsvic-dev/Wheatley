#ifndef __KERNEL_FW_APIC_H
#define __KERNEL_FW_APIC_H

#include <stdint.h>

void lapic_init();
void apic_init();

#define SPURIOUS_INTERRUPT_VECTOR_REG (0xF0)

uint32_t lapic_read(size_t reg);
void lapic_write(size_t reg, uint32_t value);

#endif // __KERNEL_FW_APIC_H
