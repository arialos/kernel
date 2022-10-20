#include "window.h"
#include "font.h"
#include "gfx.h"
#include "libstring.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct Window windows[];

void wmSpawnWindow( int x, int y, int w, int h, const char *title ) {
    int windowId = ( sizeof( struct Window ) / sizeof( windows[0] ) ) + 1 || 0;

    windows[windowId].id           = windowId;
    windows[windowId].backBuffer   = NULL;
    windows[windowId].windowBuffer = NULL;
    windows[windowId].w            = w;
    windows[windowId].h            = h;
    windows[windowId].x            = x;
    windows[windowId].y            = y;
    windows[windowId].title        = title;
    windows[windowId].parent       = NULL;

    wmRedrawWindow( windowId, x, y );
}

// Take a ID of a window within the windows array and draw it to the screen
void wmRedrawWindow( int id, int newX, int newY ) {

    gfxRestoreTempBuffer(
        windows[id].x, windows[id].y, 200, 200, windows[id].backBuffer
    );

    windows[id].x = newX;
    windows[id].y = newY;

    gfxSaveTempBuffer( newX, newY, 200, 200, windows[id].backBuffer );

    // Draw the window
    gfxDrawRect( newX, newY, 200, 200, gfxColor( 0, 0, 0 ) );
    gfxDrawRect(
        newX + 1, newY + 1, 200 - 2, 200 - 2, gfxColor( 255, 255, 255 )
    );
    gfxDrawRect( newX + 2, newY + 2, 200 - 4, 200 - 4, gfxColor( 0, 0, 0 ) );

    // Draw the title bar
    gfxDrawRect( newX + 2, newY + 2, 200 - 4, 20, gfxColor( 0, 0, 0 ) );
    gfxDrawRect( newX + 3, newY + 3, 200 - 6, 18, gfxColor( 255, 255, 255 ) );
    gfxDrawRect( newX + 4, newY + 4, 200 - 8, 16, gfxColor( 0, 0, 0 ) );

    // // Draw the title text
    // gfxDrawString(
    //     newX + 6, newY + 6, windows[id].title, gfxColor( 255, 255,
    //     255 )
    // );
}

int wmGetWindowAt( int mouseX, int mouseY ) {
    for ( int i = 0; i < ( sizeof( struct Window ) / sizeof( windows[0] ) );
          i++ ) {
        if ( mouseX >= windows[i].x && mouseX <= windows[i].x + windows[i].w &&
             mouseY >= windows[i].y && mouseY <= windows[i].y + windows[i].h ) {
            return i;
        }
    }
    return -1;
}