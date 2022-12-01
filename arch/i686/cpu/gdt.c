#include "gdt.h"

GDTEntry gdt[5];
GDTRegisters GDTRegister;

extern void gdtFlush(uint32_t);

void setGDT(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    // Setup the descriptor base address
    gdt[num].baseLow  = base & 0xFFFF;
    gdt[num].baseMid  = (base >> 16) & 0xFF;
    gdt[num].baseHigh = (base >> 24) & 0xFF;

    // Setup the descriptor limits
    gdt[num].limit       = limit & 0xFFFF;
    gdt[num].granularity = (limit >> 16) & 0x0F;

    // Finally, set up the granularity and access flags
    gdt[num].granularity |= gran & 0xF0;
    gdt[num].access = access;
}

void gdtInit() {
    setGDT(0, 0, 0, 0, 0);
    setGDT(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    setGDT(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
    setGDT(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
    setGDT(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

    GDTRegister.limit = (sizeof(struct GDTEntry) * 5) - 1;
    GDTRegister.base  = &gdt;

    printf("\n[ GDT ] Base Low:    0x%x 0x%x 0x%x 0x%x 0x%x\n", gdt[0].baseLow, gdt[1].baseLow,
           gdt[2].baseLow, gdt[3].baseLow, gdt[4].baseLow);
    printf("[ GDT ] Base Mid:    0x%x 0x%x 0x%x 0x%x 0x%x\n", gdt[0].baseMid, gdt[1].baseMid, gdt[2].baseMid,
           gdt[3].baseMid, gdt[4].baseMid);
    printf("[ GDT ] Base High:   0x%x 0x%x 0x%x 0x%x 0x%x\n", gdt[0].baseHigh, gdt[1].baseHigh,
           gdt[2].baseHigh, gdt[3].baseHigh, gdt[4].baseHigh);
    printf("[ GDT ] Limit:       0x%x 0x%x 0x%x 0x%x 0x%x\n", gdt[0].limit, gdt[1].limit, gdt[2].limit,
           gdt[3].limit, gdt[4].limit);
    printf("[ GDT ] Access:      0x%x 0x%x 0x%x 0x%x 0x%x\n", gdt[0].access, gdt[1].access, gdt[2].access,
           gdt[3].access, gdt[4].access);
    printf("[ GDT ] Granularity: 0x%x 0x%x 0x%x 0x%x 0x%x\n", gdt[0].granularity, gdt[1].granularity,
           gdt[2].granularity, gdt[3].granularity, gdt[4].granularity);

    gdtFlush(&GDTRegister);

    printf("[ GDT ] Registers limit: 0x%x : 0x%x\n", GDTRegister.limit, sizeof(gdt) * 3 - 1);
    printf("[ GDT ] Registers base:  0x%x : 0x%x\n\n", GDTRegister.base, (uint32_t)&gdt);
}
