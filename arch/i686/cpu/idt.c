#include "idt.h"

#include "isr.h"
#include "libio.h"

IDTEntry idt[256];
IDTRegisters IDTRegister;

void setIDT(uint8_t idx, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[idx].low   = base & 0xFFFF;
    idt[idx].high  = (base >> 16) & 0xFFFF;
    idt[idx].zero  = 0;
    idt[idx].sel   = sel;
    idt[idx].flags = flags;
}

int idtInit(void) {
    IDTRegister.limit = (sizeof(IDTEntry) * 256) - 1;
    IDTRegister.base  = &idt;

    memset(&idt, 0, sizeof(IDTEntry) * 256);

    idtFlush(&IDTRegister);

    return 0;
}
