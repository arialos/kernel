#ifndef IDT_H
#define IDT_H

#include <stdint.h>

struct idt_entry {
    uint16_t baseLow;
    uint16_t select;
    uint8_t zero;
    uint8_t flags;
    uint16_t baseHigh;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

struct idt_entry idt[0x100];
struct idt_ptr idtPt;

void idtInit(void);

extern void idt_flush(uint32_t);

void idtSetGate(uint8_t idx, uint32_t base, uint16_t select, uint8_t flags);

#endif