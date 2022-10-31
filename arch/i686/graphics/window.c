#include "window.h"

#include <stdint.h>

#include "font.h"
#include "gfx.h"
#include "heap.h"
#include "libio.h"
#include "libstring.h"
#include "mouse.h"
#include "pit.h"

Window *windows[];
uint32_t frames = 0;
uint32_t fps    = 0;

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

    for (int i = 0; i < (sizeof(Window) / sizeof(windows[0])); i++) {
        Window *window = windows[i];

        gfxDrawBuffer(window->x, window->y + 20, window->w, window->h - 20, window->buffer);
    }

    // gfxDrawRect(mouseX, mouseY, 20, 20, gfxColor(0x00, 0x6E, 0xFF));
    gfxDrawLegacyBitmap(mouseX, mouseY, systemCursor, gfxColor(0xFF, 0xFF, 0xFF));

    // // Draw the mouse cursor
    // gfxDrawCursor(mouseX, mouseY, gfxColor(0xFF, 0xFF, 0xFF));

    if (timer_ticks % 50 == 0) {
        fps    = frames;
        frames = 0;
        printf("FPS: %d\n", fps);
    }
    frames++;
}

void initWindowManager(void) {
    // Create the first window
    wmCreateWindow(200, 200, "Window 1");
    for (;;) {
        if (!fSynced) return;
        wmRedrawWindows();
        gfxSwapBuffers();
    }
}