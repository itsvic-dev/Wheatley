#include "sched/sched.h"
#include "sys/pcrb.h"
#include <deepMain.h>
#include <printf.h>
#include <sys/cpuid.h>

void deepMain(void) {
  printf("entered deepMain on core %d\n", getApicID());

  pcrb_init();
  sched_start();

  for (;;)
    asm("hlt");
}
