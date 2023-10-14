#include "bootproto.h"
#include "printf.h"

#include "drivers/fb/fb.h"
#include "drivers/fb/efifb.h"
#include "drivers/tty/fbtty.h"
#include "drivers/tty/serialtty.h"

#include "mm/init.h"
#include "mm/mm.h"

#include "idt.h"
#include "libk.h"
#include "panic.h"

bootproto_handoff_t *g_handoff;

void INTERRUPT int_bp(interrupt_frame_t *frame);
void INTERRUPT int_gp(interrupt_frame_t *frame);
void INTERRUPT int_pf(interrupt_frame_t *frame);
void INTERRUPT int_df(interrupt_frame_t *frame);

void kernel_main(bootproto_handoff_t *handoff) {
  g_handoff = handoff;

  // init early modules such as serialtty, efifb, fbtty
  serialtty_module_init();
  efifb_module_init();
  fbtty_module_init();

  // init MM
  mm_init();

  setup_idt();
  idt_set_handler(3, trap, &int_bp);
  idt_set_handler(8, trap, &int_df);
  idt_set_handler(13, trap, &int_gp);
  idt_set_handler(14, trap, &int_pf);
  
  panic("we're done for now");
}

void INTERRUPT int_bp(interrupt_frame_t *frame) {
  panic_interrupt("Breakpoint (#BP)", frame);
}

void INTERRUPT int_gp(interrupt_frame_t *frame) {
  panic_interrupt("General Protection (#GP)", frame);
}

void INTERRUPT int_pf(interrupt_frame_t *frame) {
  panic_interrupt("Page Fault (#PF)", frame);
}

void INTERRUPT int_df(interrupt_frame_t *frame) {
  panic_interrupt("Double Fault (#DF)", frame);
}
