#include <stdint.h>

#include "libio.h"

#include "isr.h"
#include "gfx.h"

static uint8_t keyboardScanCodeSet[] = {
    '\0',
    '\0',
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
    '0',
    '-',
    '=',
    '\0',
    '\t',
    'q',
    'w',
    'e',
    'r',
    't',
    'y',
    'u',
    'i',
    'o',
    'p',
    '[',
    ']',
    '\n',
    '\0',
    'a',
    's',
    'd',
    'f',
    'g',
    'h',
    'j',
    'k',
    'l',
    ';',
    '\'',
    '`',
    '\0',
    '\\',
    'z',
    'x',
    'c',
    'v',
    'b',
    'n',
    'm',
    ',',
    '.',
    '/',
    '\0',
    '*',
    '\0',
    ' ',
    '\0',
    /* caps lock */ '\0',
    /* F1 */ '\0',
    /* F2 */ '\0',
    /* F3 */ '\0',
    /* F4 */ '\0',
    /* F5 */ '\0',
    /* F6 */ '\0',
    /* F7 */ '\0',
    /* F8 */ '\0',
    /* F9 */ '\0',
    /* F10 */ '\0',
    /* num lock */ '\0',
    /* scroll lock */ '7',
    '8',
    '9',
    '-',
    '4',
    '5',
    '6',
    '+',
    '1',
    '2',
    '3',
    '0',
    '.',
    '\0',
    '\0',
    '\0',
    '\0',
    '\0',
    '\0',
    '\0',
    '\0',
    '\0',
};

uint8_t scancodeToChar(uint8_t scancode)
{
    size_t sizeOfKeyboardScanCodeSet = sizeof(keyboardScanCodeSet) / sizeof(uint8_t);
    if (scancode > sizeOfKeyboardScanCodeSet)
        return 0;

    return keyboardScanCodeSet[scancode] += 32;
}

static void keyboardHandler(struct regs_t *regs)
{
    (void)regs;
    uint8_t scancode = inb(0x60);

    // 0x80 indicates that the key was released. So we ignore it.
    if (scancode & 0x80)
        return;

    uint8_t character = scancodeToChar(scancode);

    printf("%d:%s", scancode, character);
}

void initKeyboard(void)
{
    registerInterruptHandler(33, keyboardHandler);
    printf("[ KEYBOARD ] Driver registered successfully at: 0x%x\n", keyboardHandler);

    printf("[ KEYBOARD ] Waiting for Keyboard acknowledgement... ");
    while (1)
        if ((inb(0x64) & 2) == 0)
            break;

    outb(0x60, 0xF0);
    outb(0x60, 1);
    printf("Success!\n");

    printf("[ KEYBOARD ] Keyboard ready\n");
}
