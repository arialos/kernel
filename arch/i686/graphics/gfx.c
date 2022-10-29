#include "gfx.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "font.h"
#include "heap.h"
#include "libio.h"
#include "libmath.h"
#include "libstring.h"
#include "multiboot.h"

uint8_t *screenbuffer = NULL;
uint8_t *backbuffer   = NULL;

uint16_t fbWidth;
uint16_t fbHeight;
uint8_t fbBPP;
uint16_t fbStride;

inline void gfxDrawPixel(int x, int y, uint32_t col) { ((uint32_t *)backbuffer)[y * fbWidth + x] = col; }

// dx = x2 − x1
// dy = y2 − y1

// for x from x1 to x2 do
//     y = y1 + dy × (x − x1) / dx
//     plot(x, y)

void gfxDrawRect(int x, int y, int w, int h, uint32_t col) {
    for (int i = 0; i < h; i++) {
        // Instead of plotting pixel by pixel. Draw and fill rows of col to backbuffer

        // gfxDrawHorizontalLine(x, y + i, w, col);

        memset((void *)((uint32_t *)backbuffer + (y + i) * fbWidth + x), col, w * sizeof(uint32_t));
    }
}

void gfxSaveTempBuffer(int x, int y, int w, int h, uint32_t *tempBuffer) {
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            tempBuffer[i * w + j] = ((uint32_t *)backbuffer)[(i + y) * fbWidth + (j + x)];
        }
    }
}

void gfxRestoreTempBuffer(int x, int y, int w, int h, uint32_t *tempBuffer) {
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            ((uint32_t *)backbuffer)[(i + y) * fbWidth + (j + x)] = tempBuffer[i * w + j];
        }
    }
}

void gfxDrawCharacter(int x, int y, char c, uint32_t col) {
    uint8_t *line_addr    = backbuffer + (x * fbBPP) + (y * fbWidth * fbBPP);
    const uint16_t stride = fbWidth * fbBPP;
    const uint8_t stop_y  = MIN(fontHeight, fbHeight - y);
    const uint8_t stop_x  = MIN(fontWidth, fbWidth - x);
    if (c < 0 || c > 132) return;

    for (int i = 0; i < stop_y; ++i) {
        uint8_t mask_table[8] = {128, 64, 32, 16, 8, 4, 2, 1};
        for (int j = 0; j < stop_x; ++j) {
            if (systemFont[c][i] & mask_table[j]) ((uint32_t *)line_addr)[j] = col;
        }
        line_addr += stride;
    }
}

void gfxDrawCursor(int x, int y, uint32_t col) {
    uint8_t *line_addr    = backbuffer + (x * fbBPP) + (y * fbWidth * fbBPP);
    const uint16_t stride = fbWidth * fbBPP;
    const uint8_t stop_y  = MIN(fontHeight, fbHeight - y);
    const uint8_t stop_x  = MIN(fontWidth, fbWidth - x);

    for (int i = 0; i < stop_y; ++i) {
        uint8_t mask_table[8] = {128, 64, 32, 16, 8, 4, 2, 1};
        for (int j = 0; j < stop_x; ++j) {
            if (systemCursor[i] & mask_table[j]) ((uint32_t *)line_addr)[j] = col;
        }
        line_addr += stride;
    }
}

void gfxFramebufferClear(uint32_t col) {
    uint8_t *p    = backbuffer;
    uint8_t *stop = backbuffer + fbWidth * fbHeight * fbBPP;
    while (p < stop) {
        *(uint32_t *)p = col;
        p += fbBPP;
    }
}

void gfxPutCharacter(char ch) {
    // If the cursor is at the bottom of the screen, scroll up
    // by the height of a character
    if (cursorY + fontHeight >= fbHeight) {
        gfxFramebufferScroll(fontHeight);
        cursorY -= fontHeight;
    }

    if (ch == '\n' || cursorX + fontWidth >= fbWidth - cursorPaddingX) {
        cursorX = cursorPaddingX;
        cursorY += fontHeight;
        return;
    }

    gfxDrawCharacter(cursorX, cursorY, ch, fgColor);
    cursorX += fontWidth;
}

void gfxFramebufferScroll(uint16_t lines) {
    uint8_t *p    = backbuffer;
    uint8_t *stop = backbuffer + fbWidth * fbHeight * fbBPP;
    while (p < stop) {
        *(uint32_t *)p = *(uint32_t *)(p + lines * fbWidth * fbBPP);
        p += fbBPP;
    }
}

void gfxPutStringAt(int x, int y, const char *str) {
    cursorX = x;
    cursorY = y;
    while (*str) {
        gfxPutCharacter(*str++);
    }
    cursorX = 0;
    cursorY = 0;
}

void gfxPutString(const char *str) {
    while (*str) {
        gfxPutCharacter(*str++);
    }
}

void gfxSwapBuffers() { memcpy(screenbuffer, backbuffer, fbHeight * fbStride); }

bool initGFX(multiboot_info_t *mbi) {
    screenbuffer = (uint8_t *)mbi->framebuffer_addr;

    // We need to allocate a screenbuffer for double buffering

    fbWidth  = mbi->framebuffer_width;
    fbHeight = mbi->framebuffer_height;
    fbBPP    = mbi->framebuffer_bpp / 8;
    fbStride = mbi->framebuffer_pitch;

    backbuffer = kmalloc(fbHeight * fbStride);

    if (fbBPP != 4) return false;

    // Clear the buffer and set everything black
    gfxFramebufferClear(gfxColor(0x00, 0x6E, 0x8D));

    cursorX = cursorPaddingX;
    cursorY = cursorPaddingY;

    fgColor = gfxColor(0xFF, 0xFF, 0xFF);
    bgColor = gfxColor(0x00, 0x6E, 0x8D);

    return true;
}