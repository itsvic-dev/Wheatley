#include "sys/cpuid.h"
#include "sys/msr.h"
#include <libk.h>
#include <mm/mm.h>
#include <printf.h>
#include <sys/pcrb.h>

static inline void set_gs(void *addr) { wrmsr(0xc0000101, (uint64_t)addr); }

void pcrb_init() {
  uint8_t apicID = getApicID();
  printf("pcrb: initializing for core %d\n", apicID);
  pcrb_t *pcrb = kmalloc(sizeof(pcrb_t));
  memset(pcrb, 0, sizeof(pcrb_t));

  pcrb->apicID = apicID;

  set_gs(pcrb);
}

pcrb_t *pcrb_get() {
  uint64_t gs = 0;
  asm volatile("mov %[gs], gs" : [gs] "=q"(gs)::"memory");
  return (pcrb_t *)gs;
}
