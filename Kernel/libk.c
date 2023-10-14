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

char *strcpy(char *dest, const char *source) {
    char *p = dest;
    while ((*dest++ = *source++));
    return p;
}

void *memcpy(void *dest, const void *src, uint64_t n) {
    uint8_t *ptr = dest;
    const uint8_t *ptr2 = src;
    while (n--)
        *ptr++ = *ptr2++;
    return dest;
}
