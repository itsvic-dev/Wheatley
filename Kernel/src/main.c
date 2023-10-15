#include <bootproto.h>
#include <printf.h>

#include <drivers/fb/fb.h>
#include <drivers/fb/efifb.h>
#include <drivers/tty/fbtty.h>
#include <drivers/tty/serialtty.h>

#include <mm/init.h>
#include <mm/mm.h>

#include <sys/idt.h>
#include <sys/isr.h>
#include <libk.h>
#include <panic.h>

bootproto_handoff_t *g_handoff;

void kernel_main(bootproto_handoff_t *handoff) {
  g_handoff = handoff;

  // init early modules such as serialtty, efifb, fbtty
  serialtty_module_init();
  efifb_module_init();
  fbtty_module_init();

  // init MM
  mm_init();

  setup_idt();
  setup_isrs();
  
  panic("we're done for now", 0);
}
