#include "keyboard.h"

#include <stdbool.h>
#include <stdint.h>

#include "irq.h"
#include "libio.h"
#include "scancodes.h"

bool keyboardCapsLock = false;
bool keyboardShift    = false;

// void keyPressHandler( struct KeyEvent *event ) {
//     printf( "%c", event->key );
// }

// Keep a string of all the keys that are currently pressed
char *pressed_keys = "";

static void keyboardInterruptHandler(Registers *regs) {
    (void)regs;

    uint8_t scancode = inb(0x60);

    switch (scancode) {
        // 0x80 indicates that the key was released. So we ignore it.
        case 0xAA:
        case 0xB6:
            keyboardShift = false;
            break;
        // 0x3A is the Caps Lock key.
        case 0x3A:
            keyboardCapsLock = !keyboardCapsLock;
            break;
        // Cover both left (0x2A) and right (0x36) shift keys
        case 0x2A:
        case 0x36:
            keyboardShift = !keyboardShift;
            break;
        // Handle all other keys
        default:
            if (scancode & 0x80) return;
            pressed_keys = setOneBase[(keyboardShift) ? 1 : (keyboardCapsLock) ? 2 : 0][scancode];
            break;
    }

    //     if ( setOneBase[1][scancode] ==
    //          0 ) return printf( "Unknown key: %x", scancode );

    // printf( "%c", setOneBase[1][scancode] );
}

int initKeyboard(void) {
    irqInstallHandler(1, keyboardInterruptHandler);

    printf("[ KEYBOARD ] Driver registered successfully at: 0x%x\n", keyboardInterruptHandler);

    // printf("[ PS/2 ] Probing controller status flags...\n");
    // printf("%x ", inb(0x64));

    printf("[ KEYBOARD ] Waiting for Keyboard acknowledgement... ");
    while (1)
        if ((inb(0x64) & 2) == 0) break;

    // Get the current state of the keyboard
    outb(0x60, 0xEE);
    uint8_t status = inb(0x60);
    printf("Status: %x ", status);

    printf("Success!\n");
    return 0;
}
