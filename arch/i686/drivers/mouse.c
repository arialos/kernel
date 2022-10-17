#include <stdbool.h>
#include <stdint.h>

#include "libio.h"

#include "gfx.h"
#include "irq.h"
#include "mouse.h"

void mouseWait( uint8_t WaitType ) {
    uint32_t timeout = 100000;

    while ( --timeout ) {
        if ( !WaitType )
            if ( ( inb( 0x64 ) & 0x01 ) == 1 )
                return;
            else

                if ( !( ( inb( 0x64 ) & 0x02 ) ) )
                return;

        printf( "[ MOUSE ] Timeout!\n" );
        return;
    }
}

static void MouseInterruptHandler( struct Registers *regs ) {
    (void)regs;

    printf( "[ Mouse ] Mouse interrupt handler called: %x\n", inb( 0x60 ) );
}

bool initMouse( void ) {
    uint8_t status;

    printf( "[ MOUSE ] Enabling second PS/2 Port\n" );
    mouseWait( 1 );
    outb( 0x64, 0xA8 );

    printf( "[ MOUSE ] Reading PS/2 controller setup byte" );
    mouseWait( 1 );
    outb( 0x64, 0x20 );
    mouseWait( 0 );

    status = inb( 0x60 ) | 2;

    printf( "[ MOUSE ] Writing status to PS/2 controller setup byte" );
    mouseWait( 1 );
    outb( 0x64, 0x60 );
    mouseWait( 1 );
    outb( 0x60, status );

    // Set the defaults for mouse movement and then wait for
    // PS/2 controller to acknowledge
    mouseWait( 1 );
    outb( 0x64, 0xD4 );
    mouseWait( 1 );
    outb( 0x60, 0xF6 );

    mouseWait( 0 );
    printf( "[ MOUSE ] Setting defaults... " );
    if ( inb( 0x60 ) != 0xFA ) {
        printf( "[ MOUSE ] Failed to set defaults!\n" );
        return false;
    } else {
        printf( "Success!\n" );
    }

    // Enable the mouse and then wait for PS/2 controller to
    // acknowledge
    mouseWait( 1 );
    outb( 0x64, 0xD4 );
    mouseWait( 1 );
    outb( 0x60, 0xF4 );

    mouseWait( 0 );
    printf( "[ MOUSE ] Enabling the mouse... " );
    if ( inb( 0x60 ) != 0xFA ) {
        printf( "[ MOUSE ] Failed to set enable the mouse!\n" );
        return false;
    } else {
        printf( "Success!\n" );
    }

    irqInstallHandler( 12, MouseInterruptHandler );

    printf(
        "[ MOUSE ] Driver registered successfully at: 0x%x\n",
        MouseInterruptHandler
    );

    return true;
}