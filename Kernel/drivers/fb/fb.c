#include "fb.h"

fb_driver_t *fb_drivers[256];
int fb_drivers_count = 0;

int fb_register_driver(fb_driver_t *driver) {
    int ret = fb_drivers_count++;
    fb_drivers[ret] = driver;
    return ret;
}
