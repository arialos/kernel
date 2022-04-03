#include <stdint.h>

#include "libio.h"

#include "idt.h"
#include "gfx.h"

struct idt_entry idt[0x100];
struct idt_ptr idtPt;

void idtSetGate(uint8_t idx, uint32_t base, uint16_t sel, uint8_t flags)
{
    idt[idx].baseLow = base & 0xFFFF;
    idt[idx].baseHigh = base >> 16;
    idt[idx].select = sel;
    idt[idx].zero = 0;
    idt[idx].flags = flags;
}

void idtInit(void)
{
    
    printf("[ IDT ] Initializing IDT... ");
    idtPt.limit = sizeof(idt) - 1;
    idtPt.base = (uint32_t)&idt;

    memset(&idt, 0, sizeof(idt));

    idt_flush((uint32_t)&idtPt);
    printf("Success! 0x%x\n", idtPt.base);
}