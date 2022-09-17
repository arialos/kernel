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
Isr interrupt_handlers[256];

void registerInterruptHandler(uint8_t n, Isr handler)
{
    interrupt_handlers[n] = handler;
}

/*pointers to IRQ handlers in C*/
void *irq_routines[16] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0};

void interruptHandler(Registers reg)
{
    asm volatile("cli");
    // Send an EOI (end of interrupt) signal to the PICs.
    // If this interrupt involved the slave.
    if (reg.id >= 40)
    {
        // Send reset signal to slave.
        outb(0xA0, 0x20);
    }
    // Send reset signal to master. (As well as slave, if necessary).
    outb(0x20, 0x20);

    if (interrupt_handlers[reg.id] != 0)
    {
        Isr handler = interrupt_handlers[reg.id];
        handler(reg);
    }
    else if (reg.id < 32)
    {
        printf("[ EXCEPTION ] %s... Dumping Registers\n", exceptions[reg.id]);
        outb(0x20, 0x20);
    }
    asm volatile("sti");
}
