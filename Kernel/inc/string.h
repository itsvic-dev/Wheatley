#ifndef _STRING_H
#define _STRING_H

#include "stdint.h"

void *memset(void *dest, int ch, size_t count);
void *memcpy(void *dest, const void *src, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);

size_t strlen(const char *str);
char *strcpy(char *dest, const char *source);

#endif
