#include "mm/mm.h"
#include "mm/vmm.h"
#include <assert.h>
#include <drivers/fb/efifb.h>
#include <drivers/fb/fb.h>
#include <libk.h>
#include <printf.h>
#include <sys/spinlock.h>

static spinlock_t spinlock = SPINLOCK_INIT;

static fb_info_t info;

fb_info_t *efifb_get_info(void) { return &info; }
static uint32_t *doubleBuf;

void efifb_setpixel(int x, int y, uint32_t pixel) {
  spinlock_wait_and_acquire(&spinlock);
  int idx = y * g_handoff->fb_pixelsPerScanLine + x;
  g_handoff->fb_buffer[idx] = pixel;
  doubleBuf[idx] = pixel;
  spinlock_release(&spinlock);
}

void efifb_readpixels(uint32_t *buf, size_t offset, size_t count) {
  spinlock_wait_and_acquire(&spinlock);
  memcpy(buf, doubleBuf + offset, count);
  spinlock_release(&spinlock);
}

void efifb_memcpy(uint32_t *buf, size_t offset, size_t count) {
  spinlock_wait_and_acquire(&spinlock);
  memcpy(g_handoff->fb_buffer + offset, buf, count);
  memcpy(doubleBuf + offset, buf, count);
  spinlock_release(&spinlock);
}

void efifb_pixset(uint32_t color, size_t offset, size_t count) {
  spinlock_wait_and_acquire(&spinlock);
  while (count--) {
    g_handoff->fb_buffer[offset] = color;
    doubleBuf[offset++] = color;
  }
  spinlock_release(&spinlock);
}

fb_driver_t efifb_driver;

void efifb_module_init() {
  efifb_driver.get_info = &efifb_get_info;
  efifb_driver.setpixel = &efifb_setpixel;
  efifb_driver.readpixels = &efifb_readpixels;
  efifb_driver.memcpy = &efifb_memcpy;
  efifb_driver.pixset = &efifb_pixset;

  assert(g_handoff->fb_pixelsPerScanLine == g_handoff->fb_width);
  info.width = g_handoff->fb_width;
  info.height = g_handoff->fb_height;

  // the buffer needs to be page mapped as its not a part of handoff's memory
  // mappings
  uint64_t spaceNeeded =
      g_handoff->fb_pixelsPerScanLine * g_handoff->fb_width * 4;
  uint64_t pagesNeeded = (spaceNeeded + 4095) / 4096;
  for (int i = 0; i < pagesNeeded; i++) {
    uint64_t addr = (uint64_t)g_handoff->fb_buffer + (i * 4096);
    vmm_map_page(vmm_get_current_pagemap(), addr, addr, 0b11);
  }

  for (int idx = 0; idx < g_handoff->fb_pixelsPerScanLine * g_handoff->fb_width;
       idx++)
    g_handoff->fb_buffer[idx] = 0;
  doubleBuf = kmalloc(info.width * info.height * 4);
  memset(doubleBuf, 0, info.width * info.height * 4);
  fb_register_driver(&efifb_driver);
}
