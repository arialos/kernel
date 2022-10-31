#include "pit.h"

#include <stdint.h>

#include "irq.h"

uint32_t timer_ticks = 0;

void setTimerPhase(int hz) {}

void timerInterruptHandler(struct Registers *regs) {
    (void)regs;
    // Increment our 'tick count'
    timer_ticks++;
    // // Every 18 clocks (approximately 1 second), we will display a message on the screen
    // if (timer_ticks % 18 == 0) {
    //     printf("One second has passed");
    // }
}

int initTimer(int freq) {
    // Firstly, register our timer callback.
    irqInstallHandler(0, timerInterruptHandler);

    int divisor = 1193180 / freq;  // Calculate our divisor
    outb(0x43, 0x36);  // Set our command byte 0x36
    outb(0x40, (divisor & 0xFF));  // Set low byte of divisor
    outb(0x40, ((divisor >> 8) & 0xFF));  // Set high byte of divisor

    return 0;
}