#include "idt.h"
#include "libio.h"
#include "isr.h"

 struct idt_entry idt[256];
 struct idtr_t idtr;

extern void load_idt(struct idt_entry*);

void idtSetGate (uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
    idt[num].baseLow = (base & 0xFFFF);
    idt[num].baseHigh = (base >> 16) & 0xFFFF;
    idt[num].selector = sel;
    idt[num].reserved = 0;
    idt[num].attributes = flags;
}

void initIdt() {
    idtSetGate(0, (uint32_t)isr0, 0x08, 0x8E);
    idtSetGate(1, (uint32_t)isr1, 0x08, 0x8E);
    idtSetGate(2, (uint32_t)isr2, 0x08, 0x8E);
    idtSetGate(3, (uint32_t)isr3, 0x08, 0x8E);
    idtSetGate(4, (uint32_t)isr4, 0x08, 0x8E);
    idtSetGate(5, (uint32_t)isr5, 0x08, 0x8E);
    idtSetGate(6, (uint32_t)isr6, 0x08, 0x8E);
    idtSetGate(7, (uint32_t)isr7, 0x08, 0x8E);
    idtSetGate(8, (uint32_t)isr8, 0x08, 0x8E);
    idtSetGate(9, (uint32_t)isr9, 0x08, 0x8E);
    idtSetGate(10, (uint32_t)isr10, 0x08, 0x8E);
    idtSetGate(11, (uint32_t)isr11, 0x08, 0x8E);
    idtSetGate(12, (uint32_t)isr12, 0x08, 0x8E);
    idtSetGate(13, (uint32_t)isr13, 0x08, 0x8E);
    idtSetGate(14, (uint32_t)isr14, 0x08, 0x8E);
    idtSetGate(15, (uint32_t)isr15, 0x08, 0x8E);
    idtSetGate(16, (uint32_t)isr16, 0x08, 0x8E);
    idtSetGate(17, (uint32_t)isr17, 0x08, 0x8E);
    idtSetGate(18, (uint32_t)isr18, 0x08, 0x8E);
    idtSetGate(19, (uint32_t)isr19, 0x08, 0x8E);
    idtSetGate(20, (uint32_t)isr20, 0x08, 0x8E);
    idtSetGate(21, (uint32_t)isr21, 0x08, 0x8E);
    idtSetGate(22, (uint32_t)isr22, 0x08, 0x8E);
    idtSetGate(23, (uint32_t)isr23, 0x08, 0x8E);
    idtSetGate(24, (uint32_t)isr24, 0x08, 0x8E);
    idtSetGate(25, (uint32_t)isr25, 0x08, 0x8E);
    idtSetGate(26, (uint32_t)isr26, 0x08, 0x8E);
    idtSetGate(27, (uint32_t)isr27, 0x08, 0x8E);
    idtSetGate(28, (uint32_t)isr28, 0x08, 0x8E);
    idtSetGate(29, (uint32_t)isr29, 0x08, 0x8E);
    idtSetGate(30, (uint32_t)isr30, 0x08, 0x8E);
    idtSetGate(31, (uint32_t)isr31, 0x08, 0x8E);

    idtr.limit = sizeof (idt) - 1;
    idtr.base = (struct idt_entry*) &idt;

    load_idt((struct idt_entry*) &idtr);
}