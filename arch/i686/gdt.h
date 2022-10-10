#ifndef GDT_H
#define GDT_H

#include <stdint.h>

struct GDTEntry {
    uint16_t limit;
    uint16_t baseLow;
    uint8_t baseMid;
    uint8_t access;
    uint8_t granularity;
    uint8_t baseHigh;
} __attribute__( ( packed ) );

struct GDTRegisters {
    uint16_t limit;
    uint32_t base;
} __attribute__( ( packed ) );

void gdtInit();

#endif // GDT_H_INCLUDED
