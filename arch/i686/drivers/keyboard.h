#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdbool.h>
#include <stdint.h>

#include "isr.h"

static void keyboardHandler( struct Registers *regs );
void initKeyboard( void );

#endif