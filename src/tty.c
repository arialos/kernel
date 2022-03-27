#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "include/vga.h"
#include "include/string.h"

static const size_t vgaWidth = 80;
static const size_t vgaHeight = 25;
static uint16_t *const vgaMemory = (uint16_t *)0xB8000;

static size_t currentRow;
static size_t currentColumn;
static uint8_t terminalColor;
static uint16_t *terminalBuffer;

void initTerminal(void)
{
    currentRow = 0;
    currentColumn = 0;
    terminalColor = vgaEntryColor(vgaColorLightBlue, vgaColorBlack);
    terminalBuffer = vgaMemory;
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
    terminalColor = vgaEntryColor(color, vgaColorBlack);
}

void terminalEntryAt(char c, uint8_t color, size_t x, size_t y)
{
    const size_t index = y * vgaWidth + x;
    terminalBuffer[index] = vgaEntry(c, color);
}

void scrollTerminal(int line)
{
    int *loop;
    char c;

    for (loop = line * (vgaWidth * 2) + 0xB8000; loop < vgaWidth * 2; loop++)
    {
        c = *loop;
        *(loop - (vgaWidth * 2)) = c;
    }
}

void deleteLastTerminalLine()
{
    int x, *ptr;

    for (x = 0; x < vgaWidth * 2; x++)
    {
        ptr = 0xB8000 + (vgaWidth * 2) * (vgaHeight - 1) + x;
        *ptr = 0;
    }
}

void terminalPutChar(char c)
{
    int line;
    unsigned char uc = c;

    if (uc == '\n')
    {
        currentRow++;
        currentColumn = 0;
        setTerminalColor(vgaColorLightBlue);
        return;
    }
    else if (uc == '\2')
    {
        setTerminalColor(vgaColorLightGreen);
        return;
    }
    else if (uc == '\7')
    {
        setTerminalColor(vgaColorLightGrey);
        return;
    }
    else if (uc == '\4')
    {
        setTerminalColor(vgaColorRed);
        return;
    }

    terminalEntryAt(uc, terminalColor, currentColumn, currentRow);

    if (++currentColumn == vgaWidth)
    {
        currentColumn = 0;
        if (++currentRow == vgaHeight)
        {
            for (line = 1; line <= vgaHeight - 1; line++)
            {
                scrollTerminal(line);
            }
            deleteLastTerminalLine();
            currentRow = vgaHeight - 1;
        }
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
