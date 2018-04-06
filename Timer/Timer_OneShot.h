#ifndef TIMER_ONESHOT_H
#define TIMER_ONESHOT_H

#include "TimerModule.h"
#include "EventSubscriber_Synchronous.h"

typedef void (*Timer_OneShotCallback_t)(void *context);

typedef struct
{
    EventSubscriber_Synchronous_t timerModuleSubscriber;
    TimerModule_t *timerModule;
    TimerTicks_t expiryTicks;
    TimerTicks_t startTicks;
    Timer_OneShotCallback_t callback;
    void *context;
} Timer_OneShot_t;


/*
 * Initializes the one-shot timer.
 * @param timer The timer instance
 * @param timerModule the timer module
 * @param expiryTicks ticks after which the timer will expire
 * @param callback to be called when ticks expire
 * @param context to be passed to callback
 */
void Timer_OneShot_Init(
        Timer_OneShot_t *timer,
        TimerModule_t *timerModule,
        TimerTicks_t expiryTicks,
        Timer_OneShotCallback_t callback,
        void *context);

/*
 * Start the timer
 */
void Timer_OneShot_Start(Timer_OneShot_t *timer);

/*
 * Stop the timer
 */
void Timer_OneShot_Stop(Timer_OneShot_t *timer);

/*
 * Update expiry ticks for timer
 */
void Timer_OneShot_UpdateExpiryTicks(Timer_OneShot_t *timer, TimerTicks_t newExpiryTicks);

#endif
