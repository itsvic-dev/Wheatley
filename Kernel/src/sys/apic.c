#include "mm/vmm.h"
#include <assert.h>
#include <drivers/timer/apic.h>
#include <fw/acpi.h>
#include <fw/madt.h>
#include <panic.h>
#include <printf.h>
#include <sys/apic.h>
#include <sys/cpuid.h>
#include <sys/io.h>
#include <sys/mmio.h>
#include <sys/msr.h>

bool x2apic = false;

static uint32_t ioapic_read(uint64_t ioapic_address, size_t reg) {
  vmm_map_page(vmm_get_kernel_pagemap(), ioapic_address, ioapic_address, 0b11);
  mmoutd((void *)ioapic_address, reg & 0xFF);
  return mmind((void *)ioapic_address + 16);
}

static void ioapic_write(uint64_t ioapic_address, size_t reg, uint32_t data) {
  mmoutd((void *)ioapic_address, reg & 0xFF);
  mmoutd((void *)ioapic_address + 16, data);
}

static uint32_t get_gsi_count(uint64_t ioapic_address) {
  return (ioapic_read(ioapic_address, 1) & 0xFF0000) >> 16;
}

static madt_ioapic_t *get_ioapic_by_gsi(uint32_t gsi) {
  for (int i = 0; i < ioapics_length; i++) {
    madt_ioapic_t *ioapic = ioapics[i];
    if (ioapic->gsiBase <= gsi &&
        ioapic->gsiBase + get_gsi_count(ioapic->address) > gsi) {
      return ioapic;
    }
  }

  return NULL;
}

void ioapic_redirect_gsi(uint32_t gsi, uint8_t vec, uint16_t flags) {
  uint32_t ioapic = get_ioapic_by_gsi(gsi)->address;

  uint32_t lowIndex = 0x10 + (gsi - get_ioapic_by_gsi(gsi)->gsiBase) * 2;
  uint32_t highIndex = lowIndex + 1;

  uint32_t high = ioapic_read(ioapic, highIndex);

  // set APIC ID
  high &= ~0xFF000000;
  high |= ioapic_read(ioapic, 0) << 24;
  ioapic_write(ioapic, highIndex, high);

  uint32_t low = ioapic_read(ioapic, lowIndex);

  // unmask the IRQ
  low &= ~(1 << 16);

  // set to physical delivery mode
  low &= ~(1 << 11);

  // set to fixed delivery mode
  low &= ~0x700;

  // set delivery vector
  low &= ~0xFF;
  low |= vec;

  // active high (0) or low (1)
  if (flags & 2) {
    low |= 1 << 13;
  }

  // edge (0) or level (1) triggered
  if (flags & 8) {
    low |= 1 << 15;
  }

  ioapic_write(ioapic, lowIndex, low);
}

void ioapic_redirect_irq(uint32_t irq, uint8_t vect) {
  for (int i = 0; i < isos_length; i++) {
    if (isos[i]->irqSource == irq) {
      ioapic_redirect_gsi(isos[i]->gsi, vect, isos[i]->flags);
      return;
    }
  }

  ioapic_redirect_gsi(irq, vect, 0);
}

static void lapic_set_nmi(uint8_t vec, uint8_t currentProcessor,
                          uint8_t processor, uint16_t flags, uint8_t lint) {
  assert(lint < 2);
  if (processor != 0xFF && currentProcessor != processor)
    return;

  // set to raise in vector "vec" and set NMI flag
  uint32_t nmi = 0x400 | vec;

  // set to active low
  if (flags & 2)
    nmi |= 1 << 13;

  // set to level triggered
  if (flags & 8)
    nmi |= 1 << 15;

  if (lint == 0) {
    lapic_write(0x350, nmi);
  } else if (lint == 1) {
    lapic_write(0x360, nmi);
  } else {
    printf("wtf??? lint=%d\n", lint);
  }
}

void lapic_init() {
  cpuid_data_t flags = cpuid(1);
  uint16_t apicID = flags.rbx >> 24;
  printf("apic: initialising LAPIC on core %d\n", apicID);

  uint64_t apic_msr = rdmsr(0x1B);
  apic_msr |= 1 << 11;
  if (flags.rcx & (1 << 21)) {
    x2apic = true;
    // set x2APIC flag
    apic_msr |= 1 << 10;
  }
  wrmsr(0x1B, apic_msr);

  if (madt->flags & 1) {
    // remap the PIC
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 4);
    outb(0xA1, 2);
    outb(0x21, 1);
    outb(0xA1, 1);
    outb(0x21, 0);
    outb(0xA1, 0);
    // disable the PIC
    outb(0xA1, 0xFF);
    outb(0x21, 0xFF);
  }

  // init LAPIC
  lapic_write(0x80, 0);
  lapic_write(0xF0, lapic_read(0xF0) | 0x100);
  if (!x2apic) {
    lapic_write(0xE0, 0xF0000000);
    lapic_write(0xD0, lapic_read(0x20));
  }

  // set NMIs
  for (int i = 0; i < nmis_length; i++) {
    madt_lapic_nmi_t *nmi = nmis[i];
    lapic_set_nmi(2, apicID, nmi->processor, nmi->flags, nmi->lint);
  }

  apic_timer_init();
}

static inline uint32_t reg_to_x2apic(uint32_t reg) {
  uint32_t x2apic_reg = 0;
  // MSR 831H is reserved. APIC register 310H is accessible at x2APIC MSR 830H
  if (reg == 0x310) {
    x2apic_reg = 0x30;
  } else {
    x2apic_reg = reg >> 4;
  }

  return x2apic_reg + 0x800;
}

uint32_t lapic_read(size_t reg) {
  if (x2apic)
    return rdmsr(reg_to_x2apic(reg));

  assert(lapic_addr != NULL);
  return mmind((void *)lapic_addr + reg);
}

void lapic_write(size_t reg, uint32_t value) {
  if (x2apic)
    return wrmsr(reg_to_x2apic(reg), value);

  assert(lapic_addr != NULL);
  mmoutd((void *)lapic_addr + reg, value);
}

void apic_init() {
  madt_init();
  lapic_init();
  ioapic_redirect_irq(0, 48);
}

void apic_send_ipi(uint32_t lapic_id, uint32_t flags) {
  if (x2apic) {
    // write to the MSR directly
    wrmsr(0x830, ((uint64_t)lapic_id << 32) | flags);
  } else {
    lapic_write(0x310, (lapic_id << 24));
    lapic_write(0x300, flags);
  }
}
