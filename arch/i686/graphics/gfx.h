#ifndef GFX_H
#define GFX_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "multiboot.h"

typedef struct gfxBuffer {
    uint32_t *buffer;
    uint16_t width, height;
} gfxBuffer;

extern bool fSynced;

extern gfxBuffer framebuffer;

extern uint16_t fbWidth, fbHeight, fbStride;
extern uint8_t fbBPP;

#define gfxColor(r, g, b) ((r << 16) | (g << 8) | (b << 0))
#define gfxColorHex(hex)  gfxColor(((hex >> 16) & 0xFF), ((hex >> 8) & 0xFF), ((hex >> 0) & 0xFF))

inline void gfxDrawPixel(int x, int y, gfxBuffer buffer, uint32_t col);
inline void gfxDrawHLine(int x, int y, int w, gfxBuffer buffer, uint32_t col);

void gfxDrawRect(int x, int y, int w, int h, gfxBuffer buffer, uint32_t col);
void gfxDrawLine(int x1, int y1, int x2, int y2, gfxBuffer buffer, uint32_t col);

void gfxDrawLegacyBitmap(int x, int y, uint8_t *bitmap, gfxBuffer buffer, uint32_t col);
void gfxDrawString(int x, int y, char *str, gfxBuffer buffer, uint32_t col);

void gfxDrawBuffer(int x, int y, gfxBuffer src, gfxBuffer dest);
void gfxSwapBuffers(void);
void gfxClearBuffer(gfxBuffer buffer, uint32_t col);

int initGraphics(MultibootInfo *mbi);

#endif