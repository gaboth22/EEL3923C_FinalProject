#ifndef STEPPERCALIBRATOR_H
#define STEPPERCALIBRATOR_H

#include "Event_Synchronous.h"
#include "EventSubscriber_Synchronous.h"
#include "I_StepperMotor.h"
#include "I_Input.h"
#include "TimerModule.h"
#include "Timer_Periodic.h"

typedef struct
{
    Event_Synchronous_t onCalibrationDoneEvent;
    EventSubscriber_Synchronous_t inputSensorSubscriber;
    EventSubscriber_Synchronous_t stepperStepsDoneSubscriber;
    I_StepperMotor_t *motorInCalibration;
    Timer_Periodic_t timerToMoveToHomePosition;
    I_Input_t *inputForHomePositionSensor;
    uint16_t stepsToPlaceAtZeroPosition;
} StepperCalibrator_t;

/*
 * Operate the motor until it hits the home position
 * @param stepperToCalibrate The stepper motor to calibrate
 * @param inputForHomePositionSensor input for the home position sensor
 * @param enoughStepsForTwoRevolutions number of steps that will at least move it two full revolutions
 * @param stepsToPlaceAtZeroPosition number of steps to place at zero position after hitting home sensor
 */
void StepperCalibrator_Calibrate(
        StepperCalibrator_t *instance,
        I_StepperMotor_t *stepperToCalibrate,
        I_Input_t *inputForHomePositionSensor,
        uint16_t enoughStepsForThreeRevolutions,
        uint16_t stepsToPlaceAtZeroPosition);

/*
 * Get the event that fires when calibration is done
 */
I_Event_t * StepperCalibrator_GetOnCalibrationDoneEvent(StepperCalibrator_t *instance);

/*
 * Initialize the module
 */
void StepperCalibrator_Init(StepperCalibrator_t *instance, TimerModule_t *timerModule);

#endif
