#ifndef LIBIO_H
#define LIBIO_H

#include <stddef.h>
#include <stdint.h>

uint8_t inb(uint16_t port);
void outb (uint16_t port, uint8_t val);
void* memset(void *buf, int val, size_t sz);
void* memcpy(void *dest, void *src, size_t sz);

#endif