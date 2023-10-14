#include "bootproto.h"
#include "printf.h"

#include "drivers/fb/efifb.h"
#include "drivers/tty/serialtty.h"

bootproto_handoff_t g_handoff;

void kernel_main(bootproto_handoff_t *handoff) {
  g_handoff = *handoff;
  
  // init early modules such as serialtty, efifb, fbtty
  serialtty_module_init();
  efifb_module_init();

  printf("hello, kernel world!\n");

  for (;;)
    __asm__("hlt");
}
