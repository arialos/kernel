#ifndef VBE_H
#define VBE_H

#include <stdbool.h>
#include <stdint.h>

#include "libmath.h"

#include "multiboot.h"

static uint16_t fbWidth;
static uint16_t fbHeight;

static int cursorPaddingX = 10;
static int cursorPaddingY = 5;
static int cursorX, cursorY;
static uint32_t fgColor, bgColor;

#define gfxColor( r, g, b ) ( ( r << 16 ) | ( g << 8 ) | ( b << 0 ) )

bool initGFX( multiboot_info_t *mbi );
void gfxFramebufferClear( uint32_t col );

void gfxSaveTempBuffer( int x, int y, int w, int h, uint32_t *tempBuffer );
void gfxRestoreTempBuffer( int x, int y, int w, int h, uint32_t *tempBuffer );

void gfxDrawPixel( int x, int y, uint32_t col );
void gfxDrawCursor( int x, int y, uint32_t col );
void gfxDrawCharacter( int x, int y, char c, uint32_t col );
void gfxDrawRect( int x, int y, int w, int h, uint32_t col );

void gfxPutString( const char *str );

#endif