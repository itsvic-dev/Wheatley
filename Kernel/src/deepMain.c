#include <deepMain.h>
#include <printf.h>
#include <sys/cpuid.h>

void deepMain(void) {
  printf("entered deepMain on core %d\n", getApicID());

  for (;;)
    asm("hlt");
}
