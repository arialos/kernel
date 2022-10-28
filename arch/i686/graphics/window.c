#include "window.h"

#include "gfx.h"
#include "heap.h"
#include "libio.h"
#include "mouse.h"

Window *windows[];

// Get the window at the given coordinates
Window *getWindowAt() {
    for (int i = 0; i < (sizeof(Window) / sizeof(windows[0])); i++) {
        if (windows[i] != NULL) {
            if (mouseX >= windows[i]->x && mouseX <= windows[i]->x + windows[i]->w) {
                if (mouseY >= windows[i]->y && mouseY <= windows[i]->y + windows[i]->h) {
                    return windows[i];
                }
            }
        }
    }
    return NULL;
}

void wmRedrawWindows() {
    gfxFramebufferClear(gfxColor(0x00, 0x6E, 0x8D));

    if (windows)
        for (int i = 0; i < (sizeof(Window) / sizeof(windows[0])); i++) {
            Window *window = windows[i];
            gfxRestoreTempBuffer(window->x, window->y, window->w, window->h, window->buffer);
        }

    // Draw the mouse cursor
    gfxDrawCursor(mouseX, mouseY, gfxColor(0xFF, 0xFF, 0xFF));

    gfxSwapBuffers();
}

void initWindowManager(void) {
    for (;;) {
        wmRedrawWindows();
    }
}