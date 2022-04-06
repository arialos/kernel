#include "gdt.h"

struct gdt_entry	gdt[3];
struct gdt_ptr		gp;

extern void load_gdt(struct gdt_entry*);

void gdtSetGate(int number, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran) {
    gdt[number].base_low = (base & 0xFFFF);
    gdt[number].base_middle = (base >> 16) & 0xFF;
    gdt[number].base_high = (base >> 24) & 0xFF;

    gdt[number].limit_low = (limit & 0xFFFF);

    gdt[number].granularity |= gran & 0xF0;
    gdt[number].access = access;
}

void initGdt () {
    gp.limit = (sizeof (struct gdt_entry) * 6) - 1;
    gp.base = (uint32_t) &gdt;

    gdtSetGate(0, 0, 0, 0, 0);
    gdtSetGate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
    gdtSetGate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
    
    printf("[ GDT ] Debuging GDT gates\n");
    printf("[ GDT ] %d ", gdt[0]);
    printf("%x 0x", gdt[0].base_low);
    printf("%x 0x", gdt[0].base_middle);
    printf("%x 0x", gdt[0].base_high);
    printf("%x 0x", gdt[0].limit_low);
    printf("%x 0x", gdt[0].granularity);
    printf("%x 0x\n", gdt[0].access);

    printf("[ GDT ] %d ", gdt[1]);
    printf("%x 0x", gdt[1].base_low);
    printf("%x 0x", gdt[1].base_middle);
    printf("%x 0x", gdt[1].base_high);
    printf("%x 0x", gdt[1].limit_low);
    printf("%x 0x", gdt[1].granularity);
    printf("%x 0x\n", gdt[1].access);

    printf("[ GDT ] %d ", gdt[2]);
    printf("%x 0x", gdt[2].base_low);
    printf("%x 0x", gdt[2].base_middle);
    printf("%x 0x", gdt[2].base_high);
    printf("%x 0x", gdt[2].limit_low);
    printf("%x 0x", gdt[2].granularity);
    printf("%x 0x\n", gdt[2].access);

    load_gdt((struct gdt_entry*)&gp);
}