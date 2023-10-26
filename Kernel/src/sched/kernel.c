#include "sys/cpuid.h"
#include <printf.h>
#include <sched/kernel.h>

void kernelTaskEntry(void) {
  printf("hi from kernel task on core %d!\n", getApicID());
  // for (;;)
  //   asm("hlt");
}
