#include "mm.h"
#include "internal.h"
#include "panic.h"

uint64_t _mm_find_page(uint64_t len) {
    for (int i = 0; i < _mm_page_count * 8; i++) {
        if (!_mm_get_page_bit(i)) {
            // looks promising, but to be 100% sure
            // that this will give us the space, loop again
            // MM_PRINT("kmalloc: page %d looks promising", i);
            bool isGood = true;
            for (int j = 1; j < len; j++) {
                if (_mm_get_page_bit(i + j)) {
                    isGood = false;
                    break;
                }
            }
            if (!isGood) {
                // MM_PRINT("kmalloc: nvm lol");
                continue;
            }
            // MM_PRINT("kmalloc: page %d is good", i);
            return i;
        }
    }

    MM_PRINT("kmalloc: couldn't find a fitting page, are we running out of memory?");
    return -1;
}

mm_alloc_data_t *_mm_find_alloc(uint32_t pageStart) {
    for (int i = 0; i < _mm_alloc_count; i++) {
        mm_alloc_data_t *entry = &_mm_allocs[i];
        if (entry->pageStart == pageStart) {
            return entry;
        }
    }

    return NULL;
}

void *kmalloc(uint64_t len) {
    // get amount of space needed
    uint64_t pagesNeeded = (len + 4095) / 4096;
    // MM_PRINT("kmalloc: need %d pages", pagesNeeded);

    // search the pages bitmap for free space
    // _mm_page_count was /8, so we need to *8
    uint64_t page = _mm_find_page(pagesNeeded);
    if (page == -1) return NULL;

    // search the allocs bitmap for an existing allocation
    mm_alloc_data_t *alloc = _mm_find_alloc(page);

    // if none, make one
    if (alloc == NULL) {
        alloc = &_mm_allocs[_mm_alloc_count++];
        alloc->pageStart = page;
    }

    // set page count and use flag
    alloc->pageCount = pagesNeeded;
    alloc->used = true;

    // lastly, set the page bits
    for (int i = 0; i < pagesNeeded; i++) {
        _mm_set_page_bit(page + i);
    }

    // and return the page address as a void *
    return (void *)(page * 4096);
}

void kfree(void *buf) {
    // find the allocation of buf
    uint64_t page = (uint64_t)buf / 4096;
    mm_alloc_data_t *alloc = _mm_find_alloc(page);

    // if none, bail
    if (alloc == NULL) {
        panic("kfree: failed to find alloc", 0);
    };

    // otherwise, set allocation and flag bits as unused
    alloc->used = false;
    for (int i = 0; i < alloc->pageCount; i++) {
        _mm_clear_page_bit(page + i);
    }

    MM_PRINT("kfree: done");
}
