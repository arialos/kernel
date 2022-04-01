#include "gdt.h"

static void gdtSetGate(int idx, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    /* Setup the descriptor base address */
    gdt[idx].baseLow = (base & 0xFFFF);
    gdt[idx].baseMiddle = (base >> 16) & 0xFF;
    gdt[idx].baseHigh = (base >> 24) & 0xFF;

    /* Setup the descriptor limits */
    gdt[idx].limitLow = (limit & 0xFFFF);
    gdt[idx].granularity = ((limit >> 16) & 0x0F);

    /* Finally, set up the granularity and access flags */
    gdt[idx].granularity |= (gran & 0xF0);
    gdt[idx].access = access;
}

void gdtInit(void)
{
    gp.limit = sizeof(gdt) - 1;
    gp.base = (uint32_t)&gdt;

    gdtSetGate(0, 0, 0, 0, 0);
    gdtSetGate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    gdtSetGate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    // Push the GDT to the assembly code
    gdt_flush((uint32_t)&gp);
}