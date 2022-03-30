#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

// http://www.strudel.org.uk/itoa/
char* itoa(int val, int base)
{
    static char buf[32] = {0};

    int neg = 0;

    if(val < 0)
    {
        val = -val;
        neg = 1;
    }

    int i = 30;

    do
    {
        buf[i] = "0123456789abcdef"[val % base];
        --i;
        val /= base;
    }
    while(val && i);

    if(neg)
    {
        buf[i] = '-';
    }

    return &buf[i+(neg?0:1)];
}