#include "mouse.h"

#include <stdbool.h>
#include <stdint.h>

#include "gfx.h"
#include "irq.h"
#include "libio.h"

uint8_t mousePacketCycle = 0;
int8_t mouseByte[3];

uint32_t mouseX, mouseY = 0;
uint32_t *cursorBuffer = NULL;

void mouseWait(uint8_t WaitType) {
    uint32_t timeout = 100000;

    while (--timeout) {
        if (!WaitType)
            if ((inb(0x64) & 0x01) == 1)
                return;
            else

                if (!((inb(0x64) & 0x02)))
                return;

        // printf( "[ MOUSE ] Timeout!\n" );
        return;
    }
}

static void MouseInterruptHandler(Registers *regs) {
    (void)regs;

    uint8_t status = inb(0x64);
    while (status & 0x01) {
        uint8_t mouseData = inb(0x60);
        if (status & 0x20) {
            switch (mousePacketCycle) {
                case 0:
                    mouseByte[0] = mouseData;
                    if (!(mouseData & 0x08)) return;
                    mousePacketCycle++;
                    break;
                case 1:
                    mouseByte[1] = mouseData;
                    mousePacketCycle++;
                    break;
                case 2:
                    mouseByte[2] = mouseData;
                    // Handle an overflow of mouse data
                    if (mouseByte[0] & 0x80 || mouseByte[0] & 0x40) return;

                    mousePacketCycle = 0;

                    // If the cursorBuffer is empty then don't restore the temp
                    // buffer

                    if (mouseX + mouseByte[1] > 0 && mouseX + mouseByte[1] < fbWidth) mouseX += mouseByte[1];

                    if (mouseY - mouseByte[2] > 0 && mouseY - mouseByte[2] < fbHeight) mouseY -= mouseByte[2];

                    // printf("[ MOUSE ] X: %d, Y: %d\n", mouseX, mouseY);

                    // if ( mouseByte[0] & 0x01 ) {
                    //     // Test is the mouse is over a window
                    //     struct Window *window = wmGetWindowAt( mouseX, mouseY
                    //     ); if ( window != NULL ) {
                    //         // printf( "[ MOUSE ] Window %d clicked!\n",
                    //         // window->id );
                    //         wmRedrawWindow( window, mouseX, mouseY );
                    //     }
                    // }

                    // if ( mouseByte[0] & 0x02 )
                    //     gfxDrawCursor( mouseX, mouseY, 0xFF0000 );

                    break;
            }
        }
        status = inb(0x64);
    }
}

bool initMouse(void) {
    uint8_t status;

    printf("[ MOUSE ] Enabling second PS/2 Port\n");
    mouseWait(1);
    outb(0x64, 0xA8);

    printf("[ MOUSE ] Reading PS/2 controller setup byte\n");
    mouseWait(1);
    outb(0x64, 0x20);
    mouseWait(0);

    status = inb(0x60) | 2;

    printf("\n[ MOUSE ] Writing status to PS/2 controller setup byte\n");
    mouseWait(1);
    outb(0x64, 0x60);
    mouseWait(1);
    outb(0x60, status);

    // Set the defaults for mouse movement and then wait for
    // PS/2 controller to acknowledge
    mouseWait(1);
    outb(0x64, 0xD4);
    mouseWait(1);
    outb(0x60, 0xF6);

    mouseWait(0);
    printf("[ MOUSE ] Setting defaults... ");
    if (inb(0x60) != 0xFA) {
        printf("[ MOUSE ] Failed to set defaults!\n");
        return false;
    } else {
        printf("Success!\n");
    }

    // Enable the mouse and then wait for PS/2 controller to
    // acknowledge
    mouseWait(1);
    outb(0x64, 0xD4);
    mouseWait(1);
    outb(0x60, 0xF4);

    mouseWait(0);
    printf("[ MOUSE ] Enabling the mouse... ");
    if (inb(0x60) != 0xFA) {
        printf("[ MOUSE ] Failed to set enable the mouse!\n");
        return false;
    } else {
        printf("Success!\n");
    }

    irqInstallHandler(12, MouseInterruptHandler);

    printf("[ MOUSE ] Driver registered successfully at: 0x%x\n", MouseInterruptHandler);

    return true;
}