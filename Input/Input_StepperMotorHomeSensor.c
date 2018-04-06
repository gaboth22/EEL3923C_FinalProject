#include "Input_StepperMotorHomeSensor.h"
#include "I_GpioGroup.h"
#include "utils.h"

enum
{
    PeriodToPollSensorMs = 1,
    PeriodToStartPollingAfterMs = 500
};

static void PollSensor(void *context)
{
    RECAST(instance, context, Input_StepperMotorHomeSensor_t *);

    if(GpioGroup_GetState(instance->gpioGroup, instance->sensorChannel) ==  GpioState_High)
    {
        Event_Publish(&instance->onChangeEvent.interface, NULL);
    }
}

static I_Event_t * GetOnChangeEvent(I_Input_t *instance)
{
    RECAST(_instance, instance, Input_StepperMotorHomeSensor_t *);
    return &_instance->onChangeEvent.interface;
}

static void StartPolling(void *context)
{
    RECAST(instance, context, Input_StepperMotorHomeSensor_t *);
    Timer_Periodic_Start(&instance->pollSensorTimer);
}

const InputApi_t speedDisplaySensorInputApi =
    { GetOnChangeEvent };

void Input_StepperMotorHomeSensor_Init(
        Input_StepperMotorHomeSensor_t *instance,
        TimerModule_t *timerModule,
        I_GpioGroup_t *gpioGroup,
        GpioChannel_t sensorChannel)
{
    instance->interface.api = &speedDisplaySensorInputApi;
    instance->gpioGroup = gpioGroup;
    instance->sensorChannel = sensorChannel;
    Event_Synchronous_Init(&instance->onChangeEvent);
    Timer_Periodic_Init(&instance->pollSensorTimer, timerModule, PeriodToPollSensorMs, PollSensor, instance);
    Timer_OneShot_Init(&instance->timerToStartPolling, timerModule, PeriodToStartPollingAfterMs, StartPolling, instance);
    Timer_OneShot_Start(&instance->timerToStartPolling);
}
