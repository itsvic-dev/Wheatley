#include "fb.h"
#include "printf.h"

fb_driver_t *fb_drivers[256];
int fb_drivers_count = 0;

int fb_register_driver(fb_driver_t *driver) {
    int ret = fb_drivers_count++;
    fb_drivers[ret] = driver;
    printf("fb: registered fb %d (driver @ %#llx)\n", ret, driver);
    return ret;
}

fb_driver_t *fb_get_driver(int idx) {
    if (idx > fb_drivers_count) return NULL;
    return fb_drivers[fb_drivers_count];
}

int fb_get_driver_count(void) {
    return fb_drivers_count;
}
