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
  uint_64b phdr_table;
  uint_64b shdr_table;

  uint_32b flags; // typically 0 on i386/x86-64
  uint_16b header_size;

  uint_16b phdr_entry_size;
  uint_16b phdr_entries_num;

  uint_16b shdr_entry_size;
  uint_16b shdr_entries_num;
  uint_16b shdr_section_names_idx;
} __attribute__((__packed__)) elf_header_t;

typedef enum elf_segment_type : uint_32b {
  elf_segt_null = 0,
  elf_segt_load = 1,
  elf_segt_dynamic = 2,
  elf_segt_interp = 3,
  elf_segt_note = 4
} elf_segment_type_t;

typedef enum elf_segment_flags : uint_32b {
  executable = 1 << 0,
  writable = 1 << 1,
  readable = 1 << 2,
} elf_segment_flags_t;

typedef struct elf_phdr {
  elf_segment_type_t type;
  elf_segment_flags_t flags;
  uint_64b offset;
  uint_64b vaddr;
  uint_64b undefined;
  uint_64b filesz;
  uint_64b memsz;
  uint_64b alignment;
} __attribute__((__packed__)) elf_phdr_t;

#endif // __BL_ELF_H
