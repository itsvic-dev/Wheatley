#include "stdint.h"
#include "libk.h"
#include "printf.h"

void *memset(void *dest, int ch, size_t count) {
    // printf("memset(%#llx, %d, %lld)\n", dest, ch, count);
    uint8_t *ptr = dest;
    uint8_t val = ch;
    while (count--) {
        *ptr++ = val;
    }
    return dest;
}

void *memcpy(void *dest, const void *src, size_t n) {
    uint8_t *ptr = dest;
    const uint8_t *ptr2 = src;
    while (n--)
        *ptr++ = *ptr2++;
    return dest;
}

int memcmp(const void *s1, const void *s2, size_t n) {
    if (s1 == s2) return 0;

    uint8_t *p1 = (uint8_t *)s1;
    uint8_t *p2 = (uint8_t *)s2;

    while (n--) {
        if (*p1 != *p2) return *p2 - *p1;
        p1++;
        p2++;
    }

    return 0;
}

char *strcpy(char *dest, const char *source) {
    char *p = dest;
    while ((*dest++ = *source++));
    return p;
}
