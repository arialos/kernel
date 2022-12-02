#ifndef IDT_H
#define IDT_H

#include <stdint.h>

typedef struct IDTEntry {
    uint16_t low;
    uint16_t sel;
    uint8_t zero;
    uint8_t flags;
    uint16_t high;
} __attribute__((packed)) IDTEntry;

typedef struct IDTRegisters {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) IDTRegisters;

extern void idtFlush(uint32_t);

void setIDT(uint8_t idx, uint32_t base, uint16_t sel, uint8_t flags);
int idtInit();

#endif  // IDT_H_INCLUDED
