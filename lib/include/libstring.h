#ifndef LIBSTRING_H
#define LIBSTRING_H

#include <stddef.h>

extern char *stringBuffer;

int printf(const char *fmt, ...);
size_t strlen(const char *str);

#endif