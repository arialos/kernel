#include "window.h"
#include "gfx.h"
#include "libio.h"
#include "mouse.h"

Window *windows[];

void wmRedrawWindows() {
    gfxFramebufferClear( gfxColor( 0x00, 0x00, 0x00 ) );

    if ( windows )
        for ( int i = 0; i < ( sizeof( Window ) / sizeof( windows[0] ) );
              i++ ) {
            Window *window = windows[i];
            gfxRestoreTempBuffer(
                window->x, window->y, window->w, window->h, window->buffer
            );
        }

    // Draw the mouse cursor
    gfxDrawCursor( mouseX, mouseY, gfxColor( 0xFF, 0xFF, 0xFF ) );

    gfxSwapBuffers();
}

void initWindowManager( void ) {
    for ( ;; ) {
        wmRedrawWindows();
    }
}