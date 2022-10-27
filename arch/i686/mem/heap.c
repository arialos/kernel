#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "libio.h"
#include "libmath.h"
#include "libstring.h"

#include "paging.h"

/* a simple pointer-arithmetic-based malloc */

/* a symbol put there by the linker */
extern char link_mem_end;
uint32_t kmalloc_addr = (uint32_t)&link_mem_end;

static void *kmallocInt( size_t sz, int align, void **phys ) {
    if ( align ) {
        kmalloc_addr += ( PAGE_SIZE - 1 );
        kmalloc_addr &= PAGE_MASK;
    }
    if ( phys ) *phys = (void *)kmalloc_addr;
    void *ret = (void *)kmalloc_addr;
    kmalloc_addr += sz;
    /* make sure the address is aligned if requested */
    assert( !align || ( ( (uint32_t)ret & 0xFFF ) == 0 ) );
    return ret;
}

void *kmalloc( size_t sz ) {
    return kmallocInt( sz, 0, NULL );
}

void *kmallocA( size_t sz ) {
    return kmallocInt( sz, 1, NULL );
}

void *kmallocP( size_t sz, void **phys ) {
    return kmallocInt( sz, 0, phys );
}

void *kmallocAp( size_t sz, void **phys ) {
    return kmallocInt( sz, 1, phys );
}

void kmallocSetAddress( uint32_t addr ) {
    kmalloc_addr = addr;
}
