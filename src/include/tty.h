#ifndef TTY_H
#define TTY_H

#include <stdint.h>

void initTerminal(void);
void setTerminalColor(uint8_t color);
void terminalPutChar(char c);
void printF(const char *data);

#endif