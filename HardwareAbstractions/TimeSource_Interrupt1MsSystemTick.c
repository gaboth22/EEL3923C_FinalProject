#include "TimeSource_Interrupt1MsSystemTick.h"
#include "I_Interrupt.h"
#include "I_TimeSource.h"

static I_TimeSource_t interruptTimeSource;

I_TimeSource_t * TimeSource_Interrupt1MsSystemTick_Init(I_Interrupt_t *oneMsTick)
{
    interruptTimeSource.onTimePeriod = oneMsTick->onInterrupt;

    return &interruptTimeSource;
}
