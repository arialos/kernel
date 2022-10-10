#include <stdint.h>

#include "libio.h"

#include "gfx.h"
#include "isr.h"
#include "keyboard.h"
#include "scancodes.h"

void keyPressHandler( struct KeyEvent *event ) {
    printf( "%c", event->key );
}

static void keyboardInterruptHandler( struct Registers *regs ) {
    (void)regs;
    struct KeyEvent event;

    uint8_t scancode = inb( 0x60 );

    // 0x80 indicates that the key was released. So we ignore it.
    if ( scancode & 0x80 ) return;

    event.scancode = scancode;
    event.key      = setOneBase[1][scancode];
    event.shift    = false;
    event.ctrl     = false;
    event.alt      = false;

    keyPressHandler( &event );
}

void initKeyboard( void ) {
    irqInstallHandler( 1, keyboardInterruptHandler );
    printf(
        "[ KEYBOARD ] Driver registered successfully at: 0x%x\n",
        keyboardInterruptHandler
    );

    // printf("[ PS/2 ] Probing controller status flags...\n");
    // printf("%x ", inb(0x64));

    printf( "[ KEYBOARD ] Waiting for Keyboard acknowledgement... " );
    while ( 1 )
        if ( ( inb( 0x64 ) & 2 ) == 0 ) break;

    // Get the current state of the keyboard
    outb( 0x60, 0xEE );
    uint8_t status = inb( 0x60 );
    printf( "Status: %x ", status );

    printf( "Success!\n" );
}
