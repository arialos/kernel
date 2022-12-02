#include "fpu.h"

#include <stddef.h>

int enableFpu(void) {
    asm volatile("clts");
    size_t t;
    asm volatile("mov %%cr4, %0" : "=r"(t));
    t |= 3 << 9;
    asm volatile("mov %0, %%cr4" ::"r"(t));

    return 0;
}

int disableFpu(void) {
    size_t t;
    asm volatile("mov %%cr0, %0" : "=r"(t));
    t |= 1 << 3;
    asm volatile("mov %0, %%cr0" ::"r"(t));

    return 0;
}
