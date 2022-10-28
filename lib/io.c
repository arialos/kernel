#include <stddef.h>
#include <stdint.h>

void outb(uint16_t port, uint8_t val) { asm volatile("outb %1, %0" : : "dN"(port), "a"(val)); }

void outw(uint16_t port, uint16_t val) { asm volatile("outw %1, %0" : : "dN"(port), "a"(val)); }

uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "dN"(port));
    return ret;
}

uint16_t inw(uint16_t port) {
    uint16_t ret;
    asm volatile("inw %1, %0" : "=a"(ret) : "dN"(port));
    return ret;
}

void *memset(void *buf, int val, size_t sz) {
    for (size_t i = 0; i < sz; ++i) {
        ((uint8_t *)buf)[i] = (uint8_t)val;
    }
    return buf;
}

void *memcpy(void *dest, void *src, size_t sz) {
    while (sz--) *(char *)dest++ = *(char *)src++;
    return dest;
}

void assertFail(const char *func, const char *file, int line) {
    printf("\nAssertion failed in function %s in file %s, line %d\n", func, file, line);
}