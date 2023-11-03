#include "sched/sched.h"
#include "sys/cpuid.h"
#include "sys/pcrb.h"
#include <printf.h>
#include <sched/kernel.h>

static void moreThreads(void) {
  pcrb_t *pcrb = pcrb_get();
  printf("hi, did you know i'm yet another task on core %d?!\n", pcrb->apicID);
  for (;;)
    ;
}

void kernelTaskEntry(void) {
  pcrb_t *pcrb = pcrb_get();
  printf("hi from kernel task on core %d!\n", pcrb->apicID);

  // we should spawn some threads
  for (int i = 0; i < 64; i++)
    sched_spawn_kernel_task(moreThreads);

  printf("kernel task spawned 64 threads\n");

  for (;;)
    asm("hlt");
}
