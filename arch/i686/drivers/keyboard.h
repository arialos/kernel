#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "isr.h"

static void keyboardHandler(struct regs_t *regs);
void initKeyboard (void);

#endif