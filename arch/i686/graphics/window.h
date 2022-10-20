#ifndef WINDOW_H
#define WINDOW_H

#include <stdint.h>

struct Window {
    uint8_t id;
    uint32_t *backBuffer;
    uint32_t *windowBuffer;
    uint32_t w;
    uint32_t h;
    uint32_t x;
    uint32_t y;
    char title;
    uint32_t *parent;
    uint32_t *children[];
};

extern struct Window windows[];

void wmSpawnWindow( int x, int y, int w, int h, const char *title );
void wmRedrawWindow( int id, int newX, int newY );
int wmGetWindowAt( int mouseX, int mouseY );

#endif