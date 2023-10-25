#include <drivers/fb/efifb.h>
#include <drivers/fb/fb.h>
#include <libk.h>
#include <assert.h>
#include <sys/spinlock.h>

static spinlock_t spinlock = SPINLOCK_INIT;

fb_info_t __efifb_info;

fb_info_t *efifb_get_info(void) {
    return &__efifb_info;
}

void efifb_setpixel(int x, int y, uint32_t pixel) {
    spinlock_wait_and_acquire(&spinlock);
    g_handoff->fb_buffer[y * g_handoff->fb_pixelsPerScanLine + x] = pixel;
    spinlock_release(&spinlock);
}

void efifb_readpixels(uint32_t *buf, size_t offset, size_t count) {
    spinlock_wait_and_acquire(&spinlock);
    memcpy(buf, g_handoff->fb_buffer + offset, count);
    spinlock_release(&spinlock);
}

void efifb_memcpy(uint32_t *buf, size_t offset, size_t count) {
    spinlock_wait_and_acquire(&spinlock);
    memcpy(g_handoff->fb_buffer + offset, buf, count);
    spinlock_release(&spinlock);
}

fb_driver_t efifb_driver;

void efifb_module_init() {
    efifb_driver.get_info = &efifb_get_info;
    efifb_driver.setpixel = &efifb_setpixel;
    efifb_driver.readpixels = &efifb_readpixels;
    efifb_driver.memcpy = &efifb_memcpy;

    assert(g_handoff->fb_pixelsPerScanLine == g_handoff->fb_width);
    __efifb_info.width = g_handoff->fb_width;
    __efifb_info.height = g_handoff->fb_height;
    for (int idx = 0; idx < g_handoff->fb_pixelsPerScanLine * g_handoff->fb_width; idx++)
        g_handoff->fb_buffer[idx] = 0;
    fb_register_driver(&efifb_driver);
}
