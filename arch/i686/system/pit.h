#ifndef PIT_H
#define PIT_H

#include <stdint.h>

#include "irq.h"

extern uint32_t timer_ticks;

int initTimer(int freq);

#endif