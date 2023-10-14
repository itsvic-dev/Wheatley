#include "serial.h"
#include "bootproto.h"
#include "printf.h"

bootproto_handoff_t g_handoff;

void kernel_main(bootproto_handoff_t *handoff) {
  if (!init_serial()) __asm__("int3");
  
  puts("hello, kernel world!");

  printf("handoff=%#llx\n", handoff);
  g_handoff = *handoff;
  printf("fb: %dx%d @ %#llx\n", g_handoff.fb_width, g_handoff.fb_height, g_handoff.fb_buffer);

  for (int x = 0; x < g_handoff.fb_width; x++) {
    for (int y = 0; y < g_handoff.fb_height; y++) {
      int idx = y * g_handoff.fb_pixelsPerScanLine + x;
      g_handoff.fb_buffer[idx] = 0xffff0000;
    }
  }

  for (;;)
    __asm__("hlt");
}
