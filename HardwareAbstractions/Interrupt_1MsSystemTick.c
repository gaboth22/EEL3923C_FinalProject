#include <msp430.h>
#include "Interrupt_1MsSystemTick.h"
#include "Event_Synchronous.h"
#include "I_Interrupt.h"
#include "types.h"

enum
{
    PeriodForOneMs = 4000
};

static I_Interrupt_t oneMsTick;
static Event_Synchronous_t onInterrupt;

static void TimerA0_Init(uint16_t period)
{
    TAR = 0;
    TACCTL0 = CCIE;
    TACTL = TASSEL_2 | MC_1 | ID_2; // Dividing by 4 to get 4Mhz
    TACCR0 = period;
}

I_Interrupt_t * Interrupt_1MsSystemTick_Init(void)
{
    Event_Synchronous_Init(&onInterrupt);
    oneMsTick.onInterrupt = &onInterrupt.interface;
    TimerA0_Init(PeriodForOneMs);

    return &oneMsTick;
}

#pragma vector = TIMERA0_VECTOR
__interrupt void TA0_ISR(void)
{
    Event_Publish(&onInterrupt.interface, NULL);
}
