#include "bootproto.h"
#include "sys/spinlock.h"
#include <libk.h>
#include <mm/mm.h>
#include <mm/vmm.h>
#include <printf.h>
#include <stdint.h>
#include <sys/io.h>

static pagemap_t kernel_pagemap = {0, SPINLOCK_INIT};

pagemap_t *vmm_get_kernel_pagemap() { return &kernel_pagemap; }

static uint64_t *get_next_level(uint64_t *currLevel, uint64_t entry,
                                bool doAlloc) {
  uint64_t *ret = NULL;

  if (currLevel[entry] & 1) {
    // if the page exists, just return it
    ret = (uint64_t *)(currLevel[entry] & ~((uint64_t)0xfff));
  } else if (doAlloc) {
    // create the page
    ret = kmalloc(4096);
    memset(ret, 0, 4096);
    currLevel[entry] = (uint64_t)ret | 0b111;
  } else {
    return false;
  }

  return ret;
}

bool vmm_map_page(pagemap_t *pagemap, uint64_t physAddr, uint64_t virtAddr,
                  uint64_t flags) {
  if (pagemap == NULL)
    return false;

  bool ret = false;
  spinlock_wait_and_acquire(&pagemap->lock);

  uint64_t level4_entry = (virtAddr & ((uint64_t)0x1ff << 39)) >> 39;
  uint64_t level3_entry = (virtAddr & ((uint64_t)0x1ff << 30)) >> 30;
  uint64_t level2_entry = (virtAddr & ((uint64_t)0x1ff << 21)) >> 21;
  uint64_t level1_entry = (virtAddr & ((uint64_t)0x1ff << 12)) >> 12;

  uint64_t *level4, *level3, *level2, *level1;

  level4 = pagemap->topLevel;
  level3 = get_next_level(level4, level4_entry, true);
  if (!level3)
    goto end;
  level2 = get_next_level(level3, level3_entry, true);
  if (!level2)
    goto end;
  level1 = get_next_level(level2, level2_entry, true);
  if (!level1)
    goto end;

  level1[level1_entry] = physAddr | flags;
  ret = true;
end:
  spinlock_release(&pagemap->lock);
  return ret;
}

bool vmm_unmap_page(pagemap_t *pagemap, uint64_t virtAddr) {
  if (pagemap == NULL)
    return false;

  bool ret = false;
  spinlock_wait_and_acquire(&pagemap->lock);

  uint64_t level4_entry = (virtAddr & ((uint64_t)0x1ff << 39)) >> 39;
  uint64_t level3_entry = (virtAddr & ((uint64_t)0x1ff << 30)) >> 30;
  uint64_t level2_entry = (virtAddr & ((uint64_t)0x1ff << 21)) >> 21;
  uint64_t level1_entry = (virtAddr & ((uint64_t)0x1ff << 12)) >> 12;

  uint64_t *level4, *level3, *level2, *level1;

  level4 = pagemap->topLevel;
  level3 = get_next_level(level4, level4_entry, false);
  if (!level3)
    goto end;
  level2 = get_next_level(level3, level3_entry, false);
  if (!level2)
    goto end;
  level1 = get_next_level(level2, level2_entry, false);
  if (!level1)
    goto end;

  level1[level1_entry] = 0;
  ret = true;
end:
  spinlock_release(&pagemap->lock);
  return ret;
}

void vmm_init(void) {
  kernel_pagemap.topLevel = kmalloc(4096);
  printf("vmm: pagemap topLevel @ %#llx\n", kernel_pagemap.topLevel);
  memset(kernel_pagemap.topLevel, 0, 4096);

  // map kernel sections from physical mem to virtual mem
  for (int i = 0; i < g_handoff->vmm_entries_length; i++) {
    bootproto_vmm_entry_t *entry = &g_handoff->vmm_entry[i];
    printf("vmm: mapping kernel section %d: %#llx -> %#llx\n", i,
           entry->physAddr, entry->virtAddr);
    for (int j = 0; j < entry->pages; j++) {
      uint64_t offset = j * 4096;
      // FIXME: map flags appropriately
      vmm_map_page(&kernel_pagemap, entry->physAddr + offset,
                   entry->virtAddr + offset, 0b11);
    }
  }

  // identity map EFI memory sections
  printf("vmm: mapping EFI sections\n");
  for (int i = 0; i < g_handoff->pmm_entries_length; i++) {
    bootproto_pmm_entry_t *entry = &g_handoff->pmm_entry[i];
    for (int j = 0; j < entry->pages; j++) {
      uint64_t offset = j * 4096;
      vmm_map_page(&kernel_pagemap, entry->start + offset,
                   entry->start + offset, 0b11);
    }
  }

  // identity map 256 MB of memory (should include most of ACPI)
  for (int i = 0; i < 0x10000; i++) {
    uint64_t addr = i * 4096;
    vmm_map_page(&kernel_pagemap, addr, addr, 0b11);
  }

  printf("vmm: switching pagemaps now\n");
  __writecr3((uint64_t)kernel_pagemap.topLevel);
  printf("vmm: we survived the page switch!\n");
}
