#ifndef __BOOTLOADER_BMP3_H
#define __BOOTLOADER_BMP3_H

#include "protocol/efi-gop.h"
#include <stdint.h>

typedef struct {
  uint16_t type; // BM (0x4d42)
  uint32_t size;
  uint16_t reserved1;
  uint16_t reserved2;
  uint32_t offBits; // offset into the pixel data section
} __attribute__((__packed__)) bmp3_fhdr_t;

typedef struct {
  uint32_t size;
  int32_t width;
  int32_t height;
  uint16_t planes;
  uint16_t bitCount;
  uint32_t compression;
  uint32_t imageSize;    // may be 0 for uncompressed images
  uint32_t xPPM;         // preferred resolution in pixels/m
  uint32_t yPPM;         // preferred resolution in pixels/m
  uint32_t clrUsed;      // number of color map entries that are used
  uint32_t clrImportant; // number of significant colors
} __attribute__((__packed__)) bmp3_ihdr_t;

void bmp3_display(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop);

#endif // __BOOTLOADER_BMP3_H
