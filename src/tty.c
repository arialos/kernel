#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "include/vga.h"
#include "include/string.h"

static const size_t vgaWidth = 80;
static const size_t vgaHeight = 25;

size_t currentRow;
size_t currentColumn;
uint8_t terminalColor;
uint16_t *terminalBuffer;

void initTerminal(void)
{
    currentRow = 0;
    currentColumn = 0;
    terminalColor = vgaEntryColor(vgaColorLightBlue, vgaColorBlack);
    terminalBuffer = (uint16_t *)0xB8000;
    for (size_t y = 0; y < vgaHeight; y++)
    {
        for (size_t x = 0; x < vgaWidth; x++)
        {
            const size_t index = y * vgaWidth + x;
            terminalBuffer[index] = vgaEntry(' ', terminalColor);
        }
    }
}

void setTerminalColor(uint8_t color)
{
    terminalColor = color;
}

void terminalEntryAt(char c, uint8_t color, size_t x, size_t y)
{
    const size_t index = y * vgaWidth + x;
    terminalBuffer[index] = vgaEntry(c, color);
}

void terminalPutChar(char c)
{
    terminalEntryAt(c, terminalColor, currentColumn, currentRow);
    if (++currentColumn == vgaWidth)
    {
        currentColumn = 0;
        if (++currentRow == vgaHeight)
            currentRow = 0;
    }
}

void writeTerminal(const char *data, size_t size)
{
    for (size_t i = 0; i < size; i++)
        terminalPutChar(data[i]);
}

void printF(const char *data)
{
    writeTerminal(data, strlen(data));
}

