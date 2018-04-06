#include "Application.h"
#include "HeartbeatLed.h"
#include "GpioTable.h"
#include "I_Adc.h"
#include "LcdDisplayController.h"
#include "SineWave.h"
#include "utils.h"

enum
{
    PeriodToToggleHeartbeatLedsMs = 200,
    PeriodToUpdateLcdDisplayMs = 500,
    EnoughStepsForThreeFullRevolutions = 12000,
    StepsToPlaceAtZeroPositionForMotorOne = 388,
    StepsToPlaceAtZeroPositionForMotorTwo = 377,
    MaxStepIndexCount = 20
};

typedef enum
{
    StepperOutputToChange_One = 0,
    StepperOutputToChange_Two = 1
} StepperOutputToChange_t;

static const uint8_t stepperOneStepsPerSubdivision[MaxStepIndexCount] =
    {
         171, 171, 171, 171, 171, 171, 171, 171, 171, 171,
         171, 171, 171, 171, 171, 171, 171, 171, 171, 171
    };

void Uint16ToString(uint16_t data, char *buffer)
{
    uint8_t byte3 = (data / 1000);
    uint8_t byte2 = ((data % 1000) / 100);
    uint8_t byte1 = (((data % 1000) % 100) / 10);
    uint8_t byte0 = (((data % 1000) % 100) % 10);

    buffer[0] = (byte3 + '0');
    buffer[1] = (byte2 + '0');
    buffer[2] = (byte1 + '0');
    buffer[3] = (byte0 + '0');
}

static void ReturnCursor(void *context)
{
    RECAST(instance, context, Application_t *);
    LcdDisplayController_SetCursorIndex(&instance->lcdDisplayController, 1, 0);
}

static void WriteToSecondLcdLine(void *context)
{
    RECAST(instance, context, Application_t *);
    char adcReadingStr[4];
    Uint16ToString(Adc_GetCounts(instance->adc), adcReadingStr);
    LcdDisplayController_WriteString(&instance->lcdDisplayController, adcReadingStr, 4);

    Timer_OneShot_Init(
            &instance->moveDisplayCursorTimer,
            instance->timerModule,
            20,
            ReturnCursor,
            instance);
    Timer_OneShot_Start(&instance->moveDisplayCursorTimer);
}

static void MoveCursorToSecondLcdLine(void *context)
{
    RECAST(instance, context, Application_t *);
    LcdDisplayController_SetCursorIndex(&instance->lcdDisplayController, 2, 0);

    Timer_OneShot_Init(
            &instance->moveDisplayCursorTimer,
            instance->timerModule,
            20,
            WriteToSecondLcdLine,
            instance);
    Timer_OneShot_Start(&instance->moveDisplayCursorTimer);
}

static void UpdateLcd(void *context)
{
    RECAST(instance, context, Application_t *);
    LcdDisplayController_WriteString(&instance->lcdDisplayController, "Hello there!", 12);

    Timer_OneShot_Init(
            &instance->moveDisplayCursorTimer,
            instance->timerModule,
            20,
            MoveCursorToSecondLcdLine,
            instance);
    Timer_OneShot_Start(&instance->moveDisplayCursorTimer);
}

//static void ToggleLed(void *context, void *args)
//{
//    RECAST(instance, context, Application_t *);
//    RECAST(gpioState, args, GpioState_t *);
//
//    if(*gpioState == GpioState_Low)
//    {
//        DacController_SendInputCode(&instance->dacController, instance->testDacValue++);
//        if(instance->testDacValue > 1023)
//        {
//            instance->testDacValue = 0;
//        }
//
//        instance->currentLedState = (GpioState_t)!instance->currentLedState;
//        GpioGroup_SetState(instance->gpioGroup, GpioStatusLed, instance->currentLedState);
//    }
//}

static void IncreaseStepperOutput(void *context, void *args)
{
    RECAST(instance, context, Application_t *);
    RECAST(gpioState, args, GpioState_t *);

    if(*gpioState == GpioState_Low)
    {
        if(instance->outputToChange == StepperOutputToChange_One)
        {
            if(!StepperMotor_IsBusy(instance->stepperMotorOne) &&
               instance->stepperOnePositionIndex < MaxStepIndexCount - 1)
            {
                if(instance->workoutStarted)
                {
                    if(instance->currentFrequency == 0)
                    {
                        WaveformGenerator_Start(
                                instance->waveformGenerator,
                                sineWave,
                                60);
                    }

                    instance->currentFrequency++;
                    WaveformGenerator_SetFrequencyInHz(instance->waveformGenerator, instance->currentFrequency);
                }

                instance->stepperOnePositionIndex++;
                StepperMotor_DoStep(
                    instance->stepperMotorOne,
                    StepDirection_Forward,
                    (uint16_t)stepperOneStepsPerSubdivision[instance->stepperOnePositionIndex]);
            }
        }
        else
        {
            if(!StepperMotor_IsBusy(instance->stepperMotorTwo) &&
               instance->stepperTwoPositionIndex < MaxStepIndexCount - 1)
            {
                instance->stepperTwoPositionIndex++;
                StepperMotor_DoStep(
                    instance->stepperMotorTwo,
                    StepDirection_Forward,
                    (uint16_t)stepperOneStepsPerSubdivision[instance->stepperTwoPositionIndex]);
            }
        }
    }
}

static void DecreaseStepperOutput(void *context, void *args)
{
    RECAST(instance, context, Application_t *);
    RECAST(gpioState, args, GpioState_t *);

    if(*gpioState == GpioState_Low)
    {
        if(instance->outputToChange == StepperOutputToChange_One)
        {
            if(!StepperMotor_IsBusy(instance->stepperMotorOne) &&
               instance->stepperOnePositionIndex >= 0)
            {
                if(instance->workoutStarted)
                {
                    instance->currentFrequency--;
                    if(instance->currentFrequency == 0)
                    {
                        WaveformGenerator_Stop(instance->waveformGenerator);
                    }
                    else
                    {
                        WaveformGenerator_SetFrequencyInHz(
                                instance->waveformGenerator,
                                instance->currentFrequency);
                    }
                }

                StepperMotor_DoStep(
                    instance->stepperMotorOne,
                    StepDirection_Backward,
                    (uint16_t)stepperOneStepsPerSubdivision[instance->stepperOnePositionIndex]);
                instance->stepperOnePositionIndex--;
            }
        }
        else
        {
            if(!StepperMotor_IsBusy(instance->stepperMotorTwo) &&
               instance->stepperTwoPositionIndex >= 0)
            {
                StepperMotor_DoStep(
                    instance->stepperMotorTwo,
                    StepDirection_Backward,
                    (uint16_t)stepperOneStepsPerSubdivision[instance->stepperTwoPositionIndex]);
                instance->stepperTwoPositionIndex--;
            }
        }
    }
}

static void SwitchBetweenStepperOneOrTwoControl(void *context, void *args)
{
    RECAST(instance, context, Application_t *);
    RECAST(gpioState, args, GpioState_t *);

    if(*gpioState == GpioState_Low)
    {
        instance->outputToChange = (StepperOutputToChange_t)!instance->outputToChange;
    }
}

static void BeginNormalOperation(void *context, void *args)
{
    RECAST(instance, context, Application_t *);
    IGNORE(args);
    LcdDisplayController_SetCursorIndex(&instance->lcdDisplayController, 1, 0);
    Event_Subscribe(Input_GetOnChangeEvent(instance->buttonOne), &instance->buttonOneSubscriber.interface);
    Event_Subscribe(Input_GetOnChangeEvent(instance->buttonTwo), &instance->buttonTwoSubscriber.interface);
    Event_Subscribe(Input_GetOnChangeEvent(instance->buttonThree), &instance->buttonThreeSubscriber.interface);
    Timer_Periodic_Start(&instance->writeToLcdTimer);
}

static void CalibrateStepperTwo(void *context, void *args)
{
    RECAST(instance, context, Application_t *);
    IGNORE(args);

    LcdDisplayController_WriteString(&instance->lcdDisplayController, "Calibrating...", 14);

    Event_Unsubscribe(
            StepperCalibrator_GetOnCalibrationDoneEvent(&instance->stepperCalibrator),
            &instance->calibrationDoneSub.interface);
    EventSubscriber_Synchronous_Init(&instance->calibrationDoneSub, BeginNormalOperation, instance);
    Event_Subscribe(
            StepperCalibrator_GetOnCalibrationDoneEvent(&instance->stepperCalibrator),
            &instance->calibrationDoneSub.interface);

    StepperCalibrator_Calibrate(
            &instance->stepperCalibrator,
            instance->stepperMotorTwo,
            instance->stepperTwoHomePositionSensor,
            EnoughStepsForThreeFullRevolutions,
            StepsToPlaceAtZeroPositionForMotorTwo);
}

void Application_Run(Application_t *instance)
{
    IGNORE(instance);
}

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
        TimerModule_t *timerModule)
{
    instance->timerModule = timerModule;
    instance->stepperMotorOne = stepperOne;
    instance->stepperMotorTwo = stepperTwo;
    instance->currentDirection = StepDirection_Forward;
    instance->buttonOne = buttonOne;
    instance->buttonTwo = buttonTwo;
    instance->buttonThree = buttonThree;
    instance->gpioGroup = gpioGroup;
    instance->stepperOneHomePositionSensor = stepperOneHomePositionSensor;
    instance->stepperTwoHomePositionSensor = stepperTwoHomePositionSensor;
    instance->adc = adc;
    instance->currentLedState = GpioState_Low;
    instance->outputToChange = StepperOutputToChange_One;
    instance->stepperOnePositionIndex = -1;
    instance->stepperTwoPositionIndex = -1;
    instance->currentFrequency = 0;
    instance->workoutStarted = true;

    DacController_Init(
            &instance->dacController,
            spi,
            gpioGroup,
            GpioSpiCs);

    instance->waveformGenerator = WaveformGenerator_Init(&instance->dacController);

    EventSubscriber_Synchronous_Init(&instance->buttonOneSubscriber, IncreaseStepperOutput, instance);
    EventSubscriber_Synchronous_Init(&instance->buttonTwoSubscriber, DecreaseStepperOutput, instance);
    EventSubscriber_Synchronous_Init(&instance->buttonThreeSubscriber, SwitchBetweenStepperOneOrTwoControl, instance);

    HeartbeatLed_Init(
            &instance->heartbeatLed,
            timerModule,
            GpioHeartbeatLed,
            gpioGroup,
            PeriodToToggleHeartbeatLedsMs);

    LcdDisplayController_Init(&instance->lcdDisplayController, lcdDisplay);

    Timer_Periodic_Init(
            &instance->writeToLcdTimer,
            timerModule,
            PeriodToUpdateLcdDisplayMs,
            UpdateLcd,
            instance);

    StepperCalibrator_Init(&instance->stepperCalibrator, timerModule);

    EventSubscriber_Synchronous_Init(&instance->calibrationDoneSub, CalibrateStepperTwo, instance);
    Event_Subscribe(
            StepperCalibrator_GetOnCalibrationDoneEvent(&instance->stepperCalibrator),
            &instance->calibrationDoneSub.interface);

    StepperCalibrator_Calibrate(
            &instance->stepperCalibrator,
            instance->stepperMotorOne,
            instance->stepperOneHomePositionSensor,
            EnoughStepsForThreeFullRevolutions,
            StepsToPlaceAtZeroPositionForMotorOne);
}
