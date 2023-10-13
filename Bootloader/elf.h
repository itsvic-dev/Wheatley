#ifndef __BL_ELF_H
#define __BL_ELF_H

#include "helpers.h"

typedef struct elf_header {
  uint_8b magic[4];  // \x7fELF
  uint_8b arch_bits; // 2 for 64-bit
  uint_8b arch_mode; // 1 for LE, 2 for BE
  uint_8b header_version;
  uint_8b abi;
  uint_8b padding[8];

  uint_16b prog_type;       // 2 - executable
  uint_16b instruction_set; // 0x3E - x86-64

  uint_32b elf_version;
  void *entry;
  void *phdr_table;
  void *shdr_table;

  uint_32b flags; // typically 0 on i386/x86-64
  uint_16b header_size;

  uint_16b phdr_entry_size;
  uint_16b phdr_entries_num;

  uint_16b shdr_entry_size;
  uint_16b shdr_entries_num;
  uint_16b shdr_section_names_idx;
} __attribute__((__packed__)) elf_header_t;

#endif // __BL_ELF_H
