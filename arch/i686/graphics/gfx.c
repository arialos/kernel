#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "libio.h"
#include "libmath.h"
#include "libstring.h"

#include "font.h"
#include "gfx.h"
#include "multiboot.h"

static uint8_t *screenbuffer = NULL;
static uint8_t *backbuffer   = NULL;

uint16_t fbWidth;
uint16_t fbHeight;
static uint8_t fbBPP;

/**
 * @brief Draws a Pixel to the main framebuffer inside the video memory.
 *
 * @param x (int32_t) The Horizontal position of the pixel.
 * @param y (int32_t) The Vertical position of the pixel.
 * @param col (uint32_t) The byte color of the pixel. It's best to use the
 * "gfxColor" macro for this.
 */
inline void gfxDrawPixel( int32_t x, int32_t y, uint32_t col ) {
    ( (uint32_t *)screenbuffer )[y * fbWidth + x] = col;
}

/**
 * @brief Draws a Rectangle to the main framebuffer inside the video memory. By
 * default, the rectangle is filled and will anchor from the top-left of the
 * coordinates
 *
 * @param x (int32_t) The Horizontal position of the rectangle.
 * @param y (int32_t) The Vertical position of the rectangle.
 * @param w (int32_t) The width of the rectangle.
 * @param h (int32_t) The height of the rectangle.
 * @param col (uint32_t) The byte color of the rectangle. It's best to use the
 * "gfxColor" macro for this.
 */
void gfxDrawRect( int32_t x, int32_t y, int32_t w, int32_t h, uint32_t col ) {
    int32_t i, j;
    for ( i = 0; i < h; i++ ) {
        for ( j = 0; j < w; j++ ) {
            ( (uint32_t *)screenbuffer )[( i + y ) * fbWidth + ( j + x )] = col;
        }
    }
}

// This has to be the stupidest code I've ever written
// because of the pointer to temBuffer always being
// NULL when it is passed into the function to restore
// I can't simply return check it in gfxRestoreTempBuffer
// so in turn a STUPID workaround is this abomination
// as long as only one buffer is being updated EVER
// this will work.
bool beenDrawn = false;

/**
 * @brief Copies a portion of the framebuffer between the coordinates given
 * and stores it in temporary buffer for redrawing later on.
 *
 * @param x (int32_t) The Horizontal position of the buffer region.
 * @param y (int32_t) The Vertical position of the buffer region.
 * @param w (int32_t) The width of the buffer region.
 * @param h (int32_t) The height of the buffer region.
 * @param tempBuffer (uint32_t) The pointer to the temporary buffer.
 */
void gfxSaveTempBuffer(
    int32_t x, int32_t y, int32_t w, int32_t h, uint32_t *tempBuffer
) {
    for ( int32_t i = 0; i < h; i++ ) {
        for ( int32_t j = 0; j < w; j++ ) {
            tempBuffer[i * w + j] =
                ( (uint32_t *)screenbuffer )[( i + y ) * fbWidth + ( j + x )];
        }
    }
    beenDrawn = true;
}

/**
 * @brief Restores a portion of the framebuffer between the coordinates given
 * from a temporary buffer.
 *
 * @param x (int32_t) The Horizontal position of the buffer region.
 * @param y (int32_t) The Vertical position of the buffer region.
 * @param w (int32_t) The width of the buffer region.
 * @param h (int32_t) The height of the buffer region.
 * @param tempBuffer (uint32_t) The pointer to the temporary buffer.
 */
void gfxRestoreTempBuffer(
    int32_t x, int32_t y, int32_t w, int32_t h, uint32_t *tempBuffer
) {
    // This is really janky and bad code to be honest
    // but whatever it works while we are only drawing
    // the mouse cursor
    if ( !beenDrawn ) return;

    for ( int32_t i = 0; i < h; i++ ) {
        for ( int32_t j = 0; j < w; j++ ) {
            ( (uint32_t *)screenbuffer )[( i + y ) * fbWidth + ( j + x )] =
                tempBuffer[i * w + j];
        }
    }
}

/**
 * @brief Draws a character to the main framebuffer inside the video memory.
 *
 * @param x (int32_t) The Horizontal position of the character.
 * @param y (int32_t) The Vertical position of the character.
 * @param c (char) The character to draw.
 * @param col (uint32_t) The byte color of the character. It's best to use the
 * "gfxColor" macro for this.
 */
void gfxDrawCharacter( int32_t x, int32_t y, char c, uint32_t col ) {
    uint8_t *line_addr = screenbuffer + ( x * fbBPP ) + ( y * fbWidth * fbBPP );
    const uint16_t stride = fbWidth * fbBPP;
    const uint8_t stop_y  = MIN( fontHeight, fbHeight - y );
    const uint8_t stop_x  = MIN( fontWidth, fbWidth - x );
    if ( c < 0 || c > 132 ) return;

    for ( int32_t i = 0; i < stop_y; ++i ) {
        uint8_t mask_table[8] = { 128, 64, 32, 16, 8, 4, 2, 1 };
        for ( int32_t j = 0; j < stop_x; ++j ) {
            if ( systemFont[c][i] & mask_table[j] )
                ( (uint32_t *)line_addr )[j] = col;
        }
        line_addr += stride;
    }
}

/**
 * @brief Draws a character to the main framebuffer inside the video memory.
 *
 * @param x (int32_t) The Horizontal position of the character.
 * @param y (int32_t) The Vertical position of the character.
 * @param col (uint32_t) The byte color of the character. It's best to use the
 * "gfxColor" macro for this.
 */
void gfxDrawCursor( int32_t x, int32_t y, uint32_t col ) {
    uint8_t *line_addr = screenbuffer + ( x * fbBPP ) + ( y * fbWidth * fbBPP );
    const uint16_t stride = fbWidth * fbBPP;
    const uint8_t stop_y  = MIN( fontHeight, fbHeight - y );
    const uint8_t stop_x  = MIN( fontWidth, fbWidth - x );

    for ( int32_t i = 0; i < stop_y; ++i ) {
        uint8_t mask_table[8] = { 128, 64, 32, 16, 8, 4, 2, 1 };
        for ( int32_t j = 0; j < stop_x; ++j ) {
            if ( systemCursor[i] & mask_table[j] )
                ( (uint32_t *)line_addr )[j] = col;
        }
        line_addr += stride;
    }
}

/**
 * @brief Clears the main framebuffer inside the video memory.
 *
 * @param col (uint32_t) The byte color to use for the clear. It's best to use
 * the "gfxColor" macro for this.
 */
void gfxFramebufferClear( uint32_t col ) {
    uint8_t *p    = screenbuffer;
    uint8_t *stop = screenbuffer + fbWidth * fbHeight * fbBPP;
    while ( p < stop ) {
        *(uint32_t *)p = col;
        p += fbBPP;
    }
}

/**
 * @brief Handles the scrolling of the framebuffer as well as postion and
 * drawing of characters to the display.
 *
 * @param ch (char) The character to draw.
 */
void gfxPutCharacter( char ch )

{
    // If the cursor is at the bottom of the screen, scroll up
    // by the height of a character
    if ( cursorY + fontHeight >= fbHeight ) {
        gfxFramebufferScroll( fontHeight );
        cursorY -= fontHeight;
    }

    if ( ch == '\n' || cursorX + fontWidth >= fbWidth - cursorPaddingX ) {
        cursorX = cursorPaddingX;
        cursorY += fontHeight;
        return;
    }

    gfxDrawCharacter( cursorX, cursorY, ch, fgColor );
    cursorX += fontWidth;
}

void gfxFramebufferScroll( uint16_t lines ) {
    uint8_t *p    = screenbuffer;
    uint8_t *stop = screenbuffer + fbWidth * fbHeight * fbBPP;
    while ( p < stop ) {
        *(uint32_t *)p = *(uint32_t *)( p + lines * fbWidth * fbBPP );
        p += fbBPP;
    }
}

void gfxPutString( const char *str ) {
    while ( *str ) {
        gfxPutCharacter( *str++ );
    }
}

void gfxSwapBuffers() {
    memcpy( screenbuffer, backbuffer, fbWidth * fbHeight * ( fbBPP / 8 ) );
    memset( backbuffer, 0, fbWidth * fbHeight * ( fbBPP / 8 ) );
}

bool initGFX( multiboot_info_t *mbi ) {
    screenbuffer = (uint8_t *)mbi->framebuffer_addr;

    // We need to allocate a screenbuffer for double buffering

    fbWidth     = mbi->framebuffer_width;
    fbHeight    = mbi->framebuffer_height;
    fbBPP       = mbi->framebuffer_bpp / 8;

    if ( fbBPP != 4 ) return false;

    // Clear the buffer and set everything black
    gfxFramebufferClear( gfxColor( 0x00, 0x6E, 0x8D ) );

    cursorX = cursorPaddingX;
    cursorY = cursorPaddingY;

    fgColor = gfxColor( 0xFF, 0xFF, 0xFF );
    bgColor = gfxColor( 0x00, 0x6E, 0x8D );

    return true;
}