#include "tty.h"
#include "printf.h"

const tty_driver_t *tty_drivers[256];
int tty_drivers_count = 0;

int tty_register_driver(const tty_driver_t *driver) {
    int ret = tty_drivers_count++;
    tty_drivers[ret] = driver;
    printf("tty: registered tty %d (driver @ %#llx)\n", ret, driver);
    return ret;
}

const tty_driver_t *tty_get_driver(int idx) {
    if (idx > tty_drivers_count) return NULL;
    return tty_drivers[idx];
}

int tty_get_driver_count(void) {
    return tty_drivers_count;
}

void _putchar(char a) {
    // print a to all TTYs
    for (int i = 0; i < tty_drivers_count; i++) {
        tty_drivers[i]->putchar(a);
    }
}
