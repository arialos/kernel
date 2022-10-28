#ifndef VBE_H
#define VBE_H

#include <stdbool.h>
#include <stdint.h>

#include "libmath.h"
#include "multiboot.h"

extern uint16_t fbWidth;
extern uint16_t fbHeight;
extern uint8_t fbBPP;
extern uint16_t fbStride;

static int32_t cursorPaddingX = 10;
static int32_t cursorPaddingY = 5;
static int32_t cursorX, cursorY;
static uint32_t fgColor, bgColor;

#define gfxColor(r, g, b) ((r << 16) | (g << 8) | (b << 0))

bool initGFX(multiboot_info_t *mbi);
void gfxFramebufferClear(uint32_t col);

void gfxSaveTempBuffer(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t *tempBuffer);
void gfxRestoreTempBuffer(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t *tempBuffer);

void gfxDrawPixel(int32_t x, int32_t y, uint32_t col);
void gfxDrawCursor(int32_t x, int32_t y, uint32_t col);
void gfxDrawCharacter(int32_t x, int32_t y, char c, uint32_t col);
void gfxDrawRect(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t col);
void gfxFramebufferClear(uint32_t col);
void gfxPutString(const char *str);
void gfxSwapBuffers(void);

#endif