#ifndef LIBMATH_H
#define LIBMATH_H

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define CLAMP(val, min, max) ((val < min) ? min : ((val > max) ? max : val))

char *itoa(int val, int base);

#endif