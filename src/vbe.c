#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "include/multiboot.h"
#include "include/string.h"
#include "include/vbe.h"
#include "include/font.h"

static uint8_t *framebuffer = NULL;
static uint16_t fbWidth;
static uint16_t fbHeight;

static uint8_t  fbBPP;

const uint16_t *vgaWidth = &fbWidth;
const uint16_t *vgaHeight = &fbHeight;

static int cursorX, cursorY;
static uint32_t fgColor, bgColor;

void vbeDrawPixel(int x, int y, uint32_t col)
{
    ((uint32_t*)framebuffer)[y * fbWidth + x] = col;
}

void vbeDrawRect(int x, int y, int w, int h, uint32_t col)
{
    int i, j;
    for( i = 0; i < h; i++)
    {
        for( j = 0; j < w; j++)
        {
            ((uint32_t*)framebuffer)[(i+y) * fbWidth + (j+x)] = col;
        }
    }
}

void vbeClear(uint32_t col)
{
    uint8_t *p = framebuffer;
    uint8_t *stop = framebuffer + fbWidth * fbHeight * fbBPP;
    while(p < stop)
    {
        *(uint32_t*)p = col;
        p += fbBPP;
    }
}


void vbeDrawCharactor(int x, int y, char c)
{
    uint8_t *line_addr = framebuffer + (x * fbBPP) + (y * fbWidth* fbBPP);
    const uint32_t fg = fgColor;
    const uint16_t stride = fbWidth*fbBPP;
    const uint8_t stop_y = MIN(FONT_HEIGHT, fbHeight - y);
    const uint8_t stop_x = MIN(FONT_WIDTH, fbWidth - x);
    if(c < 0 || c > 132)
        return;
    for(int i = 0; i < stop_y; ++i)
    {
        uint8_t mask_table[8] = {128, 64, 32, 16, 8, 4, 2, 1};
        for(int j = 0; j < stop_x; ++j)
        {
            if(gfx_font[c][i] & mask_table[j])
                ((uint32_t*)line_addr)[j] = fg;
        }
        line_addr += stride;
    }
}

void vbePutCharactor(char ch)
{
    if(ch == '\n' || cursorX + FONT_WIDTH >= fbWidth)
    {
        cursorX = 0;
        cursorY += FONT_HEIGHT;
        return;
    }


    
        vbeDrawCharactor(cursorX, cursorY, ch);
        cursorX += FONT_WIDTH;
}

void vbePutString(const char* str)
{
    while(*str)
    {
        vbePutCharactor(*str++);
    }
}

bool initVBE( multiboot_info_t *vbe) {
    framebuffer = (uint8_t *)vbe->framebuffer_addr;
    fbWidth = vbe->framebuffer_width;
    fbHeight = vbe->framebuffer_height;
    fbBPP = vbe->framebuffer_bpp / 8;

    if (fbBPP != 4) return false;

    // Clear the buffer and set everything black
    vbeClear(vbeColor(0xFF,0xFF,0xFF));

    cursorX, cursorY = 0;
    fgColor = vbeColor(0, 0, 0);
    bgColor = vbeColor(0xFF, 0xFF, 0xFF);


    return true;
}