#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>
#include <stdbool.h>

#include "isr.h"

struct KeyEvent
{
    uint8_t scancode;
    uint8_t key;
    bool shift;
    bool ctrl;
    bool alt;
};

static void keyboardHandler(struct Registers *regs);
void initKeyboard(void);

#endif