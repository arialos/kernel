#include "isr.h"
#include "gfx.h"

static void (*int_callbacks[256])(struct regs_t*) = { NULL };

void registerIsrHandler(uint8_t irq, void (*handler)(struct regs_t* regs)) {
    int_callbacks[irq] = handler;
}

void isrHandler(struct regs_t regs) {
    void (*handler)(struct regs_t *r);

    if (int_callbacks[regs.int_no] !=0) {
        printf("[ INTERRUPTS ] IRQ: %d\n", regs.int_no);
        handler = int_callbacks[regs.int_no];
        handler(&regs);
        return;
    }

    printf("[ INTERRUPTS ] Unhandled IRQ: %d\n", regs.int_no);
}