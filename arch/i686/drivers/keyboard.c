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

static void keyboardHandler(struct Registers *regs)
{
    (void)regs;
    uint8_t scancode = inb(0x60);

    // 0x80 indicates that the key was released. So we ignore it.
    if (scancode & 0x80)
        return;

    printf("%x ", scancode);
}

void initKeyboard(void)
{
    irqInstallHandler(1, keyboardHandler);
    printf("[ KEYBOARD ] Driver registered successfully at: 0x%x\n", keyboardHandler);

    // printf("[ PS/2 ] Probing controller status flags...\n");
    // printf("%x ", inb(0x64));

    printf("[ KEYBOARD ] Waiting for Keyboard acknowledgement... ");
    while (1)
        if ((inb(0x64) & 2) == 0)
            break;

    // Get the current state of the keyboard
    outb(0x60, 0xEE);
    uint8_t status = inb(0x60);
    printf("Status: %x ", status);

    printf("Success!\n");
}
