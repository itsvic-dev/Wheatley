#ifndef _LIBK_H
#define _LIBK_H

#include "bootproto.h"
#include "stdint.h"

extern bootproto_handoff_t *g_handoff;

void *memset(void *dest, int ch, uint64_t count);
void *memcpy(void *dest, const void *src, uint64_t n);
char *strcpy(char *dest, const char *source);

#endif
