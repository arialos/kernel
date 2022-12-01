#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdbool.h>
#include <stdint.h>

#include "isr.h"

static void keyboardHandler(Registers *regs);
bool initKeyboard(void);

#endif