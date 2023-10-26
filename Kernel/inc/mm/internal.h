#ifndef __KERNEL_MM_INTERNAL_H
#define __KERNEL_MM_INTERNAL_H

#include "bootproto.h"
#include "printf.h"
#include "stdint.h"
#include <sys/spinlock.h>

#define MM_PRINT(fmt, ...) printf("mm: " fmt "\n", ##__VA_ARGS__)

typedef struct {
  uint64_t pageStart;
  uint16_t pageCount;
  bool used;
} __attribute__((__packed__)) mm_alloc_data_t;

extern uint32_t *_mm_pages;
extern uint64_t _mm_page_count;

extern spinlock_t _mm_spinlock;

#define MM_ALLOC_MAX_SIZE 1024
extern mm_alloc_data_t *_mm_allocs;
extern uint16_t _mm_alloc_count;

void _mm_add_alloc(bootproto_mmap_entry_t *entry);
bool _mm_get_page_bit(uint64_t page);
void _mm_set_page_bit(uint64_t page);
void _mm_clear_page_bit(uint64_t page);

#endif
