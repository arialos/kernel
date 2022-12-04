#ifndef GDT_H
#define GDT_H

#include <stdint.h>

typedef struct GDTEntry {
    uint16_t limit;
    uint16_t base_low;
    uint8_t base_mid;
    uint8_t access;
    uint8_t granularity;
    uint8_t base_high;
} __attribute__((packed)) GDTEntry;

typedef struct GDTRegisters {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) GDTRegisters;

int gdtInit();

#endif  // GDT_H_INCLUDED
