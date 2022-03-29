#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdarg.h>
#include "stdmath.h"

#include "multiboot.h"
#include "string.h"
#include "vbe.h"
#include "font.h"

static uint8_t *framebuffer = NULL;
static uint16_t fbWidth;
static uint16_t fbHeight;

static uint8_t fbBPP;

const uint16_t *vgaWidth = &fbWidth;
const uint16_t *vgaHeight = &fbHeight;

void vbeDrawPixel(int x, int y, uint32_t col)
{
    ((uint32_t *)framebuffer)[y * fbWidth + x] = col;
}

void vbeDrawRect(int x, int y, int w, int h, uint32_t col)
{
    int i, j;
    for (i = 0; i < h; i++)
    {
        for (j = 0; j < w; j++)
        {
            ((uint32_t *)framebuffer)[(i + y) * fbWidth + (j + x)] = col;
        }
    }
}

void vbeClear(uint32_t col)
{
    uint8_t *p = framebuffer;
    uint8_t *stop = framebuffer + fbWidth * fbHeight * fbBPP;
    while (p < stop)
    {
        *(uint32_t *)p = col;
        p += fbBPP;
    }
}

void vbeDrawCharactor(int x, int y, char c)
{
    uint8_t *line_addr = framebuffer + (x * fbBPP) + (y * fbWidth * fbBPP);
    const uint32_t fg = fgColor;
    const uint16_t stride = fbWidth * fbBPP;
    const uint8_t stop_y = MIN(fontHeight, fbHeight - y);
    const uint8_t stop_x = MIN(fontWidth, fbWidth - x);
    if (c < 0 || c > 132)
        return;
    for (int i = 0; i < stop_y; ++i)
    {
        uint8_t mask_table[8] = {128, 64, 32, 16, 8, 4, 2, 1};
        for (int j = 0; j < stop_x; ++j)
        {
            if (systemFont[c][i] & mask_table[j])
                ((uint32_t *)line_addr)[j] = fg;
        }
        line_addr += stride;
    }
}

void vbePutCharactor(char ch)

{
    if (ch == '\n' || cursorX + fontWidth >= fbWidth)
    {
        cursorX = 10;
        cursorY += fontHeight;
        return;
    }

    vbeDrawCharactor(cursorX, cursorY, ch);
    cursorX += fontWidth;
}

void vbePutString(const char *str)
{
    while (*str)
    {
        vbePutCharactor(*str++);
    }
}

static char hex_table[16] = { '0', '1', '2', '3', '4', '5', '6', '7',
                              '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

static void printHex(unsigned int n)
{
    unsigned mask = 0xF0000000;
    unsigned shift = 28;
    while(mask)
    {
        vbePutCharactor(hex_table[(n & mask) >> shift]);
        mask >>= 4;
        shift -= 4;
    }
}

int printf(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    while(*fmt)
    {
        char ch = *fmt++;
        switch(ch)
        {
        case '%':
        {
            switch(*fmt++)
            {
            case '1':
                fgColor = vbeColor(0, 0xFF, 0);
                break;
            case '2':
                fgColor = vbeColor(0xFF, 0, 0);
                break;
            case '3':
                fgColor = vbeColor(0xFF, 0xFF, 0xFF);
                break;
            case 'c':
                vbePutCharactor(va_arg(ap, int));
                break;
            case 's': // string
                vbePutString(va_arg(ap, const char*));
                break;
            case 'x': // Hex
                printHex(va_arg(ap, unsigned));
                break;
            case 'd': // Int
                vbePutString(itoa(va_arg(ap, unsigned), 10));
                break;
            default:
                vbePutString("printf: unknown format\n");
                break;
            }
            break;
        }
        default:
            vbePutCharactor(ch);
            break;
        }
    }
    va_end(ap);
    return 0;
}

bool initVBE(multiboot_info_t *vbe)
{
    framebuffer = (uint8_t *)vbe->framebuffer_addr;
    fbWidth = vbe->framebuffer_width;
    fbHeight = vbe->framebuffer_height;
    fbBPP = vbe->framebuffer_bpp / 8;

    if (fbBPP != 4)
        return false;

    // Clear the buffer and set everything black
    vbeClear(vbeColor(0, 0, 0));

    cursorX =10;
    cursorY = 5;
    fgColor = vbeColor(0xFF, 0xFF, 0xFF);
    bgColor = vbeColor(0, 0, 0);

    return true;
}