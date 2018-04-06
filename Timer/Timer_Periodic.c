#include "Timer_Periodic.h"
#include "utils.h"

static void CallTimerCallback(void *context, void *args)
{
    RECAST(timer, context, Timer_Periodic_t *);
    RECAST(currentTime, args, TimerTicks_t *);

    if(*currentTime - timer->startTicks >= timer->expiryTicks)
    {
        timer->startTicks = *currentTime;
        if(!timer->paused)
        {
            timer->callback(timer->context);
        }
    }
}

void Timer_Periodic_Start(Timer_Periodic_t *timer)
{
    timer->startTicks = timer->timerModule->currentTicks;
    Event_Subscribe(TimerModule_GetPeriodEvent(timer->timerModule), &timer->timerModuleSubscriber.interface);
}

void Timer_Periodic_Command(Timer_Periodic_t *timer, Timer_Periodic_Command_t commandToExecute)
{
    switch(commandToExecute)
    {
        case Timer_Periodic_Command_Pause:
            timer->paused = true;
            break;
        case Timer_Periodic_Command_Resume:
            timer->paused = false;
            break;
        case Timer_Periodic_Command_Stop:
            Event_Unsubscribe(TimerModule_GetPeriodEvent(timer->timerModule), &timer->timerModuleSubscriber.interface);
            break;
        default:
            break;
    }
}

void Timer_Periodic_UpdateExpiryTicks(Timer_Periodic_t *timer, TimerTicks_t newExpiryTicks)
{
    timer->expiryTicks = newExpiryTicks;
}

void Timer_Periodic_Init(
        Timer_Periodic_t *timer,
        TimerModule_t *timerModule,
        TimerTicks_t expiryTicks,
        Timer_PeriodicCallback_t callback,
        void *context)
{
    timer->timerModule = timerModule;
    timer->expiryTicks = expiryTicks;
    timer->callback = callback;
    timer->context = context;
    timer->paused = false;
    EventSubscriber_Synchronous_Init(&timer->timerModuleSubscriber, CallTimerCallback, timer);
}
