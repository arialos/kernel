#ifndef IRQ_H
#define IRQ_H

#include <stdint.h>
#include "isr.h"

extern void _irq0(void);
extern void _irq1(void);
extern void _irq2(void);
extern void _irq3(void);
extern void _irq4(void);
extern void _irq5(void);
extern void _irq6(void);
extern void _irq7(void);
extern void _irq8(void);
extern void _irq9(void);
extern void _irq10(void);
extern void _irq11(void);
extern void _irq12(void);
extern void _irq13(void);
extern void _irq14(void);
extern void _irq15(void);
extern void _int0x80(void);

void irqInit();
void irqInstallHandler(int32_t irq, void (*handler)(struct Registers *regs));
void irqUninstallHandler(int32_t irq);
void irqHandler(struct Registers *regs);

#endif // IRQ_H_INCLUDED
