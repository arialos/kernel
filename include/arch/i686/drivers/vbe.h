#ifndef VBE_H
#define VBE_H

#include <stdint.h>
#include "multiboot.h"


static uint32_t fgColor, bgColor;
#define MIN(x,y) ((x<y)?x:y)
#define vbeColor(r, g, b)  ((r << 16)|(g << 8)|(b << 0))

bool initVBE( multiboot_info_t *vbe);
void vbePutString(const char* str);
void vbeDrawRect(int x, int y, int w, int h, uint32_t col);
void vbeDrawPixel(int x, int y, uint32_t col);

#endif