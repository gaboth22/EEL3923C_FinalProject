#ifndef HEARTBEATLED_H
#define HEARTBEATLED_H

#include "I_GpioGroup.h"
#include "TimerModule.h"
#include "Timer_Periodic.h"
#include "types.h"

typedef struct
{
    Timer_Periodic_t heartbeatTimer;
    GpioState_t currentState;
    GpioChannel_t channelToToggle;
    I_GpioGroup_t *gpioGroup;
} HeartbeatLed_t;

/*
 * Initialize heartbeat LED
 * @param instance The instance
 * @param timerModule The timer module
 * @param channelToToggle GPIO channel to toggle
 * @param gpioGroup The GPIO group
 * @param togglePeriod period to toggle
 */
void HeartbeatLed_Init(
        HeartbeatLed_t *instance,
        TimerModule_t *timerModule,
        GpioChannel_t channelToToggle,
        I_GpioGroup_t *gpioGroup,
        TimerTicks_t togglePeriod);

#endif
