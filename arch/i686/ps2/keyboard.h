#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdbool.h>
#include <stdint.h>

#include "isr.h"

extern char* pressed_keys;

static void keyboardHandler(Registers* regs);
int initKeyboard(void);

#endif