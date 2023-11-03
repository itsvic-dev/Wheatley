#include "mm/mm.h"
#include "sys/pcrb.h"
#include <deepMain.h>
#include <drivers/timer/hpet.h>
#include <fw/madt.h>
#include <libk.h>
#include <printf.h>
#include <sys/apic.h>
#include <sys/cpuid.h>
#include <sys/gdt.h>
#include <sys/idt.h>
#include <sys/io.h>
#include <sys/smp.h>
#include <sys/spinlock.h>

void ap_trampoline(void);

volatile uint8_t aprunning = 0;
uint8_t bspID;
static spinlock_t aprLock = SPINLOCK_INIT;

pcrb_t *pcrbs = NULL;

void smp_init() {
  cpuid_data_t data = cpuid(1);
  bspID = data.rbx >> 24;

  // set up space for pcrbs
  pcrbs = kmalloc(lapics_length * sizeof(pcrb_t));
  memset(pcrbs, 0, lapics_length * sizeof(pcrb_t));

  // copy the AP trampoline to a fixed address in low conventional mem
  memcpy((void *)0x8000, &ap_trampoline, 4096);

  // copy the GDT pointer
  memcpy((void *)0x9000, &gdt_ptr, sizeof(gdt_ptr_t));

  // put value of CR3 at 0x9010
  *(uint32_t *)0x9010 = __readcr3();

  // for each local APIC
  for (int i = 0; i < lapics_length; i++) {
    madt_lapic_t *lapic = lapics[i];
    // no need to bring up the BSP
    if (lapic->apicID == bspID)
      continue;
    // if the processor isn't enabled nor online capable,
    // don't try to bring it up
    if ((lapic->flags & 0b01) == 0 && (lapic->flags & 0b10) == 0)
      continue;

    printf("smp: bringing up LAPIC #%d\n", i);

    lapic_write(0x280, 0);      // clear APIC errors
    apic_send_ipi(i, 0x00C500); // init IPI
    do {
      asm("pause");
    } while (lapic_read(0x300) & (1 << 12)); // wait for delivery
    apic_send_ipi(i, 0x008500);              // deassert
    do {
      asm("pause");
    } while (lapic_read(0x300) & (1 << 12)); // wait for delivery
    mdelay(10);

    // send startup IPI twice
    for (int j = 0; j < 2; j++) {
      lapic_write(0x280, 0);      // clear APIC errors
      apic_send_ipi(i, 0x000608); // startup IPI for 0800:0000
      udelay(200);
      do {
        asm("pause");
      } while (lapic_read(0x300) & (1 << 12)); // wait for delivery
    }
  }
  spinlock_wait_and_acquire(&aprLock);
  printf("smp: APs running=%d\n", aprunning);
  spinlock_release(&aprLock);

  // done initialising APs, let's enter deep main
  deepMain();
}

void ap_startup() {
  // increment `aprunning` because lol
  spinlock_wait_and_acquire(&aprLock);
  aprunning++;
  spinlock_release(&aprLock);

  // bring the CPU back up to speed with the rest of the system
  gdt_reload();
  idt_reload();
  lapic_init();

  // and enter deep main
  deepMain();

  for (;;)
    asm("hlt");
}
