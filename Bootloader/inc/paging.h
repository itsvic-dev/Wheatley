#ifndef __BL_PAGING_H
#define __BL_PAGING_H

#include "helpers.h"

static inline uint64_t __readcr0(void) {
    uint64_t value;
    __asm__ __volatile__("mov %[value], cr0" : [value] "=q"(value));
    return value;
}

static inline uint64_t __readcr3(void) {
    uint64_t value;
    __asm__ __volatile__("mov %[value], cr3" : [value] "=q"(value));
    return value;
}

static inline void __writecr0(uint64_t value) {
    __asm__ __volatile__("mov cr0, %[value]" : : [value] "q"(value) : "memory");
}

bool Paging_MapPage(uint64_t *pagemap, uint64_t physAddr, uint64_t virtAddr, uint64_t flags);

#endif
