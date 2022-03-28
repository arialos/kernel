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

void gfx_drawpixel(int x, int y, uint32_t col)
{
    ((uint32_t*)framebuffer)[y * fbWidth + x] = col;
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

void vbeDrawCharactor(int x, int y, char c, uint32_t fg, uint32_t bg)
{
    int stride = fbBPP * fbWidth;
    uint8_t *line_addr = framebuffer + (x * fbBPP) + (y * fbWidth * fbBPP);
    for(int i = y; i < y + FONT_HEIGHT; ++i)
    {
        uint32_t line_buf[8] = {bg};
        uint8_t mask = 0x80;
        for(int j = 0; j < 8; ++j, mask >>= 1)
        {
            if(gfx_font[(int)c][i] & mask)
                line_buf[j] = fg;
        }
        memcpy(line_addr, line_buf, sizeof(line_buf));
        line_addr += stride;
    }
}

void vbePutCharactor(char ch)
{
    if(ch != '\n')
    {
        vbeDrawCharactor(cursorX, cursorY, ch, fgColor, bgColor);
        cursorX += FONT_WIDTH;
        if(cursorX >= fbWidth)
        {
            cursorX = 0;
            cursorY += FONT_HEIGHT;
            if(cursorY >= fbHeight)
            {
                vbeClear(bgColor);
                cursorY = 0;
            }
        }
    }
    else
    {
        cursorX = 0;
        cursorY += FONT_HEIGHT;
        if(cursorY >= fbHeight)
        {
            vbeClear(bgColor);
            cursorY = 0;
        }
    }
}

void vbePutString(const char* str)
{
    while(*str)
    {
        vbePutCharactor(*str++);
    }
}

bool initVBE(struct vbe_info_t *vbe) {
    framebuffer = (uint8_t *)vbe->physbase;
    fbWidth = vbe->Xres;
    fbHeight = vbe->Yres;
    fbBPP = vbe->bpp / 8;

    if (fbBPP != 4) return false;

    // Clear the buffer and set everything black
    vbeClear(vbeColor(0,0,0));

    cursorX, cursorY = 0;
    fgColor = vbeColor(0xff, 0xff, 0xff);
    bgColor = vbeColor(0, 0, 0);

    vbePutString("Hello, world!\n");
    return true;
}