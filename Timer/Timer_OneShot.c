#include "Timer_OneShot.h"
#include "EventSubscriber_Synchronous.h"
#include "I_Event.h"
#include "TimerModule.h"
#include "utils.h"

static void CallTimerCallback(void *context, void *args)
{
    RECAST(timer, context, Timer_OneShot_t *);
    RECAST(currentTime, args, TimerTicks_t *);

    if(*currentTime - timer->startTicks >= timer->expiryTicks)
    {
        Event_Unsubscribe(TimerModule_GetPeriodEvent(timer->timerModule), &timer->timerModuleSubscriber.interface);
        timer->callback(timer->context);
    }
}

void Timer_OneShot_Start(Timer_OneShot_t *timer)
{
    timer->startTicks = timer->timerModule->currentTicks;
    Event_Subscribe(TimerModule_GetPeriodEvent(timer->timerModule), &timer->timerModuleSubscriber.interface);
}

void Timer_OneShot_Stop(Timer_OneShot_t *timer)
{
    Event_Unsubscribe(TimerModule_GetPeriodEvent(timer->timerModule), &timer->timerModuleSubscriber.interface);
}

void Timer_OneShot_UpdateExpiryTicks(Timer_OneShot_t *timer, TimerTicks_t newExpiryTicks)
{
    timer->expiryTicks = newExpiryTicks;
}

void Timer_OneShot_Init(
        Timer_OneShot_t *timer,
        TimerModule_t *timerModule,
        TimerTicks_t expiryTicks,
        Timer_OneShotCallback_t callback,
        void *context)
{
    timer->timerModule = timerModule;
    timer->expiryTicks = expiryTicks;
    timer->callback = callback;
    timer->context = context;
    EventSubscriber_Synchronous_Init(&timer->timerModuleSubscriber, CallTimerCallback, timer);
}
