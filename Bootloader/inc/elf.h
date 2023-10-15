#ifndef __BL_ELF_H
#define __BL_ELF_H

#include "helpers.h"

#define ELF_MAGIC 0x464C457F

typedef uint64_t Elf64_Addr;
typedef uint64_t Elf64_Off;
typedef uint16_t Elf64_Half;
typedef uint32_t Elf64_Word;
typedef int32_t Elf64_Sword;
typedef uint64_t Elf64_Xword;
typedef int64_t Elf64_Sxword;

typedef struct elf_header {
  uint32_t magic;
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

  uint32_t flags;
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

typedef enum elf_section_type : Elf64_Word {
  SHT_NULL,
  SHT_PROGBITS,
  SHT_SYMTAB,
  SHT_STRTAB,
  SHT_RELA,
  SHT_HASH,
  SHT_DYNAMIC,
  SHT_NOTE,
  SHT_NOBITS,
  SHT_REL,
  SHT_SHLIB,
  SHT_DYNSYM,

  SHT_LOPROC = 0x70000000,
  SHT_HIPROC = 0x7fffffff,
  SHT_LOUSER = 0x80000000,
  SHT_HIUSER = 0xffffffff
} elf_section_type_t;

typedef struct {
  Elf64_Word name;
  elf_section_type_t type;
  Elf64_Xword flags;
  Elf64_Addr addr;
  Elf64_Off offset;
  Elf64_Xword size;
  Elf64_Word link;
  Elf64_Word info;
  Elf64_Xword addralign;
  Elf64_Xword entsize;
} __attribute__((__packed__)) elf_shdr_t;

typedef struct {
  Elf64_Word name;
  uint8_t info;
  uint8_t other;
  Elf64_Half shndx;
  Elf64_Addr value;
  Elf64_Xword size;
} __attribute__((__packed__)) elf_symbol_t;

#define ELF_SYMBOL_TYPE(i) ((i) & 0xf)
#define ELF_SYMBOL_BIND(i) ((i) >> 4)

typedef enum {
  STT_NOTYPE,
  STT_OBJECT,
  STT_FUNC,
  STT_SECTION,
  STT_FILE,
  STT_LOPROC = 13,
  STT_HIPROC = 15
} elf_symbol_type_t;

#endif // __BL_ELF_H
