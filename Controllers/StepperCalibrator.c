#include "StepperCalibrator.h"
#include "Event_Synchronous.h"
#include "I_Input.h"
#include "I_Event.h"
#include "utils.h"

enum
{
    PeriodToAttempToMoveToHomePosition = 100
};

static void PublishCalibrationDoneEvent(void *context, void *args)
{
    IGNORE(args);
    RECAST(instance, context, StepperCalibrator_t *);

    Event_Unsubscribe(StepperMotor_GetOnStepsDoneEvent(instance->motorInCalibration), &instance->stepperStepsDoneSubscriber.interface);
    Event_Publish(&instance->onCalibrationDoneEvent.interface, NULL);
}

static void AttempToMoveToHomePosition(void *context)
{
    RECAST(instance, context, StepperCalibrator_t *);

    if(!StepperMotor_IsBusy(instance->motorInCalibration))
    {
        Timer_Periodic_Command(&instance->timerToMoveToHomePosition, Timer_Periodic_Command_Stop);
        Event_Subscribe(StepperMotor_GetOnStepsDoneEvent(instance->motorInCalibration), &instance->stepperStepsDoneSubscriber.interface);
        StepperMotor_DoStep(instance->motorInCalibration, StepDirection_Forward, instance->stepsToPlaceAtZeroPosition);
    }
}

static void StopStepperMotor(void *context, void *args)
{
    IGNORE(args);
    RECAST(instance, context, StepperCalibrator_t *);

    Event_Unsubscribe(Input_GetOnChangeEvent(instance->inputForHomePositionSensor), &instance->inputSensorSubscriber.interface);
    StepperMotor_Stop(instance->motorInCalibration);
    Timer_Periodic_Start(&instance->timerToMoveToHomePosition);
}

void StepperCalibrator_Calibrate(
        StepperCalibrator_t *instance,
        I_StepperMotor_t *stepperToCalibrate,
        I_Input_t *inputForHomePositionSensor,
        uint16_t enoughStepsForThreeRevolutions,
        uint16_t stepsToPlaceAtZeroPosition)
{
    instance->stepsToPlaceAtZeroPosition = stepsToPlaceAtZeroPosition;
    instance->inputForHomePositionSensor = inputForHomePositionSensor;
    instance->motorInCalibration = stepperToCalibrate;
    Event_Subscribe(Input_GetOnChangeEvent(inputForHomePositionSensor), &instance->inputSensorSubscriber.interface);
    StepperMotor_DoStep(stepperToCalibrate, StepDirection_Forward, enoughStepsForThreeRevolutions);
}

I_Event_t * StepperCalibrator_GetOnCalibrationDoneEvent(StepperCalibrator_t *instance)
{
    return &instance->onCalibrationDoneEvent.interface;
}

void StepperCalibrator_Init(StepperCalibrator_t *instance, TimerModule_t *timerModule)
{
    Timer_Periodic_Init(&instance->timerToMoveToHomePosition, timerModule, PeriodToAttempToMoveToHomePosition, AttempToMoveToHomePosition, instance);
    EventSubscriber_Synchronous_Init(&instance->inputSensorSubscriber, StopStepperMotor, instance);
    EventSubscriber_Synchronous_Init(&instance->stepperStepsDoneSubscriber, PublishCalibrationDoneEvent, instance);
    Event_Synchronous_Init(&instance->onCalibrationDoneEvent);
}
