#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "include/vga.h"    
#include "include/tty.h"


void main(void)
{
    /* Initialize terminal interface */
    initTerminal();

    /* Newline support is left as an exercise. */
    printF("Hello, kernel World!\n");
}