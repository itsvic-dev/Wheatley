#include <drivers/tty/fbtty.h>
#include <drivers/tty/tty.h>
#include <drivers/fb/fb.h>
#include <printf.h>
#include <mm/mm.h>

extern char _binary_Tamsyn8x16r_psf_start;
extern char _binary_Tamsyn8x16r_psf_end;

psf_font_t *_fbtty_font;

// FIXME: support for multiple FBs.
// will need kmalloc
fb_driver_t *_fbtty_fb;
int _fbtty_cx = 0;
int _fbtty_cy = 0;

int _fbtty_fbw = 0;
int _fbtty_fbh = 0;

// FIXME: this should be dynamically allocated
uint64_t _fbtty_scrollbackBuf[1920 * 1080];

static inline uint8_t shifted(int i, int j) {
    return (i >> (7 - j)) & 1;
}

void fbtty_putchar(char c) {
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
            _fbtty_fb->setpixel(realX + i, realY + j, shifted(glyphData[j], i) ? 0xffffff : 0);
        }
    }

    _fbtty_cx++;

check_space:
    if (_fbtty_cx * _fbtty_font->width >= _fbtty_fbw) {
        _fbtty_cy++;
        _fbtty_cx = 0;
    }

    if (_fbtty_cy * _fbtty_font->height >= _fbtty_fbh) {
        // FIXME: add double-buffering maybe
        uint64_t offset = _fbtty_fbw * _fbtty_font->height;
        uint64_t count = _fbtty_fbw * (_fbtty_fbh - _fbtty_font->height) * 8;
        _fbtty_fb->readpixels(_fbtty_scrollbackBuf, offset, count);
        _fbtty_fb->memcpy(_fbtty_scrollbackBuf, 0, count);
        _fbtty_cy--;
    }
}

tty_driver_t fbtty_driver;

void fbtty_module_init() {
    _fbtty_font = (psf_font_t *)&_binary_Tamsyn8x16r_psf_start;

    if (_fbtty_font->magic != PSF_FONT_MAGIC) {
        printf("fbtty: psf: expected %#x, got %#x\n", PSF_FONT_MAGIC, _fbtty_font->magic);
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
    // _fbtty_scrollbackBuf = kmalloc(_fbtty_fbw * (_fbtty_fbh - _fbtty_font->height) * 8);

    tty_register_driver(&fbtty_driver);
}
