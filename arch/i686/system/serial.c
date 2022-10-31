#include <stdint.h>

#include "libio.h"

char readSerial(void) {
    while (inb(0x3F8 + 5) & 1 == 0)
        ;
    return inb(0x3F8);
}

void writeSerial(char a) {
    while (inb(0x3F8 + 5) & 0x20 == 0)
        ;
    outb(0x3F8, a);
}

int initSerial(void) {
    outb(0x3F8 + 1, 0x00);  // Disable all interrupts
    outb(0x3F8 + 3, 0x80);  // Enable DLAB (set baud rate divisor)
    outb(0x3F8 + 0, 0x03);  // Set divisor to 3 (lo byte) 38400 baud
    outb(0x3F8 + 1, 0x00);  //                  (hi byte)
    outb(0x3F8 + 3, 0x03);  // 8 bits, no parity, one stop bit
    outb(0x3F8 + 2, 0xC7);  // Enable FIFO, clear them, with 14-byte threshold
    outb(0x3F8 + 4, 0x0B);  // IRQs enabled, RTS/DSR set
    outb(0x3F8 + 4, 0x1E);  // Set in loopback mode, test the serial chip
    outb(0x3F8 + 0, 0xAE);  // Test serial chip (send byte 0xAE and check if serial returns same byte)

    if (inb(0x3F8 + 0) != 0xAE) {
        return 1;
    }

    outb(0x3F8 + 4, 0x0F);  // Enable interrupts, RTS/DSR set

    return 0;
}
