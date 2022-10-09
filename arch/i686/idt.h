#ifndef IDT_H
#define IDT_H

#include <stdint.h>

struct IDTEntry
{
    uint16_t low;
    uint16_t sel;
    uint8_t zero;
    uint8_t flags;
    uint16_t high;
} __attribute__((packed));

struct IDTRegisters
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

extern void idtFlush(uint32_t);

void idtInit();
void setIDT(uint8_t idx, uint32_t base, uint16_t sel, uint8_t flags);

#endif // IDT_H_INCLUDED
