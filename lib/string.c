#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "libmath.h"
#include "serial.h"

static char hex_table[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

static void printHex(unsigned int n) {
    unsigned mask  = 0xF0000000;
    unsigned shift = 28;
    while (mask) {
        writeSerial(hex_table[(n & mask) >> shift]);
        // gfxPutCharacter(hex_table[(n & mask) >> shift]);
        mask >>= 4;
        shift -= 4;
    }
}

char *stringBuffer = "";

int printf(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    while (*fmt) {
        char ch = *fmt++;
        switch (ch) {
            case '%': {
                switch (*fmt++) {
                    case 'c':
                        writeSerial(va_arg(ap, int));
                        stringBuffer += va_arg(ap, int);
                        // gfxPutCharacter(va_arg(ap, int));
                        break;
                    case 's':  // string
                        // Loop over ap and print each character
                        for (char *s = va_arg(ap, char *); *s; s++) {
                            writeSerial(*s);
                            stringBuffer += *s;
                            // gfxPutCharacter(*s);
                        }
                        // gfxPutString(va_arg(ap, const char *));
                        break;
                    case 'x':  // Hex
                        unsigned mask  = 0xF0000000;
                        unsigned shift = 28;
                        while (mask) {
                            writeSerial(hex_table[(va_arg(ap, unsigned) & mask) >> shift]);
                            stringBuffer += hex_table[(va_arg(ap, unsigned) & mask) >> shift];

                            mask >>= 4;
                            shift -= 4;
                        }
                        break;
                    case 'd':  // Int
                        for (char *s = itoa(va_arg(ap, int), 10); *s; s++) {
                            writeSerial(*s);
                            stringBuffer += *s;
                            // gfxPutCharacter(*s);
                        }
                        break;
                    default:
                        for (char *s = va_arg(ap, char *); *s; s++) {
                            writeSerial(*s);
                            stringBuffer += *s;
                            // gfxPutCharacter(*s);
                        }
                        break;
                }
                break;
            }
            default:
                writeSerial(ch);
                stringBuffer += ch;
                // gfxPutCharacter(ch);
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