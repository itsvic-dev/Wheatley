#include "serial.h"
#include "bootproto.h"
#include "printf.h"

#include "drivers/fb/efifb.h"

bootproto_handoff_t g_handoff;

void kernel_main(bootproto_handoff_t *handoff) {
  // fixme: turn this into a TTY driver.
  if (!init_serial()) __asm__("int3");
  
  puts("hello, kernel world!");

  printf("handoff=%#llx\n", handoff);
  g_handoff = *handoff;
  printf("fb: %dx%d @ %#llx\n", g_handoff.fb_width, g_handoff.fb_height, g_handoff.fb_buffer);

  efifb_module_init();

  for (;;)
    __asm__("hlt");
}
