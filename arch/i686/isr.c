#include <stddef.h>
#include "libio.h"
#include "isr.h"
#include "idt.h"

void (*intCallbacks[256])(struct regs_t) = { NULL };

void setInterruptHandler (uint8_t n, void (*callback)(struct regs_t))
{
    printf ("[ ISR ] Setting handler for IRQ%d\n", n);
    intCallbacks[n] = callback;
}

void isrHandler(struct regs_t regs)
{
    if(intCallbacks[regs.int_no]) {
        printf("[ ISR ] Calling handler for interrupt: 0x%x\n", regs.int_no);
        intCallbacks[regs.int_no](regs);
    } else {
        printf("[ ISR ] Uncaught handler: 0x%x!\n", regs.int_no);
    }
}

void isrInit(void)
{
    idtSetGate(0, (uint32_t)_isr0, 0x08, 0x8E);
    idtSetGate(1, (uint32_t)_isr1, 0x08, 0x8E);
    idtSetGate(2, (uint32_t)_isr2, 0x08, 0x8E);
    idtSetGate(3, (uint32_t)_isr3, 0x08, 0x8E);
    idtSetGate(4, (uint32_t)_isr4, 0x08, 0x8E);
    idtSetGate(5, (uint32_t)_isr5, 0x08, 0x8E);
    idtSetGate(6, (uint32_t)_isr6, 0x08, 0x8E);
    idtSetGate(7, (uint32_t)_isr7, 0x08, 0x8E);
    idtSetGate(8, (uint32_t)_isr8, 0x08, 0x8E);
    idtSetGate(9, (uint32_t)_isr9, 0x08, 0x8E);
    idtSetGate(10, (uint32_t)_isr10, 0x08, 0x8E);
    idtSetGate(11, (uint32_t)_isr11, 0x08, 0x8E);
    idtSetGate(12, (uint32_t)_isr12, 0x08, 0x8E);
    idtSetGate(13, (uint32_t)_isr13, 0x08, 0x8E);
    idtSetGate(14, (uint32_t)_isr14, 0x08, 0x8E);
    idtSetGate(15, (uint32_t)_isr15, 0x08, 0x8E);
    idtSetGate(16, (uint32_t)_isr16, 0x08, 0x8E);
    idtSetGate(17, (uint32_t)_isr17, 0x08, 0x8E);
    idtSetGate(18, (uint32_t)_isr18, 0x08, 0x8E);
    idtSetGate(19, (uint32_t)_isr19, 0x08, 0x8E);
    idtSetGate(20, (uint32_t)_isr20, 0x08, 0x8E);
    idtSetGate(21, (uint32_t)_isr21, 0x08, 0x8E);
    idtSetGate(22, (uint32_t)_isr22, 0x08, 0x8E);
    idtSetGate(23, (uint32_t)_isr23, 0x08, 0x8E);
    idtSetGate(24, (uint32_t)_isr24, 0x08, 0x8E);
    idtSetGate(25, (uint32_t)_isr25, 0x08, 0x8E);
    idtSetGate(26, (uint32_t)_isr26, 0x08, 0x8E);
    idtSetGate(27, (uint32_t)_isr27, 0x08, 0x8E);
    idtSetGate(28, (uint32_t)_isr28, 0x08, 0x8E);
    idtSetGate(29, (uint32_t)_isr29, 0x08, 0x8E);
    idtSetGate(30, (uint32_t)_isr30, 0x08, 0x8E);
    idtSetGate(31, (uint32_t)_isr31, 0x08, 0x8E);
}