#ifndef _BOOTPROTO_H
#define _BOOTPROTO_H

#include "stdint.h"

typedef struct {
  // framebufer data
  uint32_t *fb_buffer;
  uint16_t  fb_width;
  uint16_t  fb_height;
  uint32_t   fb_pixelsPerScanLine;
} __attribute__((__packed__)) bootproto_handoff_t;

#endif // _BOOTPROTO_H
