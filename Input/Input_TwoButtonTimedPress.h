#ifndef INPUT_TWOBUTTONTIMEDPRESS_H
#define INPUT_TWOBUTTONTIMEDPRESS_H

#include "I_Input.h"
#include "Event_Synchronous.h"
#include "EventSubscriber_Synchronous.h"
#include "Timer_OneShot.h"
#include "TimerModule.h"

typedef struct
{
    I_Input_t interface;
    Event_Synchronous_t onTimedElapsed;
    Timer_OneShot_t pressPeriodTimer;
    TimerModule_t *timerModule;
    EventSubscriber_Synchronous_t butto1Subscriber;
    EventSubscriber_Synchronous_t butto2Subscriber;
    bool button1Pressed;
    bool button2Pressed;
    bool timerStarted;
    TimerTicks_t ticksAfterWhichTheTwoButtonPressMatters;
} Input_TwoButtonTimedPress_t;

/*
 * Initialize input
 */
void Input_TwoButtonTimedPress_Init(
        Input_TwoButtonTimedPress_t *instance,
        I_Input_t *button1,
        I_Input_t *button2,
        TimerTicks_t ticksAfterWhichTheTwoButtonPressMatters,
        TimerModule_t *timerModule);

#endif
