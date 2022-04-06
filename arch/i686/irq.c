#include "libio.h" 
#include "idt.h"
#include "isr.h"
#include "irq.h"
#include "gfx.h"

void remapPic() {
    outb(0x20, 17);
	outb(0xA0, 17);
	outb(0x21, 32);
	outb(0xA1, 64);
	outb(0x21, 4);
	outb(0xA1, 2);
	outb(0x21, 1);
	outb(0xA1, 1);
	outb(0x21, 0);
	outb(0xA1, 0); 
}

void registerIrqHandler(uint8_t irq, void (*handler)(struct regs_t* regs)) {
    int_callbacks[irq] = handler;
}

void irqHandler(struct regs_t regs) {
    void (*handler)(struct regs_t *r);

    if (regs.int_no >= 40) {
        outb(0xA0, 0x20);
    }
    outb(0x20, 0x20);
    
    if (int_callbacks[regs.int_no] != 0) {
        printf("[ INTERRUPTS ] IRQ: %d\n", regs.int_no);
        handler = int_callbacks[regs.int_no];
        handler(&regs);
    }

    printf("[ INTERRUPTS ] Unhandled IRQ: %d\n", regs.int_no);

}

void initIrq() {
    remapPic();

    idtSetGate(32, (uint32_t)irq0, 0x08, 0x8E);
    idtSetGate(33, (uint32_t)irq1, 0x08, 0x8E);
    idtSetGate(34, (uint32_t)irq2, 0x08, 0x8E);
    idtSetGate(35, (uint32_t)irq3, 0x08, 0x8E);
    idtSetGate(36, (uint32_t)irq4, 0x08, 0x8E);
    idtSetGate(37, (uint32_t)irq5, 0x08, 0x8E);
    idtSetGate(38, (uint32_t)irq6, 0x08, 0x8E);
    idtSetGate(39, (uint32_t)irq7, 0x08, 0x8E);
    idtSetGate(40, (uint32_t)irq8, 0x08, 0x8E);
    idtSetGate(41, (uint32_t)irq9, 0x08, 0x8E);
    idtSetGate(42, (uint32_t)irq10, 0x08, 0x8E);
    idtSetGate(43, (uint32_t)irq11, 0x08, 0x8E);
    idtSetGate(44, (uint32_t)irq12, 0x08, 0x8E);
    idtSetGate(45, (uint32_t)irq13, 0x08, 0x8E);
    idtSetGate(46, (uint32_t)irq14, 0x08, 0x8E);
    idtSetGate(47, (uint32_t)irq15, 0x08, 0x8E);

    asm volatile ("sti");
}