#include <stdint.h>
#include <stddef.h>
#include "libstring.h"
#include "libio.h"
#include "idt.h"
#include "irq.h"
#include "isr.h"

/* in isr.c */
extern void *intCallbacks[256];

void remapIrq(void)
{
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);
}

void irqInit(void)
{
    remapIrq();
    idtSetGate(32, (uint32_t)_irq0, 0x08, 0x8E);
    idtSetGate(33, (uint32_t)_irq1, 0x08, 0x8E);
    idtSetGate(34, (uint32_t)_irq2, 0x08, 0x8E);
    idtSetGate(35, (uint32_t)_irq3, 0x08, 0x8E);
    idtSetGate(36, (uint32_t)_irq4, 0x08, 0x8E);
    idtSetGate(37, (uint32_t)_irq5, 0x08, 0x8E);
    idtSetGate(38, (uint32_t)_irq6, 0x08, 0x8E);
    idtSetGate(39, (uint32_t)_irq7, 0x08, 0x8E);
    idtSetGate(40, (uint32_t)_irq8, 0x08, 0x8E);
    idtSetGate(41, (uint32_t)_irq9, 0x08, 0x8E);
    idtSetGate(42, (uint32_t)_irq10, 0x08, 0x8E);
    idtSetGate(43, (uint32_t)_irq11, 0x08, 0x8E);
    idtSetGate(44, (uint32_t)_irq12, 0x08, 0x8E);
    idtSetGate(45, (uint32_t)_irq13, 0x08, 0x8E);
    idtSetGate(46, (uint32_t)_irq14, 0x08, 0x8E);
    idtSetGate(47, (uint32_t)_irq15, 0x08, 0x8E);
}

void irqHandler(struct regs_t regs)
{
    void (*handler)(struct regs_t r);
    handler = intCallbacks[regs.int_no];


    if(handler) {
        printf("[ IRQ ] Interrupt %d fired!\n", regs.int_no);
        handler(regs);
    }else {
        printf("[ IRQ ] Uncaught handler: 0x%x!\n", regs.int_no);
    }
    
    

    /* If the IDT entry that was invoked was greater than 40
     *  (meaning IRQ8 - 15), then we need to send an EOI to
     *  the slave controller */
    if (regs.int_no >= 40)
        outb(0xA0, 0x20);

    /* send an EOI to the master controller */
    outb(0x20, 0x20);
}