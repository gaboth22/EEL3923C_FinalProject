#ifndef TIMESOURCE_INTERRUPT1MSSYSTEMTICK_H
#define TIMESOURCE_INTERRUPT1MSSYSTEMTICK_H

#include "I_TimeSource.h"
#include "I_Interrupt.h"

I_TimeSource_t * TimeSource_Interrupt1MsSystemTick_Init(I_Interrupt_t *oneMsTick);

#endif
