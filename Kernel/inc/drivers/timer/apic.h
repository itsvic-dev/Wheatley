#ifndef __KERNEL_DRIVERS_TIMER_APIC_H
#define __KERNEL_DRIVERS_TIMER_APIC_H

#include <stdint.h>

void apic_timer_init();
void timer_stop_sched(void);
void timer_sched_oneshot(uint8_t isr, uint32_t us);

#endif // __KERNEL_DRIVERS_TIMER_APIC_H
