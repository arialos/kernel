#ifndef GFX_H
#define GFX_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "multiboot.h"

extern bool fSynced;

extern uint16_t fbWidth, fbHeight, fbStride;
extern uint8_t fbBPP;

#define gfxColor(r, g, b) ((r << 16) | (g << 8) | (b << 0))

inline void gfxDrawPixel(int x, int y, uint32_t col);
inline void gfxDrawHLine(int x, int y, int w, uint32_t col);

void gfxDrawRect(int x, int y, int w, int h, uint32_t col);
void gfxDrawLine(int x1, int y1, int x2, int y2, uint32_t col);

void gfxDrawLegacyBitmap(int x, int y, uint8_t *bitmap, uint32_t col);
void gfxDrawBuffer(int x, int y, int w, int h, uint32_t *buffer);
void gfxSwapBuffers(void);
void gfxClearBuffer(uint32_t col);

int initGraphics(MultibootInfo *mbi);

#endif