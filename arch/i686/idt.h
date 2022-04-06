#ifndef IDT_H
#define IDT_H

#include <stdint.h>

struct idt_entry
{
    uint16_t baseLow;
    uint16_t selector;
    uint8_t reserved;
    uint8_t attributes;
    uint16_t baseHigh;
} __attribute__((packed));

struct idtr_t
{
    uint16_t limit;
    struct idt_entry* base;
} __attribute__((packed));

extern struct idt_entry idt[256];
extern struct idtr_t idtr;

void idtSetGate (uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);
void initIdt();

#endif