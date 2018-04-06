#ifndef TIMER_PERIODIC_H
#define TIMER_PERIODIC_H

#include "TimerModule.h"
#include "EventSubscriber_Synchronous.h"
#include "types.h"

typedef void (*Timer_PeriodicCallback_t)(void *context);

typedef enum
{
    Timer_Periodic_Command_Pause = 0,
    Timer_Periodic_Command_Resume,
    Timer_Periodic_Command_Stop
} Timer_Periodic_Command_t;

typedef struct
{
    EventSubscriber_Synchronous_t timerModuleSubscriber;
    TimerModule_t *timerModule;
    TimerTicks_t expiryTicks;
    TimerTicks_t startTicks;
    Timer_PeriodicCallback_t callback;
    void *context;
    bool paused;
} Timer_Periodic_t;

/*
 * Initializes the preiodic timer.
 * @param timer The timer instance
 * @param timerModule the timer module
 * @param expiryTicks ticks after which the timer will expire
 * @param callback to be called when ticks expire
 * @param context to be passed to callback
 */
void Timer_Periodic_Init(
        Timer_Periodic_t *timer,
        TimerModule_t *timerModule,
        TimerTicks_t expiryTicks,
        Timer_PeriodicCallback_t callback,
        void *context);

/*
 * Start the timer
 */
void Timer_Periodic_Start(Timer_Periodic_t *timer);

/*
 * Have timer pause, resume or stop
 */
void Timer_Periodic_Command(Timer_Periodic_t *timer, Timer_Periodic_Command_t commandToExecute);

/*
 * Updates the expiry period of the timer
 */
void Timer_Periodic_UpdateExpiryTicks(Timer_Periodic_t *timer, TimerTicks_t newExpiryTicks);

#endif
