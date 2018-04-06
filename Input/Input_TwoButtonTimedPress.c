#include "Input_TwoButtonTimedPress.h"
#include "I_GpioGroup.h"
#include "EventSubscriber_Synchronous.h"
#include "utils.h"

static void PublishTimeElapsedEvent(void *context)
{
    RECAST(instance, context, Input_TwoButtonTimedPress_t *);
    Event_Publish(&instance->onTimedElapsed.interface, &instance->ticksAfterWhichTheTwoButtonPressMatters);
}

static void ButtonOnePressed(void *context, void *args)
{
    RECAST(instance, context, Input_TwoButtonTimedPress_t *);
    RECAST(buttonState, args, GpioState_t *);

    if(instance->timerStarted)
    {
        Timer_OneShot_Stop(&instance->pressPeriodTimer);
    }

    if(*buttonState == GpioState_Low)
    {
        instance->button1Pressed = true;

        if(instance->button2Pressed)
        {
            Timer_OneShot_Init(
                    &instance->pressPeriodTimer,
                    instance->timerModule,
                    instance->ticksAfterWhichTheTwoButtonPressMatters,
                    PublishTimeElapsedEvent,
                    instance);
            Timer_OneShot_Start(&instance->pressPeriodTimer);
            instance->timerStarted = true;
        }
    }
}

static void ButtonTwoPressed(void *context, void *args)
{
    RECAST(instance, context, Input_TwoButtonTimedPress_t *);
    RECAST(buttonState, args, GpioState_t *);

    if(instance->timerStarted)
    {
        Timer_OneShot_Stop(&instance->pressPeriodTimer);
    }

    if(*buttonState == GpioState_Low)
    {
        instance->button2Pressed = true;

        if(instance->button1Pressed)
        {
            Timer_OneShot_Init(
                    &instance->pressPeriodTimer,
                    instance->timerModule,
                    instance->ticksAfterWhichTheTwoButtonPressMatters,
                    PublishTimeElapsedEvent,
                    instance);
            Timer_OneShot_Start( &instance->pressPeriodTimer);
            instance->timerStarted = true;
        }
    }
}

static I_Event_t * GetOnChangeEvent(I_Input_t *_instance)
{
    RECAST(instance, _instance, Input_TwoButtonTimedPress_t *);
    return &instance->onTimedElapsed.interface;
}

static void Read(I_Input_t *_instance, void *buffer)
{
    IGNORE(_instance);
    IGNORE(buffer);
}

static const InputApi_t inputTwoButtonTimedPressApi =
    { GetOnChangeEvent, Read };

void Input_TwoButtonTimedPress_Init(
        Input_TwoButtonTimedPress_t *instance,
        I_Input_t *button1,
        I_Input_t *button2,
        TimerTicks_t ticksAfterWhichTheTwoButtonPressMatters,
        TimerModule_t *timerModule)
{
    instance->timerModule = timerModule;
    instance->interface.api = &inputTwoButtonTimedPressApi;
    instance->ticksAfterWhichTheTwoButtonPressMatters = ticksAfterWhichTheTwoButtonPressMatters;
    instance->timerStarted = false;
    instance->button1Pressed = false;
    instance->button2Pressed = false;
    Event_Synchronous_Init(&instance->onTimedElapsed);
    EventSubscriber_Synchronous_Init(&instance->butto1Subscriber, ButtonOnePressed, instance);
    EventSubscriber_Synchronous_Init(&instance->butto2Subscriber, ButtonTwoPressed, instance);
    Event_Subscribe(Input_GetOnChangeEvent(button1), &instance->butto1Subscriber.interface);
    Event_Subscribe(Input_GetOnChangeEvent(button2), &instance->butto2Subscriber.interface);
}
