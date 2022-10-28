#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "gfx.h"

static char hex_table[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

static void printHex(unsigned int n) {
    unsigned mask  = 0xF0000000;
    unsigned shift = 28;
    while (mask) {
        gfxPutCharacter(hex_table[(n & mask) >> shift]);
        mask >>= 4;
        shift -= 4;
    }
}

int printf(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    while (*fmt) {
        char ch = *fmt++;
        switch (ch) {
            case '%': {
                switch (*fmt++) {
                    case 'c':
                        gfxPutCharacter(va_arg(ap, int));
                        break;
                    case 's':  // string
                        gfxPutString(va_arg(ap, const char *));
                        break;
                    case 'x':  // Hex
                        printHex(va_arg(ap, unsigned));
                        break;
                    case 'd':  // Int
                        gfxPutString(itoa(va_arg(ap, unsigned), 10));
                        break;
                    default:
                        gfxPutString("printf: unknown format\n");
                        break;
                }
                break;
            }
            default:
                gfxPutCharacter(ch);
                break;
        }
    }
    va_end(ap);
    return 0;
}

size_t strlen(const char *str) {
    size_t len = 0;
    while (str[len]) len++;
    return len;
}