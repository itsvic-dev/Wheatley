#include "mm/vmm.h"
#include <acpispec/tables.h>
#include <assert.h>
#include <drivers/timer/hpet.h>
#include <fw/acpi.h>
#include <panic.h>
#include <printf.h>

typedef struct {
  acpi_header_t header;
  uint32_t etbID;
  acpi_gas_t gas;
  uint8_t hpetNumber;
  uint16_t minClockTick;
  uint8_t pageProtection;
} __attribute__((__packed__)) hpet_t;

hpet_t *hpet = NULL;
void *hpetData = NULL;

#define NANOSECONDS_100 (100000000) // 100 nanoseconds in femtoseconds
uint32_t multiplier = 0;

#define HPET_CONFIGURATION (hpet_gen_conf_reg_t *)(hpetData + 0x10)
#define TIMER_CONFIGURATION(n)                                                 \
  (hpet_timer_conf_reg_t *)(hpetData + 0x100 + 0x20 * n)
#define TIMER_COMPARATOR(n) (uint64_t *)(hpetData + 0x108 + 0x20 * n)

void hpet_init() {
  hpet = (hpet_t *)acpi_find_table("HPET", 0);
  if (hpet == NULL)
    panic("Wheatley requires an HPET", 0);

  printf("hpet: HPET @ %#llx\n", hpet);
  hpetData = (void *)hpet->gas.base;

  // identity map the HPET address
  vmm_map_page(vmm_get_kernel_pagemap(), (uint64_t)hpetData, (uint64_t)hpetData,
               0b11);

  // get general capabilities register
  hpet_gen_cap_reg_t genCap = *(hpet_gen_cap_reg_t *)hpetData;
  assert(genCap.revID != 0);
  multiplier = NANOSECONDS_100 / genCap.counterClkPeriod;
  printf("hpet: accuracy: %d ns\n", genCap.counterClkPeriod / 1000000);

  hpet_gen_conf_reg_t *genConf = HPET_CONFIGURATION;
  genConf->enable = 1; // run the main counter (im retarded)
  genConf->legacyReplace = 0;
}

uint64_t hpet_time() {
  // return time in 100ns accuracy
  return *(uint64_t *)(hpetData + 0xF0) / multiplier;
}

// sleep for N/100 ns
void hpet_sleep(uint64_t ns) {
  uint64_t comparatorTime = hpet_time() + (ns / 100);
  while (hpet_time() < comparatorTime)
    asm("pause");
}
