#include "window.h"

#include <stdint.h>

#include "gfx.h"
#include "heap.h"
#include "libio.h"
#include "mouse.h"
#include "pit.h"

Window *windows[];
// uint32_t frames = 0;
// uint32_t fps    = 0;

// Get the window at the given coordinates
Window *wmGetWindowAt(int x, int y) {
    for (int i = 0; i < (sizeof(Window) / sizeof(windows[0])); i++) {
        if (windows[i] != NULL) {
            if (x >= windows[i]->x && x <= windows[i]->x + windows[i]->w) {
                if (y >= windows[i]->y && y <= windows[i]->y + windows[i]->h) {
                    return windows[i];
                }
            }
        }
    }
    return NULL;
}

Window *wmCreateWindow(int w, int h, char *str) {
    Window *window = windows[(sizeof(Window) / sizeof(windows[0])) + 1];

    window->id = (sizeof(Window) / sizeof(windows[0]));
    window->x  = 10;
    window->y  = 10;

    window->w = w;
    window->h = h;

    window->title  = str;
    window->buffer = kmalloc(h * fbStride);

    return window;
}

void wmRedrawWindows() {
    gfxClearBuffer(gfxColor(0x00, 0x6E, 0x8D));

    // if (windows)
    //     for (int i = 0; i < (sizeof(Window) / sizeof(windows[0])); i++) {
    //         Window *window = windows[i];
    //         gfxRestoreTempBuffer(window->x, window->y, window->w, window->h, window->buffer);
    //     }

    gfxDrawRect(mouseX, mouseY, 20, 20, gfxColor(0x00, 0x6E, 0xFF));

    // // Draw the mouse cursor
    // gfxDrawCursor(mouseX, mouseY, gfxColor(0xFF, 0xFF, 0xFF));

    // if (timer_ticks % 18 == 0) {
    //     fps    = frames;
    //     frames = 0;
    // }
    // gfxPutStringAt(10, 10, "FPS: ");
    // gfxPutStringAt(82, 10, itoa(fps, 10));
    gfxSwapBuffers();
}

void initWindowManager(void) {
    for (;;) {
        // frames++;
        wmRedrawWindows();
    }
}