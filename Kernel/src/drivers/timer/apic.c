#include <drivers/timer/apic.h>
#include <drivers/timer/hpet.h>
#include <panic.h>
#include <printf.h>
#include <sys/apic.h>

uint32_t ticksIn10ms;

#define APIC_TIMER_DIV 0x3e0
#define APIC_INIT_COUNT 0x380
#define APIC_CURRENT_COUNT 0x390
#define APIC_LVT_TIMER 0x320
#define TIMER_INT ((uint32_t)32)
#define TIMER_INT_MASKED (TIMER_INT | ((uint32_t)1 << 16))

void apic_timer_init() {
  printf("apic: callibrating timer with HPET\n");

  lapic_write(0x3E0, 3);
  lapic_write(0x380, 0xFFFFFFFF);
  hpet_sleep(10 * 100000);
  lapic_write(0x320, 0x10000);
  ticksIn10ms = 0xFFFFFFFF - lapic_read(0x390);
  printf("ticksIn10ms=%#llx\n", ticksIn10ms);
}

void timer_stop_sched(void) {
  lapic_write(0x380, 0);
  lapic_write(0x320, (1 << 16));
}

void timer_sched_oneshot(uint8_t isr, uint32_t us) {
  asm("cli");
  timer_stop_sched();
  lapic_write(0x320, isr | 0x20000);
  lapic_write(0x3E0, 3);
  lapic_write(0x380, 0xFFFFFFFF - ((ticksIn10ms * (us / 1000))) / 10);
  asm("sti");
}
