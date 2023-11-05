#include "mm/vmm.h"
#include <fw/acpi.h>
#include <fw/madt.h>
#include <panic.h>
#include <printf.h>
#include <sys/io.h>

madt_t *madt;
uint32_t *lapic_addr = NULL;

madt_lapic_t *lapics[256];
uint8_t lapics_length = 0;

madt_ioapic_iso_t *isos[256];
uint8_t isos_length = 0;

madt_ioapic_t *ioapics[256];
uint8_t ioapics_length = 0;

madt_lapic_nmi_t *nmis[256];
uint8_t nmis_length = 0;

void madt_init() {
  madt = (madt_t *)acpi_find_table("APIC", 0);
  if (!madt)
    panic("madt: no MADT found", 0);

  printf("madt: MADT @ %#llx, %#x bytes long\n", madt, madt->header.length);

  lapic_addr = (uint32_t *)(uint64_t)madt->lapic_address;

  // i love fucking with pointers
  madt_record_header_t *record =
      (madt_record_header_t *)((uint64_t)madt + 0x2C);
  while (record != NULL) {
    // this could've been a for loop
    switch (record->type) {
    case LAPIC_ADDRESS_OVERRIDE:
      lapic_addr = (uint32_t *)((madt_lapic_addr_override_t *)record)->address;
      break;
    case LAPIC:
      lapics[lapics_length++] = (madt_lapic_t *)record;
      break;
    case IOAPIC:
      ioapics[ioapics_length++] = (madt_ioapic_t *)record;
      break;
    case IOAPIC_INTERRUPT_SOURCE_OVERRIDE:
      isos[isos_length++] = (madt_ioapic_iso_t *)record;
      break;
    case LAPIC_NMI:
      nmis[nmis_length++] = (madt_lapic_nmi_t *)record;
      break;
    default:
      printf("apic: unhandled MADT record type %d\n", record->type);
      break;
    }

    record = (madt_record_header_t *)((uint64_t)record + record->length);
    if ((uint64_t)record >= (uint64_t)madt + madt->header.length) {
      record = NULL;
      break;
    }
  }

  printf("madt: LAPIC @ %#llx\n", lapic_addr);
  vmm_map_page(vmm_get_kernel_pagemap(), (uint64_t)lapic_addr,
               (uint64_t)lapic_addr, 0b11);
}
