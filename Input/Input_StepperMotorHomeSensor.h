#ifndef INPUT_SPEEDDISPLAYMOTORHOMESENSOR_H
#define INPUT_SPEEDDISPLAYMOTORHOMESENSOR_H

#include "Event_Synchronous.h"
#include "I_Input.h"
#include "I_GpioGroup.h"
#include "TimerModule.h"
#include "Timer_Periodic.h"
#include "Timer_OneShot.h"

/*
 * The event will only publish the first time the
 * needle hits the home sensor, and then will stop
 * polling.
 */

typedef struct
{
    I_Input_t interface;
    Event_Synchronous_t onChangeEvent;
    Timer_Periodic_t pollSensorTimer;
    Timer_OneShot_t timerToStartPolling;
    TimerModule_t *timerModule;
    I_GpioGroup_t *gpioGroup;
    GpioChannel_t sensorChannel;
} Input_StepperMotorHomeSensor_t;

void Input_StepperMotorHomeSensor_Init(
        Input_StepperMotorHomeSensor_t *instance,
        TimerModule_t *timerModule,
        I_GpioGroup_t *gpioGroup,
        GpioChannel_t sensorChannel);

#endif
