#ifndef HEAP_H
#define HEAP_H

#include <stddef.h>
#include <stdint.h>

static void *kmallocInt(size_t sz, int align, void **phys);

void *kmalloc(size_t sz);
void *kmallocA(size_t sz);
void *kmallocP(size_t sz, void **phys);
void *kmallocAp(size_t sz, void **phys);
void kmallocSetAddress(uint32_t addr);

#endif