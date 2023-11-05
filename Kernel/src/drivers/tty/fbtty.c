#include <drivers/fb/fb.h>
#include <drivers/tty/fbtty.h>
#include <drivers/tty/tty.h>
#include <mm/mm.h>
#include <printf.h>
#include <sys/spinlock.h>

#define BG_COLOR (0x180a0a)
#define FG_COLOR (0xf49445)

static spinlock_t spinlock = SPINLOCK_INIT;

extern char _binary_Tamsyn8x16r_psf_start;
extern char _binary_Tamsyn8x16r_psf_end;

psf_font_t *_fbtty_font;

// TODO: support for multiple FBs
fb_driver_t *_fbtty_fb;
int _fbtty_cx = 0;
int _fbtty_cy = 0;

int _fbtty_fbw = 0;
int _fbtty_fbh = 0;

uint32_t *_fbtty_scrollbackBuf;

static inline uint8_t shifted(int i, int j) { return (i >> (7 - j)) & 1; }

void fbtty_putchar(char c) {
  spinlock_wait_and_acquire(&spinlock);
  int realX = _fbtty_cx * _fbtty_font->width;
  int realY = _fbtty_cy * _fbtty_font->height;

  int offset = c * _fbtty_font->bytesperglyph + _fbtty_font->headersize;

  uint8_t *glyphData = &((uint8_t *)(_fbtty_font))[offset];

  int i, j;

  if (c == '\n') {
    _fbtty_cy++;
    _fbtty_cx = 0;
    goto check_space;
  }

  for (j = 0; j < _fbtty_font->height; j++) {
    for (i = 0; i < _fbtty_font->width; i++) {
      // free optimization: setpixel only if the glyph bit is actually set
      if (shifted(glyphData[j], i))
        _fbtty_fb->setpixel(realX + i, realY + j, FG_COLOR);
    }
  }

  _fbtty_cx++;

  // FIXME: on some resolutions, the current X and Y can sometimes overflow into
  // out of bounds space. thus, we're being super paranoid by checking the pixel
  // in front of us, instead of ourselves. we really shouldn't have to do this,
  // but i have no idea why it happens.
  if ((_fbtty_cx + 1) * _fbtty_font->width >= _fbtty_fbw) {
    _fbtty_cy++;
    _fbtty_cx = 0;
  }

check_space:
  if ((_fbtty_cy + 1) * _fbtty_font->height >= _fbtty_fbh) {
    uint64_t offset = _fbtty_fbw * _fbtty_font->height;
    uint64_t count = _fbtty_fbw * (_fbtty_fbh - _fbtty_font->height) * 4;
    _fbtty_fb->readpixels(_fbtty_scrollbackBuf, offset, count);
    _fbtty_fb->memcpy(_fbtty_scrollbackBuf, 0, count);
    _fbtty_fb->pixset(BG_COLOR, count / 4, offset);
    _fbtty_cy--;
  }

  spinlock_release(&spinlock);
}

tty_driver_t fbtty_driver;

void fbtty_module_init() {
  _fbtty_font = (psf_font_t *)&_binary_Tamsyn8x16r_psf_start;

  if (_fbtty_font->magic != PSF_FONT_MAGIC) {
    printf("fbtty: psf: expected %#x, got %#x\n", PSF_FONT_MAGIC,
           _fbtty_font->magic);
    return;
  }

  if (_fbtty_font->flags) {
    printf("fbtty: psf: unicode support doesn't exist\n");
  }

  if (_fbtty_font->width != 8) {
    printf("fbtty: psf: fonts with a width of not 8 are likely to be broken\n");
  }

  // _fbtty_font loaded, get in the fb
  _fbtty_fb = fb_get_driver(0);
  if (!_fbtty_fb) {
    printf("fbtty: fb0 does not exist\n");
    return;
  }

  fbtty_driver.putchar = &fbtty_putchar;
  _fbtty_fbw = _fbtty_fb->get_info()->width;
  _fbtty_fbh = _fbtty_fb->get_info()->height;
  _fbtty_scrollbackBuf =
      kmalloc(_fbtty_fbw * (_fbtty_fbh - _fbtty_font->height) * 4);

  _fbtty_fb->pixset(BG_COLOR, 0, _fbtty_fbw * _fbtty_fbh);

  tty_register_driver(&fbtty_driver);
}
