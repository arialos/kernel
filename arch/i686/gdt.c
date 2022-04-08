#include "gdt.h"
#include "gfx.h"

GDT gdt[3];
GDTRegister gdtReg;

extern void gdtFlush(GDTRegister *gdtReg);

void setGDT(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    gdt[num].baseLow = base & 0xffff;
    gdt[num].baseMid = (base >> 16) & 0xff;
    gdt[num].baseHigh = (base >> 24) & 0xff;
    gdt[num].limit = limit & 0xffff;
    gdt[num].granularity = (limit >> 16) & 0x0f;
    gdt[num].granularity |= gran & 0xf0;
    gdt[num].access = access;
}

void gdtInit()
{
    setGDT(0, 0, 0, 0, 0);
    setGDT(1, 0, 0xffffffff, 0x9a, 0xcf);
    setGDT(2, 0, 0xffffffff, 0x92, 0xcf);

    gdtReg.limit = sizeof(GDT) * 3 - 1;
    gdtReg.base = (uint32_t)&gdt;

    printf("[ GDT ] Debuging GDT gates\n");
    printf("[ GDT ] %d ", gdt[0]);
    printf("0x%x ", gdt[0].baseLow);
    printf("0x%x ", gdt[0].baseMid);
    printf("0x%x ", gdt[0].baseHigh);
    printf("0x%x ", gdt[0].limit);
    printf("0x%x ", gdt[0].granularity);
    printf("0x%x \n", gdt[0].access);

    printf("[ GDT ] %d ", gdt[1]);
    printf("0x%x ", gdt[1].baseLow);
    printf("0x%x ", gdt[1].baseMid);
    printf("0x%x ", gdt[1].baseHigh);
    printf("0x%x ", gdt[1].limit);
    printf("0x%x ", gdt[1].granularity);
    printf("0x%x \n", gdt[1].access);

    printf("[ GDT ] %d ", gdt[2]);
    printf("0x%x ", gdt[2].baseLow);
    printf("0x%x ", gdt[2].baseMid);
    printf("0x%x ", gdt[2].baseHigh);
    printf("0x%x ", gdt[2].limit);
    printf("0x%x ", gdt[2].granularity);
    printf("0x%x \n", gdt[2].access);

    gdtFlush(&gdtReg);
}
