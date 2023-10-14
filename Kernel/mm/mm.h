#ifndef __KERNEL_MM_MM_H
#define __KERNEL_MM_MM_H

#include "stdint.h"

void *kmalloc(uint64_t len);
void kfree(void *buf);

#endif // __KERNEL_MM_MM_H
