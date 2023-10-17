#ifndef _LIBK_H
#define _LIBK_H

#include "bootproto.h"
#include "stdint.h"

extern bootproto_handoff_t *g_handoff;

void *memset(void *dest, int ch, uint64_t count);
void *memcpy(void *dest, const void *src, size_t n);
int memcmp(const void *s1, const void *s2, size_t n);

char *strcpy(char *dest, const char *source);

#endif
