#include <deepMain.h>
#include <sys/cpuid.h>
#include <printf.h>

void deepMain(void) {
    printf("entered deepMain on core %d\n", getApicID());

    for(;;)
        asm("hlt");
}
