#include "pit.h"

#include <stdint.h>

#include "irq.h"

uint32_t timer_ticks = 0;

void setTimerPhase(int hz) {
    int divisor = 1193180 / hz;  // Calculate our divisor
    outb(0x43, 0x36);  // Set our command byte 0x36
    outb(0x40, divisor & 0xFF);  // Set low byte of divisor
    outb(0x40, divisor >> 8);  // Set high byte of divisor
}

void timerInterruptHandler(struct Registers *regs) {
    // Increment our 'tick count'
    timer_ticks++;
    // // Every 18 clocks (approximately 1 second), we will display a message on the screen
    // if (timer_ticks % 18 == 0) {
    //     printf("One second has passed");
    // }
}

void initTimer() {
    // Firstly, register our timer callback.
    irqInstallHandler(0, timerInterruptHandler);
    // Secondly, install the timer into IRQ0
    setTimerPhase(100);
}