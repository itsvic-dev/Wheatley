#ifndef __KERNEL_MM_VMM_H
#define __KERNEL_MM_VMM_H

#include <stdint.h>
#include <sys/spinlock.h>

typedef struct {
  uint64_t *topLevel;
  spinlock_t lock;
} pagemap_t;

#define PAGE_READ (0b1)
#define PAGE_WRITE (0b10)
#define PAGE_EXECUTE (1ull << 63)
#define INVALID_PHYS (0xFFFFFFFFFFFFFFFFull)

uint64_t *vmm_get_current_pagemap();
bool vmm_map_page(uint64_t *pagemap, uint64_t physAddr, uint64_t virtAddr,
                  uint64_t flags);
bool vmm_unmap_page(uint64_t *pagemap, uint64_t virtAddr);

void vmm_init(void);

#endif // __KERNEL_MM_VMM_H
