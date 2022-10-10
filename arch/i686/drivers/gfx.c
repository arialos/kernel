#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "libmath.h"
#include "libstring.h"

#include "font.h"
#include "gfx.h"
#include "multiboot.h"

static uint8_t *framebuffer = NULL;
static uint16_t fbWidth;
static uint16_t fbHeight;

static uint8_t fbBPP;

const uint16_t *vgaWidth  = &fbWidth;
const uint16_t *vgaHeight = &fbHeight;

void gfxDrawPixel( int x, int y, uint32_t col ) {
    ( (uint32_t *)framebuffer )[y * fbWidth + x] = col;
}

void gfxDrawRect( int x, int y, int w, int h, uint32_t col ) {
    int i, j;
    for ( i = 0; i < h; i++ ) {
        for ( j = 0; j < w; j++ ) {
            ( (uint32_t *)framebuffer )[( i + y ) * fbWidth + ( j + x )] = col;
        }
    }
}

void gfxDrawCharactor( int x, int y, char c ) {
    uint8_t *line_addr = framebuffer + ( x * fbBPP ) + ( y * fbWidth * fbBPP );
    const uint32_t fg  = fgColor;
    const uint16_t stride = fbWidth * fbBPP;
    const uint8_t stop_y  = MIN( fontHeight, fbHeight - y );
    const uint8_t stop_x  = MIN( fontWidth, fbWidth - x );
    if ( c < 0 || c > 132 ) return;
    for ( int i = 0; i < stop_y; ++i ) {
        uint8_t mask_table[8] = { 128, 64, 32, 16, 8, 4, 2, 1 };
        for ( int j = 0; j < stop_x; ++j ) {
            if ( systemFont[c][i] & mask_table[j] )
                ( (uint32_t *)line_addr )[j] = fg;
        }
        line_addr += stride;
    }
}

void gfxFramebufferClear( uint32_t col ) {
    uint8_t *p    = framebuffer;
    uint8_t *stop = framebuffer + fbWidth * fbHeight * fbBPP;
    while ( p < stop ) {
        *(uint32_t *)p = col;
        p += fbBPP;
    }
}

void gfxPutCharactor( char ch )

{
    if ( ch == '\n' || cursorX + fontWidth >= fbWidth ) {
        cursorX = 10;
        cursorY += fontHeight;
        return;
    }

    gfxDrawCharactor( cursorX, cursorY, ch );
    cursorX += fontWidth;
}

void gfxPutString( const char *str ) {
    while ( *str ) {
        gfxPutCharactor( *str++ );
    }
}

bool initGFX( multiboot_info_t *mbi ) {
    framebuffer = (uint8_t *)mbi->framebuffer_addr;
    fbWidth     = mbi->framebuffer_width;
    fbHeight    = mbi->framebuffer_height;
    fbBPP       = mbi->framebuffer_bpp / 8;

    if ( fbBPP != 4 ) return false;

    // Clear the buffer and set everything black
    gfxFramebufferClear( gfxColor( 0x00, 0x6E, 0x8D ) );

    cursorX = 10;
    cursorY = 5;
    fgColor = gfxColor( 0xFF, 0xFF, 0xFF );
    bgColor = gfxColor( 0x00, 0x6E, 0x8D );

    return true;
}