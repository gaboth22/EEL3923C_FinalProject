#ifndef APPLICATION_H
#define APPLICATION_H

#include "EventSubscriber_Synchronous.h"
#include "DacController.h"
#include "HeartbeatLed.h"
#include "I_Adc.h"
#include "I_GpioGroup.h"
#include "I_StepperMotor.h"
#include "I_Input.h"
#include "I_Spi.h"
#include "Input_TimedButtonPress.h"
#include "LcdDisplayController.h"
#include "StepperCalibrator.h"
#include "Timer_OneShot.h"
#include "Timer_Periodic.h"
#include "TimerModule.h"
#include "WaveformGenerator.h"

typedef struct
{
    HeartbeatLed_t heartbeatLed;
    LcdDisplayController_t lcdDisplayController;
    Timer_Periodic_t writeToLcdTimer;
    Timer_Periodic_t activeWorkoutLedTimer;
    Timer_OneShot_t moveDisplayCursorTimer;
    TimerModule_t *timerModule;
    I_StepperMotor_t *stepperMotorOne;
    I_StepperMotor_t *stepperMotorTwo;
    I_Input_t *buttonOne;
    I_Input_t *buttonTwo;
    I_Input_t *buttonThree;
    I_Input_t *stepperOneHomePositionSensor;
    I_Input_t *stepperTwoHomePositionSensor;
    I_GpioGroup_t *gpioGroup;
    I_Adc_t *adc;
    WaveformGenerator_t *waveformGenerator;
    DacController_t dacController;
    EventSubscriber_Synchronous_t buttonOneSubscriber;
    EventSubscriber_Synchronous_t buttonTwoSubscriber;
    EventSubscriber_Synchronous_t buttonThreeSubscriber;
    EventSubscriber_Synchronous_t calibrationDoneSub;
    EventSubscriber_Synchronous_t timedInputSub;
    StepDirection_t currentDirection;
    StepperCalibrator_t stepperCalibrator;
    Input_TimedButtonPress_t timedInput;
    GpioState_t currentLedState;
    int8_t stepperOnePositionIndex;
    int8_t stepperTwoPositionIndex;
    bool shouldOutputWave;
    uint16_t waveFrequencyHz;
    float waveAmplitudePercentage;
    char lcdDisplayLine1[16];
    char lcdDisplayLine2[16];
    uint8_t outputToChange;
    uint16_t workoutTime;
    bool updateTime;
    bool calibrationDone;
} Application_t;

/*
 * Initialize application
 */
void Application_Init(
        Application_t *instance,
        I_GpioGroup_t *gpioGroup,
        I_LcdDisplayParallel2Line_t *lcdDisplay,
        I_StepperMotor_t *stepperOne,
        I_StepperMotor_t *stepperTwo,
        I_Input_t *buttonOne,
        I_Input_t *buttonTwo,
        I_Input_t *buttonThree,
        I_Input_t *stepperOneHomePositionSensor,
        I_Input_t *stepperTwoHomePositionSensor,
        I_Adc_t *adc,
        I_Spi_t *spi,
        TimerModule_t *timerModule);

/*
 * To be placed in main loop
 */
void Application_Run(Application_t *instance);

#endif
