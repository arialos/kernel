#include "irq.h"

#include "idt.h"
#include "isr.h"
#include "libio.h"

// extern void *interrupt_handlers[256];

// Remap the PIC to prepare for the new interrupt handlers
void irqRemap() {
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x00);
    outb(0xA1, 0x00);
}

void irqInit() {
    irqRemap();
    setIDT(32, (uint32_t)_irq0, 0x08, 0x8E);
    setIDT(33, (uint32_t)_irq1, 0x08, 0x8E);
    setIDT(34, (uint32_t)_irq2, 0x08, 0x8E);
    setIDT(35, (uint32_t)_irq3, 0x08, 0x8E);
    setIDT(36, (uint32_t)_irq4, 0x08, 0x8E);
    setIDT(37, (uint32_t)_irq5, 0x08, 0x8E);
    setIDT(38, (uint32_t)_irq6, 0x08, 0x8E);
    setIDT(39, (uint32_t)_irq7, 0x08, 0x8E);
    setIDT(40, (uint32_t)_irq8, 0x08, 0x8E);
    setIDT(41, (uint32_t)_irq9, 0x08, 0x8E);
    setIDT(42, (uint32_t)_irq10, 0x08, 0x8E);
    setIDT(43, (uint32_t)_irq11, 0x08, 0x8E);
    setIDT(44, (uint32_t)_irq12, 0x08, 0x8E);
    setIDT(45, (uint32_t)_irq13, 0x08, 0x8E);
    setIDT(46, (uint32_t)_irq14, 0x08, 0x8E);
    setIDT(47, (uint32_t)_irq15, 0x08, 0x8E);
    setIDT(128, (uint32_t)_int0x80, 0x08, 0xBE);
}

void *irqRoutines[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void irqInstallHandler(int32_t irq, void (*handler)(Registers *regs)) {
    printf("[ IRQ ] Installing handler for IRQ %d\n", irq);
    irqRoutines[irq] = handler;
}

void irqUninstallHandler(int32_t irq) { irqRoutines[irq] = 0; }

void irqHandler(Registers *regs) {
    void (*handler)(Registers * reg);
    handler = irqRoutines[regs->id - 32];

    if (handler) {
        handler(regs);
    }
    // If this interrupt involved the slave.
    if (regs->id >= 40) {
        // Send reset signal to slave.
        outb(0xA0, 0x20);
    }
    // Send reset signal to master. (As well as slave, if necessary).
    outb(0x20, 0x20);
}