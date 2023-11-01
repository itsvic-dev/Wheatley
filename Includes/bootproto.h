#ifndef _BOOTPROTO_H
#define _BOOTPROTO_H

#include "stdint.h"

#define PACKED __attribute__((__packed__))

typedef enum {
  free,
  used,
  reserved,
} bootproto_pmm_entry_type_t;

typedef struct {
  // used for initialising kernel MM
  bootproto_pmm_entry_type_t type;
  uint64_t start;
  uint64_t pages;
} PACKED bootproto_pmm_entry_t;

typedef enum {
  RO,
  RW,
  RX,
} bootproto_vmm_entry_type_t;

typedef struct {
  // used for initialising kernel VMM
  bootproto_vmm_entry_type_t type;
  uint64_t physAddr;
  uint64_t virtAddr;
  uint64_t pages;
} PACKED bootproto_vmm_entry_t;

typedef struct {
  char name[64];
  uint64_t addr;
} PACKED bootproto_symbol_t;

typedef struct {
  char signature[8];
  uint8_t checksum;
  char oemID[6];
  uint8_t revision;
  uint32_t rsdtAddress; // deprecated in ACPI 2.0

  uint32_t length;
  uint64_t xsdtAddress;
  uint8_t extendedChecksum;
  uint8_t reserved[3];
} PACKED xsdp_t;

typedef struct {
  bootproto_pmm_entry_t pmm_entry[128];
  uint8_t pmm_entries_length;

  bootproto_vmm_entry_t vmm_entry[6];
  uint8_t vmm_entries_length;

  bootproto_symbol_t symbols[256];
  int symbols_length;

  xsdp_t rsdp;

  // framebuffer data
  uint32_t *fb_buffer;
  uint16_t fb_width;
  uint16_t fb_height;
  uint32_t fb_pixelsPerScanLine;
} PACKED bootproto_handoff_t;

#endif // _BOOTPROTO_H
