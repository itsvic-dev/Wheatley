#include "paging.h"
#include <efi.h>

extern EFI_BOOT_SERVICES *BS;

static uint64_t *Paging_GetNextLevel(uint64_t *currLevel, uint64_t entry) {
  uint64_t *ret = NULL;

  if (currLevel[entry] & 1) {
    // if the page exists, just return it
    ret = (uint64_t *)(currLevel[entry] & ~((uint64_t)0xfff));
  } else {
    // create the page
    BS->AllocatePages(AllocateAnyPages, EfiLoaderData, 1, (uint64_t *)&ret);
    BS->SetMem(ret, 4096, 0);
    currLevel[entry] = (uint64_t)ret | 0b111;
  }

  return ret;
}

bool Paging_MapPage(uint64_t *pagemap, uint64_t physAddr, uint64_t virtAddr,
                    uint64_t flags) {
  // thanks nsg for the help

  if (pagemap == NULL)
    return false;

  uint64_t level4_entry = (virtAddr & ((uint64_t)0x1ff << 39)) >> 39;
  uint64_t level3_entry = (virtAddr & ((uint64_t)0x1ff << 30)) >> 30;
  uint64_t level2_entry = (virtAddr & ((uint64_t)0x1ff << 21)) >> 21;
  uint64_t level1_entry = (virtAddr & ((uint64_t)0x1ff << 12)) >> 12;

  uint64_t *level4, *level3, *level2, *level1;

  level4 = pagemap;
  level3 = Paging_GetNextLevel(level4, level4_entry);
  if (!level3)
    return false;
  level2 = Paging_GetNextLevel(level3, level3_entry);
  if (!level2)
    return false;
  level1 = Paging_GetNextLevel(level2, level2_entry);
  if (!level1)
    return false;

  level1[level1_entry] = physAddr | flags;
  return true;
}
