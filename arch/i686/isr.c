#include "isr.h"
#include "irq.h"
#include "gfx.h"
#include "libio.h"

char *exceptions[] = {
    "Divide by zero Error",
    "Debug",
    "Non-maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "Reserved (15)",
    "x87 Floating-Point Exception",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception",
    "Reserved (21)",
    "Reserved (22)",
    "Reserved (23)",
    "Reserved (24)",
    "Reserved (25)",
    "Reserved (26)",
    "Reserved (27)",
    "Reserved (28)",
    "Reserved (29)",
    "Security Exception",
    "Reserved (31)"};

void isrHandler(Registers reg)
{
    // printf("[ INTERRUPT ] %d\n", reg.id);
    if (reg.id >= 32)
    {
        if (reg.id == 33)
            printf("[ EXCEPTION ] Keyboard stuff\n");

        if (reg.id >= 40)
            outb(0xa0, 0x20);

        outb(0x20, 0x20);
    }
    else if (reg.id < 32)
    {
        printf("[ EXCEPTION ] %s\n", exceptions[reg.id]);
    }
}
