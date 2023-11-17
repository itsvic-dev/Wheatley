#include "sys/cpuid.h"
#include "sys/msr.h"
#include <assert.h>
#include <libk.h>
#include <mm/mm.h>
#include <printf.h>
#include <sys/pcrb.h>

extern pcrb_t *pcrbs;

static inline void writegs(void *addr) { wrmsr(0xc0000101, (uint64_t)addr); }

static inline uint8_t readgs() {
  uint8_t value;
  asm volatile("mov %[value], gs:[0]" : [value] "=q"(value)::"memory");
  return value;
}

void pcrb_init() {
  uint8_t apicID = getApicID();
  pcrb_t *pcrb = &pcrbs[apicID];

  pcrb->apicID = apicID;

  writegs(pcrb);
  printf("pcrb: initialized for core %d @ %#llx\n", pcrb->apicID, pcrb);
}

pcrb_t *pcrb_get() { return &pcrbs[readgs()]; }
