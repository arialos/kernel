#include "idt.h"
#include "isr.h"
#include "libio.h"

struct IDTEntry idt[256];
struct IDTRegisters IDTRegister;

void setIDT( uint8_t idx, uint32_t base, uint16_t sel, uint8_t flags ) {
    idt[idx].low   = base & 0xFFFF;
    idt[idx].high  = ( base >> 16 ) & 0xFFFF;
    idt[idx].zero  = 0;
    idt[idx].sel   = sel;
    idt[idx].flags = flags;
}

void idtInit() {
    IDTRegister.limit = ( sizeof( struct IDTEntry ) * 256 ) - 1;
    IDTRegister.base  = &idt;

    memset( &idt, 0, sizeof( struct IDTEntry ) * 256 );

    idtFlush( &IDTRegister );
}
