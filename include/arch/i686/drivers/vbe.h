#ifndef VBE_H
#define VBE_H

#include <stdint.h>
#include "multiboot.h"
#include "stdmath.h"

static uint32_t fgColor, bgColor;

#define vbeColor(r, g, b)  ((r << 16)|(g << 8)|(b << 0))

void vbeDrawPixel(int x, int y, uint32_t col);
void vbeDrawRect(int x, int y, int w, int h, uint32_t col);
void vbeClear(uint32_t col);
void vbePutString(const char* str);

bool initVBE( multiboot_info_t *vbe);

#endif