#ifndef WINDOW_H
#define WINDOW_H

#include <stdint.h>

typedef struct Window {
    // Window properties
    uint16_t id;  // Window ID
    uint8_t *buffer;  // Framebuffer for the window

    // in future this will be determined from
    // the name of the process that created it
    char *title;  // Window title

    uint16_t w;  // Width of the window
    uint16_t h;  // Height of the window

    // Postion of the window
    int32_t x;  // Horizontal position from top-left
    int32_t y;  // Vertical position from top-left
    uint16_t z;  // Z-index. For controlling order of redraws
} Window;

Window *wmGetWindowAt(int x, int y);
Window *wmCreateWindow(int w, int h, char *str);

void initWindowManager(void);
#endif