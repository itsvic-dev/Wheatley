#ifndef _BOOTPROTO_H
#define _BOOTPROTO_H

#include "stdint.h"

typedef enum {
  free,
  used,
  reserved,
} bootproto_mmap_entry_type_t;

typedef struct {
  // used for initialising kernel MM
  bootproto_mmap_entry_type_t type;
  uint64_t start;
  uint64_t pages;
} __attribute__((__packed__)) bootproto_mmap_entry_t;

typedef struct {
  char name[64];
  uint64_t addr;
} __attribute__((__packed__)) bootproto_symbol_t;

typedef struct {
 char signature[8];
 uint8_t checksum;
 char oemID[6];
 uint8_t revision;
 uint32_t rsdtAddress; // deprecated in ACPI 2.0
} __attribute__((packed)) rsdp_t;

typedef struct {
  bootproto_mmap_entry_t mmap_entry[128];
  uint8_t mmap_entries_length;

  bootproto_symbol_t symbols[256];
  int symbols_length;

  rsdp_t rsdp;

  // framebuffer data
  uint32_t *fb_buffer;
  uint16_t  fb_width;
  uint16_t  fb_height;
  uint32_t  fb_pixelsPerScanLine;
} __attribute__((__packed__)) bootproto_handoff_t;

#endif // _BOOTPROTO_H
