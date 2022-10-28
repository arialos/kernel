#include "libstring.h"

__attribute__((noreturn)) void panic(const char *msg) {
    printf("Kernel panic: %s", msg);
    for (;;)
        ;
}