// NOTHING MAKES SENSE ANYMORE

#include <mm/vmm.h>
#include <mm/mm.h>
#include <printf.h>

vmm_pagemap_t *kernel_pagemap = NULL;

static uint64_t *get_next_level(uint64_t *topLevel, size_t idx, bool allocate) {
    if (topLevel[idx] & 1) {
        return (uint64_t *)(topLevel[idx] & ~((uint64_t)0xFFF));
    }

    if (!allocate)
        return NULL;

    void *nextLevel = mm_alloc_page(1);
    topLevel[idx] = (uint64_t)nextLevel | 0b111;

    return (uint64_t *)nextLevel;
}

void vmm_init() {
    kernel_pagemap = mm_alloc_page(1);
    kernel_pagemap->topLevel = mm_alloc_page(1);

    for (uint64_t p = 256; p < 512; p++) {
        get_next_level(kernel_pagemap->topLevel, p, true);
    }

    // map first 4 GB of memory into itself
    for (uint64_t p = 0; p < 4096ull * 1024 * 1024; p += 4096) {
        vmm_map_page(kernel_pagemap, p, p, PAGE_READ | PAGE_WRITE);
    }

    // fixme: map kernel sections as given by the bldr

    printf("vmm: switching pagemaps now!!!!\n");
    asm volatile(
        "mov %0, %%cr3"
        :
        : "r"(kernel_pagemap->topLevel)
        : "memory"
    );

    printf("vmm: theres no way this works yet, int3 to see whats going on\n");
    asm("int3");
}

bool vmm_map_page(vmm_pagemap_t *pagemap, uint64_t physAddr, uint64_t virtAddr, uint64_t flags) {
    uint64_t pml4_entry = (virtAddr & ((uint64_t)0x1ff << 39)) >> 39;
    uint64_t pml3_entry = (virtAddr & ((uint64_t)0x1ff << 30)) >> 30;
    uint64_t pml2_entry = (virtAddr & ((uint64_t)0x1ff << 21)) >> 21;
    uint64_t pml1_entry = (virtAddr & ((uint64_t)0x1ff << 12)) >> 12;

    uint64_t *pml4, *pml3, *pml2, *pml1;

    pml4 = pagemap->topLevel;
    pml3 = get_next_level(pml4, pml4_entry, true);
    if (!pml3) return false;
    pml2 = get_next_level(pml3, pml3_entry, true);
    if (!pml2) return false;
    pml1 = get_next_level(pml2, pml2_entry, true);
    if (!pml1) return false;

    pml1[pml1_entry] = physAddr | flags;
    return true;
}
