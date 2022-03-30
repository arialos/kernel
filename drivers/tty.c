#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "libstring.h"

#include "tty.h"

static const size_t vgaWidth = 80;
static const size_t vgaHeight = 25;

static size_t currentRow;
static size_t currentColumn;
static uint8_t ttyFgColor, ttyBgColor;
static uint16_t *terminalBuffer = (uint16_t *)0xB8000;

void ttyClear ()
{
    for (size_t y = 0; y < vgaHeight; y++)
    {
        for (size_t x = 0; x < vgaWidth; x++)
        {
            terminalBuffer[y * vgaWidth + x] = ttyEntry(' ', ttyColor(ttyFgColor, ttyBgColor));
        }
    }
}

void ttyPutCharactor(char c)
{
    int line;
    unsigned char uc = c;

    if (uc == '\n')
    {
        currentRow++;
        currentColumn = 0;
        return;
    }

    if (++currentColumn == vgaWidth)
    {
        currentColumn = 0;
        if (++currentRow == vgaHeight)
        {
            ttyClear();
            currentRow = 0;
        }
    }
    terminalBuffer[currentRow * vgaWidth + currentColumn] = ttyEntry(c, ttyColor(ttyFgColor, ttyBgColor));
}

void ttyPutString(const char *data)
{
    for (size_t i = 0; i < strlen(data); i++)
        ttyPutCharactor(data[i]);
}

bool initTty()
{
    currentRow = 0;
    currentColumn = 0;

    ttyFgColor = vgaColorLightBlue;
    ttyBgColor = vgaColorBlack;

    ttyClear();

    return true;
}