#ifndef __KERNEL_FW_APIC_H
#define __KERNEL_FW_APIC_H

#include <stdint.h>

void lapic_init();
void apic_init();

#define SPURIOUS_INTERRUPT_VECTOR_REG (0xF0)

uint32_t lapic_read(size_t reg);
void lapic_write(size_t reg, uint32_t value);

void apic_send_ipi(uint32_t lapic_id, uint32_t flags);
static inline void apic_eoi(void) { lapic_write(0xB0, 0); }

#endif // __KERNEL_FW_APIC_H
