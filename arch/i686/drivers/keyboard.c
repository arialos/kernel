#include <stdint.h>

#include "libio.h"

#include "isr.h"
#include "gfx.h"

uint8_t scancodeSetOne[128] =
    {
        0, 27, '1', '2', '3', '4', '5', '6', '7', '8',    /* 9 */
        '9', '0', '-', '=', '\b',                         /* Backspace */
        '\t',                                             /* Tab */
        'q', 'w', 'e', 'r',                               /* 19 */
        't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',     /* Enter key */
        0,                                                /* 29   - Control */
        'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', /* 39 */
        '\'', '`', 0,                                     /* Left shift */
        '\\', 'z', 'x', 'c', 'v', 'b', 'n',               /* 49 */
        'm', ',', '.', '/', 0,                            /* Right shift */
        '*',
        0,   /* Alt */
        ' ', /* Space bar */
        0,   /* Caps lock */
        0,   /* 59 - F1 key ... > */
        0, 0, 0, 0, 0, 0, 0, 0,
        0, /* < ... F10 */
        0, /* 69 - Num lock*/
        0, /* Scroll Lock */
        0, /* Home key */
        0, /* Up Arrow */
        0, /* Page Up */
        '-',
        0, /* Left Arrow */
        0,
        0, /* Right Arrow */
        '+',
        0, /* 79 - End key*/
        0, /* Down Arrow */
        0, /* Page Down */
        0, /* Insert Key */
        0, /* Delete Key */
        0, 0, 0,
        0, /* F11 Key */
        0, /* F12 Key */
        0, /* All other keys are undefined */
};

static void keyboardHandler(struct Registers *regs)
{
    (void)regs;
    uint8_t scancode = inb(0x60);

    // 0x80 indicates that the key was released. So we ignore it.
    if (scancode & 0x80)
        return;

    printf("%c", scancodeSetOne[scancode]);
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
