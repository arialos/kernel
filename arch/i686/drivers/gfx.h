#ifndef VBE_H
#define VBE_H

#include <stdint.h>
#include <stdbool.h>

#include "libmath.h"

#include "multiboot.h"

static int cursorX, cursorY;
static uint32_t fgColor, bgColor;

#define gfxColor(r, g, b)  ((r << 16)|(g << 8)|(b << 0))

bool initGFX( multiboot_info_t *mbi);
void gfxFramebufferClear(uint32_t col);

void gfxDrawPixel(int x, int y, uint32_t col);
void gfxDrawRect(int x, int y, int w, int h, uint32_t col);

void gfxPutString(const char* str);


#endif