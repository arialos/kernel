#ifndef SERIAL_H
#define SERIAL_H

#include <stdint.h>

int initSerial(void);
char readSerial(void);
void writeSerial(char a);
#endif