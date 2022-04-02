#include <stdint.h>

#include "libio.h"

#include "isr.h"
#include "gfx.h"

static void keyboardHandler(struct regs_t *regs) {
    (void) regs;
    uint8_t scancode = inb(0x60);
    printf("[ KEYBOARD ] Received key down: %x\n", scancode);
}

void initKeyboard (void) {
    setInterruptHandler(IRQ(1), keyboardHandler);
    
    printf("[ KEYBOARD ] Keyboard initialized\n");
}
