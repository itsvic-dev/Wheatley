// parses the BMP and puts it on screen
#include "protocol/efi-gop.h"
#include <assert.h>
#include <bmp3.h>
#include <efi-bs.h>
#include <efi.h>
#include <printf.h>

extern char _binary_logo_bmp_start;

extern EFI_BOOT_SERVICES *BS;

typedef struct {
  uint8_t b;
  uint8_t g;
  uint8_t r;
} __attribute__((__packed__)) rgb_t;

static inline int fuckInt(int x) {
  if (x % 4 == 0)
    return x;
  return x + (4 - (x % 4));
}

void bmp3_display(EFI_GRAPHICS_OUTPUT_PROTOCOL *gop) {
  bmp3_fhdr_t *fhdr = (bmp3_fhdr_t *)&_binary_logo_bmp_start;
  ASSERT(fhdr->type == 0x4d42);
  ASSERT(fhdr->offBits == 0x36);
  bmp3_ihdr_t *ihdr = (bmp3_ihdr_t *)((uint64_t)fhdr + sizeof(bmp3_fhdr_t));
  ASSERT(ihdr->bitCount == 24);

  // allocate buffer for blt buffer
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *bltBuf;
  BS->AllocatePool(EfiLoaderData,
                   ihdr->width * ihdr->height *
                       sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL),
                   (void **)&bltBuf);

  rgb_t *pixData = (rgb_t *)((uint64_t)fhdr + fhdr->offBits);

  uint32_t fuckedWidth = fuckInt(ihdr->width * 3);

  for (int j = 0; j < ihdr->height; j++) {
    for (int i = 0; i < ihdr->width; i++) {
      int bltIdx = j * ihdr->width + i;
      int bmpIdx = (ihdr->height - j - 1) * fuckedWidth + i * 3;
      rgb_t *pix = (rgb_t *)((uint64_t)pixData + bmpIdx);
      bltBuf[bltIdx].RED = pix->r;
      bltBuf[bltIdx].GREEN = pix->g;
      bltBuf[bltIdx].BLUE = pix->b;
    }
  }

  // display it in the bottom-right corner of the screen
  uint32_t xOff = gop->Mode->Info->HorizontalResolution - ihdr->width;
  uint32_t yOff = gop->Mode->Info->VerticalResolution - ihdr->height;

  gop->Blt(gop, (void *)bltBuf, EfiBltBufferToVideo, 0, 0, xOff, yOff,
           ihdr->width, ihdr->height, 0);

  BS->FreePool(bltBuf);
}
