#ifndef _LIBK_H
#define _LIBK_H

#include "bootproto.h"
#include "mm/mm.h"
#include "stdint.h"

extern bootproto_handoff_t *g_handoff;

void *memset(void *dest, int ch, uint64_t count);

#endif
