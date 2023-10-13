#ifndef __BL_ELF_H
#define __BL_ELF_H

#include "helpers.h"

typedef struct elf_header {
  uint8_t magic[4];  // \x7fELF
  uint8_t arch_bits; // 2 for 64-bit
  uint8_t arch_mode; // 1 for LE, 2 for BE
  uint8_t header_version;
  uint8_t abi;
  uint8_t padding[8];

  uint16_t prog_type;       // 2 - executable
  uint16_t instruction_set; // 0x3E - x86-64

  uint32_t elf_version;
  void *entry;
  uint64_t phdr_table;
  uint64_t shdr_table;

  uint32_t flags; // typically 0 on i386/x86-64
  uint16_t header_size;

  uint16_t phdr_entry_size;
  uint16_t phdr_entries_num;

  uint16_t shdr_entry_size;
  uint16_t shdr_entries_num;
  uint16_t shdr_section_names_idx;
} __attribute__((__packed__)) elf_header_t;

typedef enum elf_segment_type : uint32_t {
  elf_segt_null = 0,
  elf_segt_load = 1,
  elf_segt_dynamic = 2,
  elf_segt_interp = 3,
  elf_segt_note = 4
} elf_segment_type_t;

typedef enum elf_segment_flags : uint32_t {
  executable = 1 << 0,
  writable = 1 << 1,
  readable = 1 << 2,
} elf_segment_flags_t;

typedef struct elf_phdr {
  elf_segment_type_t type;
  elf_segment_flags_t flags;
  uint64_t offset;
  uint64_t vaddr;
  uint64_t undefined;
  uint64_t filesz;
  uint64_t memsz;
  uint64_t alignment;
} __attribute__((__packed__)) elf_phdr_t;

#endif // __BL_ELF_H
