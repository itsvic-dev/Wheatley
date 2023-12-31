#ifndef __KERNEL_DRIVERS_TTY_FBTTY_H
#define __KERNEL_DRIVERS_TTY_FBTTY_H

#include "stdint.h"

#define PSF_FONT_MAGIC 0x864ab572

typedef struct {
  uint32_t magic;         /* magic bytes to identify PSF */
  uint32_t version;       /* zero */
  uint32_t headersize;    /* offset of bitmaps in file, 32 */
  uint32_t flags;         /* 0 if there's no unicode table */
  uint32_t numglyph;      /* number of glyphs */
  uint32_t bytesperglyph; /* size of each glyph */
  uint32_t height;        /* height in pixels */
  uint32_t width;         /* width in pixels */
} psf_font_t;

void fbtty_module_init();

#endif // __KERNEL_DRIVERS_TTY_FBTTY_H
