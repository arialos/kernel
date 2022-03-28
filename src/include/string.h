#ifdef STRING_H
#define STRING_H

#include <stddef.h>

void* memset(void *buf, int val, size_t sz);
void* memcpy(void *dest, void *src, size_t sz);
size_t strlen(const char *str);

#endif