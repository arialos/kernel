#include "gfx.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "font.h"
#include "heap.h"
#include "libio.h"
#include "libmath.h"
#include "multiboot.h"

uint32_t *video_memory = NULL;
uint32_t *framebuffer  = NULL;

bool fSynced = true;

uint16_t fbWidth, fbHeight, fbStride;
uint8_t fbBPP;

inline void gfxDrawPixel(int x, int y, uint32_t col) { ((uint32_t *)framebuffer)[y * fbWidth + x] = col; }

inline void gfxDrawHLine(int x, int y, int w, uint32_t col) {
    for (int i = 0; i < w; i++) {
        ((uint32_t *)framebuffer)[y * fbWidth + x + i] = col;
    }
}
inline void gfxDrawVLine(int x, int y, int h, uint32_t col) {
    for (int i = 0; i < h; i++) {
        ((uint32_t *)framebuffer)[(y + i) * fbWidth + x] = col;
    }
}
void gfxDrawLine(int x1, int y1, int x2, int y2, uint32_t col) {}

void gfxDrawRect(int x, int y, int w, int h, uint32_t col) {
    uint32_t *where = &framebuffer[x + y * fbWidth];

    for (int i = 0; i < MIN(fbHeight - y, h); i++) {
        for (int j = 0; j < MIN(fbWidth - x, w); j++) {
            *where++ = col;
        }
        where += fbWidth - MIN(fbWidth - x, w);
    }
}

void gfxDrawLegacyBitmap(int x, int y, uint8_t bitmap[], uint32_t col) {
    uint32_t *where = &framebuffer[x + y * fbWidth];

    for (int i = 0; i < MIN(fbHeight - y, fontHeight); ++i) {
        for (int j = 0; j < MIN(fbWidth - x, fontWidth); ++j) {
            *where++ = bitmap[i] & maskTable[j] ? col : *where;
        }
        where += fbWidth - MIN(fbWidth - x, fontWidth);
    }
}

void gfxDrawBuffer(int x, int y, int w, int h, uint32_t *buffer) {
    uint32_t *where = &framebuffer[x + y * fbWidth];

    for (int i = 0; i < MIN(fbHeight - y, h); i++) {
        for (int j = 0; j < MIN(fbWidth - x, w); j++) {
            *where++ = *buffer++;
        }
        where += fbWidth - MIN(fbWidth - x, w);
    }
}

void gfxSwapBuffers(void) {
    fSynced = false;
    for (int i = 0; i < fbWidth * fbHeight; i++) {
        if (framebuffer[i] != video_memory[i]) {
            video_memory[i] = framebuffer[i];
        }
    }
    fSynced = true;
}

void gfxClearBuffer(uint32_t col) { gfxDrawRect(0, 0, fbWidth, fbHeight, col); }

int initGraphics(multiboot_info_t *mbi) {
    if (mbi->framebuffer_bpp != 32) return 1;

    fbWidth  = mbi->framebuffer_width;
    fbHeight = mbi->framebuffer_height;
    fbBPP    = mbi->framebuffer_bpp / 8;
    fbStride = mbi->framebuffer_pitch;

    video_memory = (uint8_t *)mbi->framebuffer_addr;
    framebuffer  = kmalloc(fbHeight * fbStride);

    // gfxClearBuffer(gfxColor(0x00, 0x00, 0xFF));
    // gfxSwapBuffers();

    return 0;
}