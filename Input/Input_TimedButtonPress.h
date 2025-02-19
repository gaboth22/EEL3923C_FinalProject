#ifndef INPUT_TIMEDBUTTONPRESS_H
#define INPUT_TIMEDBUTTONPRESS_H

#include "I_Input.h"
#include "Event_Synchronous.h"
#include "EventSubscriber_Synchronous.h"
#include "TimerModule.h"
#include "Timer_OneShot.h"

typedef struct
{
    I_Input_t interface;
    Event_Synchronous_t onTimedPressEvent;
    EventSubscriber_Synchronous_t inputButtonSub;
    Timer_OneShot_t timedPressTimer;
    bool timerRunning;
} Input_TimedButtonPress_t;

void Input_TimedButtonPress_Init(
        Input_TimedButtonPress_t *instance,
        I_Input_t *button,
        TimerTicks_t periodToCountPressAfter,
        TimerModule_t *timerModule);

#endif
