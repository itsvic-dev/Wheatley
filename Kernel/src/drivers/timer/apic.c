#include <drivers/timer/apic.h>
#include <drivers/timer/hpet.h>
#include <panic.h>
#include <printf.h>
#include <sys/apic.h>

static uint32_t ticksIn10ms;

#define APIC_TIMER_DIV 0x3e0
#define APIC_INIT_COUNT 0x380
#define APIC_CURRENT_COUNT 0x390
#define APIC_LVT_TIMER 0x320

void apic_timer_init() {
  lapic_write(APIC_TIMER_DIV, 3);
  lapic_write(APIC_INIT_COUNT, 0xFFFFFFFF);
  hpet_sleep(10 * 1000 * 1000);
  lapic_write(APIC_LVT_TIMER, 0x10000);
  ticksIn10ms = 0xFFFFFFFF - lapic_read(APIC_CURRENT_COUNT);

  lapic_write(APIC_LVT_TIMER, 48 | 0x20000);
  lapic_write(APIC_TIMER_DIV, 3);
  lapic_write(APIC_INIT_COUNT, ticksIn10ms / 10);
}

void timer_stop_sched(void) {
  lapic_write(APIC_INIT_COUNT, 0);
  lapic_write(APIC_LVT_TIMER, (1 << 16));
}

void timer_sched_oneshot(uint8_t isr, uint32_t us) {
  asm("cli");
  timer_stop_sched();
  lapic_write(APIC_LVT_TIMER, isr | 0x20000);
  lapic_write(APIC_TIMER_DIV, 3);
  lapic_write(APIC_INIT_COUNT, ticksIn10ms * (us / 1000) / 10);
  asm("sti");
}
