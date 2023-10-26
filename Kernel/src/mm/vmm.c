#include <libk.h>
#include <mm/mm.h>
#include <mm/vmm.h>
#include <sys/io.h>

uint64_t *vmm_get_current_pagemap() { return (uint64_t *)__readcr3(); }

static uint64_t *vmm_get_next_level(uint64_t *currLevel, uint64_t entry,
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

bool vmm_map_page(uint64_t *pagemap, uint64_t physAddr, uint64_t virtAddr,
                  uint64_t flags) {
  if (pagemap == NULL)
    return false;

  uint64_t level4_entry = (virtAddr & ((uint64_t)0x1ff << 39)) >> 39;
  uint64_t level3_entry = (virtAddr & ((uint64_t)0x1ff << 30)) >> 30;
  uint64_t level2_entry = (virtAddr & ((uint64_t)0x1ff << 21)) >> 21;
  uint64_t level1_entry = (virtAddr & ((uint64_t)0x1ff << 12)) >> 12;

  uint64_t *level4, *level3, *level2, *level1;

  level4 = pagemap;
  level3 = vmm_get_next_level(level4, level4_entry, true);
  if (!level3)
    return false;
  level2 = vmm_get_next_level(level3, level3_entry, true);
  if (!level2)
    return false;
  level1 = vmm_get_next_level(level2, level2_entry, true);
  if (!level1)
    return false;

  level1[level1_entry] = physAddr | flags;
  return true;
}

bool vmm_unmap_page(uint64_t *pagemap, uint64_t virtAddr) {
  if (pagemap == NULL)
    return false;

  uint64_t level4_entry = (virtAddr & ((uint64_t)0x1ff << 39)) >> 39;
  uint64_t level3_entry = (virtAddr & ((uint64_t)0x1ff << 30)) >> 30;
  uint64_t level2_entry = (virtAddr & ((uint64_t)0x1ff << 21)) >> 21;
  uint64_t level1_entry = (virtAddr & ((uint64_t)0x1ff << 12)) >> 12;

  uint64_t *level4, *level3, *level2, *level1;

  level4 = pagemap;
  level3 = vmm_get_next_level(level4, level4_entry, false);
  if (!level3)
    return false;
  level2 = vmm_get_next_level(level3, level3_entry, false);
  if (!level2)
    return false;
  level1 = vmm_get_next_level(level2, level2_entry, false);
  if (!level1)
    return false;

  level1[level1_entry] = 0;
  return true;
}
