#ifndef __KERNEL_SYS_VMM_H
#define __KERNEL_SYS_VMM_H

#include <stdint.h>

#define PAGE_READ (0b1)
#define PAGE_WRITE (0b10)
#define PAGE_EXECUTE (1ull << 63ull)
#define INVALID_PHYS ((uint64_t)0xFFFFFFFFFFFFFFFF)

typedef struct {
    uint64_t *topLevel;
} vmm_pagemap_t;

void vmm_init();
bool vmm_map_page(vmm_pagemap_t *pagemap, uint64_t physAddr, uint64_t virtAddr, uint64_t flags);
bool vmm_unmap_virt(vmm_pagemap_t *pagemap, uint64_t virtAddr);
uint64_t vmm_virt_to_phys(vmm_pagemap_t *pagemap, uint64_t physAddr, uint64_t virtAddr, uint64_t flags);

#endif // __KERNEL_SYS_VMM_H
