#include "serial.h"

void kernel_main(void) {
  if (!init_serial()) __asm__("int3");
  
  puts("hello, kernel world!");
  
  for (;;)
    __asm__("hlt");
}
