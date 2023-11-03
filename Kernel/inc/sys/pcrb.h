#ifndef __KERNEL_SYS_PCRB_H
#define __KERNEL_SYS_PCRB_H

#include <stdint.h>

typedef struct {
  uint64_t apicID;
  void *currentTask;
} pcrb_t;

void pcrb_init();
pcrb_t *pcrb_get();

#endif // __KERNEL_SYS_PCRB_H
