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
    // Cast the buffer pointer to uint32_t *
    uint32_t *d = (uint32_t *)buf;
    // Set the value to be copied in a 32-bit integer
    uint32_t v = (val & 0xff) | ((val & 0xff) << 8) | ((val & 0xff) << 16) | ((val & 0xff) << 24);

    // Set data in blocks of 16 bytes (4 words)
    while (sz >= 16) {
        // Unroll the inner loop
        *d++ = v;
        *d++ = v;
        *d++ = v;
        *d++ = v;
        sz -= 16;
    }

    // Set any remaining words individually
    while (sz >= 4) {
        *d++ = v;
        sz -= 4;
    }

    // Set any remaining bytes individually
    char *d2 = (char *)d;
    while (sz--) *d2++ = (char)val;

    return buf;
}

void *memcpy(void *dest, const void *src, size_t n) {
    // Cast the destination and source pointers to uint32_t *
    uint32_t *d       = (uint32_t *)dest;
    const uint32_t *s = (const uint32_t *)src;

    // Copy data in blocks of 16 bytes (4 words)
    while (n >= 16) {
        // Unroll the inner loop
        *d++ = *s++;
        *d++ = *s++;
        *d++ = *s++;
        *d++ = *s++;
        n -= 16;
    }

    // Copy any remaining words individually
    while (n >= 4) {
        *d++ = *s++;
        n -= 4;
    }

    // Copy any remaining bytes individually
    char *d2       = (char *)d;
    const char *s2 = (const char *)s;
    while (n--) *d2++ = *s2++;

    return dest;
}

void assertFail(const char *func, const char *file, int line) {
    printf("\nAssertion failed in function %s in file %s, line %d\n", func, file, line);
}