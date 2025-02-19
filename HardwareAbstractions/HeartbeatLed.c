#include "HeartbeatLed.h"
#include "GpioTable.h"
#include "I_GpioGroup.h"
#include "TimerModule.h"
#include "Timer_Periodic.h"
#include "utils.h"

static void ToggleLed(void *context)
{
    RECAST(instance, context, HeartbeatLed_t *);

    instance->currentState = (GpioState_t)(!instance->currentState);
    GpioGroup_SetState(instance->gpioGroup, GpioHeartbeatLed, instance->currentState);
}

void HeartbeatLed_Init(
        HeartbeatLed_t *instance,
        TimerModule_t *timerModule,
        GpioChannel_t channelToToggle,
        I_GpioGroup_t *gpioGroup,
        TimerTicks_t togglePeriod)
{
    instance->channelToToggle = channelToToggle;
    instance->gpioGroup = gpioGroup;
    instance->currentState = GpioState_Low;
    Timer_Periodic_Init(&instance->heartbeatTimer, timerModule, togglePeriod / 2, ToggleLed, instance);
    Timer_Periodic_Start(&instance->heartbeatTimer);
}
