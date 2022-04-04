// #include <stdint.h>

// #include "libio.h"

// #include "isr.h"
// #include "gfx.h"

// static void keyboardHandler(struct regs_t *regs) {
//     (void) regs;
//     uint8_t scancode = inb(0x60);
//     printf("[ KEYBOARD ] Received key down: %x\n", scancode);
// }

// void initKeyboard (void) {
//     setInterruptHandler(IRQ(1), keyboardHandler);
//     printf("[ KEYBOARD ] Function address part 2: 0x%x\n", keyboardHandler);


//     printf("[ KEYBOARD ] Initializing keyboard... ");
//     while(1)
//         if ((inb(0x64) & 2) == 0) break;

//     outb(0x60, 0xF0);
//     outb(0x60, (uint8_t) 1);
//     printf("Success!\n");
    
//     printf("[ KEYBOARD ] Keyboard ready\n");
// }
