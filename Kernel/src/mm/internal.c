#include <mm/internal.h>

void _mm_add_alloc(bootproto_mmap_entry_t *entry) {
  mm_alloc_data_t *alloc = &_mm_allocs[_mm_alloc_count++];

  uint64_t pageStart = entry->start / 4096;
  alloc->pageStart = pageStart;
  alloc->pageCount = entry->pages;
  alloc->used = entry->type != free;
}

bool _mm_get_page_bit(uint64_t page) {
  uint8_t pageBit = page % 32;
  uint64_t pageIdx = page / 32;

  return (_mm_pages[pageIdx] >> pageBit) & 1;
}

void _mm_set_page_bit(uint64_t page) {
  uint8_t pageBit = page % 32;
  uint64_t pageIdx = page / 32;

  _mm_pages[pageIdx] |= (1 << pageBit);
}

void _mm_clear_page_bit(uint64_t page) {
  uint8_t pageBit = page % 32;
  uint64_t pageIdx = page / 32;

  _mm_pages[pageIdx] &= ~(uint32_t)(1 << pageBit);
}
