#include <assert.h>
#include <libk.h>
#include <mm/init.h>
#include <mm/internal.h>
#include <stdint.h>

uint32_t *_mm_pages = NULL;
uint64_t _mm_page_count = 0;

mm_alloc_data_t *_mm_allocs = NULL;
uint16_t _mm_alloc_count = 0;

spinlock_t _mm_spinlock = SPINLOCK_INIT;

void mm_init(void) {
  spinlock_wait_and_acquire(&_mm_spinlock);
  MM_PRINT("setting up allocation tables");

  // get the needed page count for _mm_pages
  for (int i = 0; i < g_handoff->mmap_entries_length; i++) {
    _mm_page_count += g_handoff->mmap_entry[i].pages;
  }
  _mm_page_count = (_mm_page_count + 7) / 8;

  // add needed pages for the allocation table
  uint16_t alloc_pages =
      ((sizeof(mm_alloc_data_t) * MM_ALLOC_MAX_SIZE) + 4095) / 4096;
  uint64_t total_needed_pages = alloc_pages + _mm_page_count;

  // find space
  MM_PRINT("need %d pages", total_needed_pages);
  for (int i = 0; i < g_handoff->mmap_entries_length; i++) {
    bootproto_mmap_entry_t *entry = &g_handoff->mmap_entry[i];

    if (entry->pages >= total_needed_pages && entry->type == free) {
      MM_PRINT("found space @ %#llx (%d pages)", entry->start, entry->pages);
      // put _mm_pages into that space
      _mm_pages = (uint32_t *)entry->start;
      break;
    }
  }
  assert(_mm_pages != NULL);
  // put _mm_allocs after _mm_pages
  _mm_allocs = (mm_alloc_data_t *)((uint64_t)_mm_pages + _mm_page_count);

  MM_PRINT("_mm_pages=%#llx", _mm_pages);
  MM_PRINT("_mm_allocs=%#llx", _mm_allocs);

  MM_PRINT("setting bits and allocs");
  memset(_mm_pages, 0xFF, _mm_page_count);

  for (int i = 0; i < g_handoff->mmap_entries_length; i++) {
    bootproto_mmap_entry_t *entry = &g_handoff->mmap_entry[i];

    if (entry->type == free) {
      for (int i = 0; i < entry->pages; i++) {
        uint64_t pageStart = entry->start / 4096;
        _mm_clear_page_bit(pageStart + i);
      }
    }
    if (entry->type == used) {
      _mm_add_alloc(entry);
    }
  }

  // mark lower 16 MB of pages as used because god fuck
  for (uint64_t i = 0; i < 16 * 1024 * 1024 / 4096; i++) {
    _mm_set_page_bit(i);
  }

  // mark allocation tables as used
  MM_PRINT("marking allocation tables as used");
  uint64_t pageStart = (uint64_t)_mm_pages / 4096;
  for (int i = 0; i < total_needed_pages; i++) {
    _mm_set_page_bit(pageStart + i);
  }

  MM_PRINT("in the end we have %d allocations", _mm_alloc_count);
  spinlock_release(&_mm_spinlock);
}
