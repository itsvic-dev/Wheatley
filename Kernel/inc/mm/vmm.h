#ifndef __KERNEL_MM_VMM_H
#define __KERNEL_MM_VMM_H

#include <stdint.h>

uint64_t *vmm_get_current_pagemap();
bool vmm_map_page(uint64_t *pagemap, uint64_t physAddr, uint64_t virtAddr, uint64_t flags);
bool vmm_unmap_page(uint64_t *pagemap, uint64_t virtAddr);

#endif // __KERNEL_MM_VMM_H
