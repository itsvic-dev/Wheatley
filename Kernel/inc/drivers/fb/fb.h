#ifndef __KERNEL_DRIVERS_FB_FB_H
#define __KERNEL_DRIVERS_FB_FB_H

#include "stdint.h"

typedef struct {
    uint16_t width;
    uint16_t height;
} fb_info_t;

typedef struct {
    fb_info_t *(*get_info)(void);
    void (*setpixel)(int x, int y, uint32_t pixel);
} fb_driver_t;

int fb_register_driver(fb_driver_t *driver);

fb_driver_t *fb_get_driver(int idx);
int fb_get_driver_count(void);

#endif // __KERNEL_DRIVERS_FB_FB_H
