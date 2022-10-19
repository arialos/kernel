#ifndef LIBMATH_H
#define LIBMATH_H

#define MIN( x, y ) ( ( x < y ) ? x : y )
#define MAX( x, y ) ( ( x > y ) ? x : y )
#define CLAMP( val, min, max )                                                 \
    ( ( val < min ) ? min : ( ( val > max ) ? max : val ) )

char *itoa( int val, int base );

#endif