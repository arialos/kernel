#include "isr.h"
#include "irq.h"
#include "gfx.h"
#include "libio.h"
#include "idt.h"

void isrInit()
{
    setIDT(0, (uint32_t)_isr0, 0x08, 0x8E);
    setIDT(1, (uint32_t)_isr1, 0x08, 0x8E);
    setIDT(2, (uint32_t)_isr2, 0x08, 0x8E);
    setIDT(3, (uint32_t)_isr3, 0x08, 0x8E);
    setIDT(4, (uint32_t)_isr4, 0x08, 0x8E);
    setIDT(5, (uint32_t)_isr5, 0x08, 0x8E);
    setIDT(6, (uint32_t)_isr6, 0x08, 0x8E);
    setIDT(7, (uint32_t)_isr7, 0x08, 0x8E);
    setIDT(8, (uint32_t)_isr8, 0x08, 0x8E);
    setIDT(9, (uint32_t)_isr9, 0x08, 0x8E);
    setIDT(10, (uint32_t)_isr10, 0x08, 0x8E);
    setIDT(11, (uint32_t)_isr11, 0x08, 0x8E);
    setIDT(12, (uint32_t)_isr12, 0x08, 0x8E);
    setIDT(13, (uint32_t)_isr13, 0x08, 0x8E);
    setIDT(14, (uint32_t)_isr14, 0x08, 0x8E);
    setIDT(15, (uint32_t)_isr15, 0x08, 0x8E);
    setIDT(16, (uint32_t)_isr16, 0x08, 0x8E);
    setIDT(17, (uint32_t)_isr17, 0x08, 0x8E);
    setIDT(18, (uint32_t)_isr18, 0x08, 0x8E);
    setIDT(19, (uint32_t)_isr19, 0x08, 0x8E);
    setIDT(20, (uint32_t)_isr20, 0x08, 0x8E);
    setIDT(21, (uint32_t)_isr21, 0x08, 0x8E);
    setIDT(22, (uint32_t)_isr22, 0x08, 0x8E);
    setIDT(23, (uint32_t)_isr23, 0x08, 0x8E);
    setIDT(24, (uint32_t)_isr24, 0x08, 0x8E);
    setIDT(25, (uint32_t)_isr25, 0x08, 0x8E);
    setIDT(26, (uint32_t)_isr26, 0x08, 0x8E);
    setIDT(27, (uint32_t)_isr27, 0x08, 0x8E);
    setIDT(28, (uint32_t)_isr28, 0x08, 0x8E);
    setIDT(29, (uint32_t)_isr29, 0x08, 0x8E);
    setIDT(30, (uint32_t)_isr30, 0x08, 0x8E);
    setIDT(31, (uint32_t)_isr31, 0x08, 0x8E);
}

uint8_t *exceptionMessages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Corprocessor",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault Exception",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",
    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
};

void *isrRoutines[32] =
    {
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
};

void isrInstallHandler(int32_t isr, void (*handler)(struct Registers *regs))
{
    isrRoutines[isr] = handler;
}

void isrHandler(struct Registers *regs)
{
    if (isrRoutines[regs->id] != 0)
    {
        void (*handler)(struct Registers * regs) = isrRoutines[regs->id];
        handler(regs);
    }
    else
    {
        printf("Unhandled Interrupt: %d", regs->id);
        printf("%s", exceptionMessages[regs->id]);

        for (;;)
            ;
    }
}