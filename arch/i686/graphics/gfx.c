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
gfxBuffer framebuffer;

bool fSynced = true;

uint16_t fbWidth, fbHeight, fbStride;
uint8_t fbBPP;

inline void gfxDrawPixel(int x, int y, gfxBuffer buffer, uint32_t col) {
    ((uint32_t *)buffer.buffer)[y * buffer.width + x] = col;
}

inline void gfxDrawHLine(int x, int y, int w, gfxBuffer buffer, uint32_t col) {
    for (int i = 0; i < w; i++) {
        ((uint32_t *)buffer.buffer)[y * buffer.width + x + i] = col;
    }
}
inline void gfxDrawVLine(int x, int y, int h, gfxBuffer buffer, uint32_t col) {
    for (int i = 0; i < h; i++) {
        ((uint32_t *)buffer.buffer)[(y + i) * buffer.width + x] = col;
    }
}
void gfxDrawLine(int x1, int y1, int x2, int y2, gfxBuffer buffer, uint32_t col) {}

void gfxDrawRect(int x, int y, int w, int h, gfxBuffer buffer, uint32_t col) {
    uint32_t *where = &buffer.buffer[x + y * buffer.width];

    for (int i = 0; i < MIN(buffer.height - y, h); i++) {
        for (int j = 0; j < MIN(buffer.width - x, w); j++) {
            *where++ = col;
        }
        where += buffer.width - MIN(buffer.width - x, w);
    }
}

void gfxDrawLegacyBitmap(int x, int y, uint8_t *bitmap, gfxBuffer buffer, uint32_t col) {
    uint32_t *where = &buffer.buffer[x + y * buffer.width];

    for (int i = 0; i < MIN(buffer.height - y, fontHeight); ++i) {
        for (int j = 0; j < MIN(buffer.width - x, fontWidth); ++j) {
            *where++ = bitmap[i] & maskTable[j] ? col : *where;
        }
        where += buffer.width - MIN(buffer.width - x, fontWidth);
    }
}

int cursor_x, cursor_y;

void gfxDrawCharacter(int x, int y, char ch, gfxBuffer buffer, uint32_t col) {
    // If the cursor is at the bottom of the screen, scroll up
    // by the height of a character
    if (cursor_y + fontHeight >= fbHeight) cursor_y = y;

    if (ch == '\n' || cursor_x + fontWidth >= buffer.width) {
        cursor_x = x;
        cursor_y += fontHeight;
        return;
    }

    gfxDrawLegacyBitmap(cursor_x, cursor_y, systemFont[ch], buffer, col);
    cursor_x += fontWidth;
}

void gfxDrawString(int x, int y, char *str, gfxBuffer buffer, uint32_t col) {
    cursor_x = x;
    cursor_y = y;

    while (*str) {
        gfxDrawCharacter(x, y, *str++, buffer, col);
    }
}

void gfxDrawBuffer(int x, int y, gfxBuffer src, gfxBuffer dest) {
    uint32_t *where = &dest.buffer[x + y * dest.width];

    for (int i = 0; i < MIN(dest.height - y, src.height); i++) {
        for (int j = 0; j < MIN(dest.width - x, src.width); j++) {
            *where++ = src.buffer[i * src.width + j];
        }
        where += dest.width - MIN(dest.width - x, src.width);
    }
}

void gfxSwapBuffers(void) {
    fSynced = false;
    for (int i = 0; i < framebuffer.width * framebuffer.height; i++) {
        if (framebuffer.buffer[i] != video_memory[i]) {
            video_memory[i] = framebuffer.buffer[i];
        }
    }
    fSynced = true;
}

void gfxClearBuffer(gfxBuffer buffer, uint32_t col) {
    for (int i = 0; i < buffer.width * buffer.height; i++) {
        if (buffer.buffer[i] != col) buffer.buffer[i] = col;
    }
}

int initGraphics(MultibootInfo *mbi) {
    if (mbi->framebuffer_bpp != 32) return 1;

    fbWidth  = mbi->framebuffer_width;
    fbHeight = mbi->framebuffer_height;
    fbBPP    = mbi->framebuffer_bpp / 8;
    fbStride = mbi->framebuffer_pitch;

    video_memory = (uint8_t *)mbi->framebuffer_addr;

    framebuffer.buffer = kmalloc(fbHeight * fbStride);
    framebuffer.height = fbHeight;
    framebuffer.width  = fbWidth;

    return 0;
}