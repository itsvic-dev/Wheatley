#include "stdint.h"
#include "libk.h"

void *memset(void *dest, int ch, uint64_t count) {
    uint8_t *ptr = dest;
    uint8_t val = ch;
    while (count--) {
        *ptr++ = val;
    }
    return dest;
}
