#include "Application.h"
#include "HeartbeatLed.h"
#include "GpioTable.h"
#include "I_Adc.h"
#include "LcdDisplayController.h"
#include "SineWave.h"
#include "utils.h"

enum
{
    PeriodToToggleHeartbeatLedsMs = 400,
    PeriodToToggleActiveWorkoutLedMs = 50,
    PeriodToUpdateLcdDisplayMs = 500,
    EnoughStepsForThreeFullRevolutions = 12000,
    StepsToPlaceAtZeroPositionForMotorOne = 388,
    StepsToPlaceAtZeroPositionForMotorTwo = 377,
    MaxStepIndexCount = 20,
    PeriodToCountTimedPressAfterMs = 1500
};

typedef enum
{
    StepperOutputToChange_One = 0,
    StepperOutputToChange_Two = 1
} StepperOutputToChange_t;

static const uint8_t stepperStepsPerSubdivision[MaxStepIndexCount] =
    {
         171, 171, 171, 171, 171, 171, 171, 171, 171, 171,
         171, 171, 171, 171, 171, 171, 171, 171, 171, 171
    };

static void StartCalibrationCycle(Application_t *instance);

static void Uint8To3LenghtString(uint8_t data, char *buffer)
{
    uint8_t byte2 = (data / 100);
    uint8_t byte1 = ((data % 100) / 10);
    uint8_t byte0 = (data % 10);

    buffer[0] = (byte2 + '0');
    buffer[1] = (byte1 + '0');
    buffer[2] = (byte0 + '0');
}

static void Uint8To2LenghtString(uint8_t data, char *buffer)
{
    uint8_t byte1 = (data / 10);
    uint8_t byte0 = (data % 10);

    buffer[0] = (byte1 + '0');
    buffer[1] = (byte0 + '0');
}

static void ReturnCursor(void *context)
{
    RECAST(instance, context, Application_t *);
    LcdDisplayController_SetCursorIndex(&instance->lcdDisplayController, 1, 0);
}

static void WriteToSecondLcdLine(void *context)
{
    RECAST(instance, context, Application_t *);

    LcdDisplayController_WriteString(
            &instance->lcdDisplayController,
            instance->lcdDisplayLine2,
            16);
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

static uint8_t GetHr(Application_t *instance)
{
    return (uint8_t)(Adc_GetCounts(instance->adc) / 8) + 60;
}

static void GetTimeAsString(Application_t *instance, char *time)
{
    uint8_t minutes = (uint8_t)(instance->workoutTime / 60);
    uint8_t seconds = (uint8_t)(instance->workoutTime % 60);
    Uint8To2LenghtString(minutes, time);
    time[2] = ':';
    Uint8To2LenghtString(seconds, &time[3]);
}

static void UpdateLcd(void *context)
{
    RECAST(instance, context, Application_t *);

    if(instance->shouldOutputWave)
    {
        if(instance->updateTime)
        {
            instance->workoutTime++;
        }
        instance->updateTime = !instance->updateTime;

        char hr[3];
        char time[5];
        Uint8To3LenghtString(GetHr(instance), hr);
        GetTimeAsString(instance, time);
        memcpy(instance->lcdDisplayLine1, "TIME:00:00______", 16);
        memcpy(instance->lcdDisplayLine2, "BPM: 000", 7);
        instance->lcdDisplayLine1[5] = time[0];
        instance->lcdDisplayLine1[6] = time[1];
        instance->lcdDisplayLine1[7] = time[2];
        instance->lcdDisplayLine1[8] = time[3];
        instance->lcdDisplayLine1[9] = time[4];
        instance->lcdDisplayLine2[5] = hr[0];
        instance->lcdDisplayLine2[6] = hr[1];
        instance->lcdDisplayLine2[7] = hr[2];
    }
    else
    {
        if(instance->calibrationDone)
        {
            memcpy(instance->lcdDisplayLine2, " START  |", 9);
        }
        else
        {
            memcpy(instance->lcdDisplayLine2, "         ", 9);
        }
    }

    LcdDisplayController_WriteString(
            &instance->lcdDisplayController,
            instance->lcdDisplayLine1,
            16);
    Timer_OneShot_Init(
            &instance->moveDisplayCursorTimer,
            instance->timerModule,
            20,
            MoveCursorToSecondLcdLine,
            instance);
    Timer_OneShot_Start(&instance->moveDisplayCursorTimer);
}

static void ToggleWaveState(void *context, void *args)
{
    RECAST(instance, context, Application_t *);
    IGNORE(args);

    if(!instance->shouldOutputWave)
    {
        Timer_Periodic_Start(&instance->activeWorkoutLedTimer);
        instance->shouldOutputWave = true;
        WaveformGenerator_SetAmplitudePercentage(instance->waveformGenerator, instance->waveAmplitudePercentage);
        WaveformGenerator_Start(instance->waveformGenerator, sineWave, sineWaveLookupSize);
        WaveformGenerator_SetFrequencyInHz(instance->waveformGenerator, instance->waveFrequencyHz);
    }
    else
    {
        Timer_Periodic_Command(&instance->activeWorkoutLedTimer, Timer_Periodic_Command_Stop);
        GpioGroup_SetState(instance->gpioGroup, GpioStatusLed, GpioState_High);
        instance->shouldOutputWave = false;
        instance->calibrationDone = false;
        memcpy(instance->lcdDisplayLine1, "Calibrating...  ", 16);
        memcpy(instance->lcdDisplayLine2, "                ", 16);
        WaveformGenerator_Stop(instance->waveformGenerator, 0);
        StartCalibrationCycle(instance);
        instance->stepperOnePositionIndex = -1;
        instance->stepperTwoPositionIndex = -1;
        instance->waveFrequencyHz = 0;
        instance->waveAmplitudePercentage = 0.0;
        instance->workoutTime = 0;
    }
}

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
                if(instance->waveFrequencyHz <= 0 && instance->shouldOutputWave)
                {
                    WaveformGenerator_Start(
                            instance->waveformGenerator,
                            sineWave,
                            sineWaveLookupSize);
                }

                instance->waveFrequencyHz++;

                if(instance->shouldOutputWave)
                {
                    WaveformGenerator_SetFrequencyInHz(instance->waveformGenerator, instance->waveFrequencyHz);
                }

                instance->stepperOnePositionIndex++;
                StepperMotor_DoStep(
                    instance->stepperMotorOne,
                    StepDirection_Forward,
                    (uint16_t)stepperStepsPerSubdivision[instance->stepperOnePositionIndex]);
            }
        }
        else
        {
            if(!StepperMotor_IsBusy(instance->stepperMotorTwo) &&
               instance->stepperTwoPositionIndex < MaxStepIndexCount - 1)
            {
                if(instance->waveAmplitudePercentage < 1.0)
                {
                    instance->waveAmplitudePercentage += 0.05;
                }

                if(instance->shouldOutputWave)
                {
                    WaveformGenerator_SetAmplitudePercentage(
                            instance->waveformGenerator,
                            instance->waveAmplitudePercentage);
                }

                instance->stepperTwoPositionIndex++;
                StepperMotor_DoStep(
                    instance->stepperMotorTwo,
                    StepDirection_Forward,
                    (uint16_t)stepperStepsPerSubdivision[instance->stepperTwoPositionIndex]);
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
                instance->waveFrequencyHz--;

                if(instance->shouldOutputWave)
                {
                    if(instance->waveFrequencyHz == 0)
                    {
                        WaveformGenerator_Stop(instance->waveformGenerator, 0);
                    }
                    else
                    {
                        WaveformGenerator_SetFrequencyInHz(
                                instance->waveformGenerator,
                                instance->waveFrequencyHz);
                    }
                }

                StepperMotor_DoStep(
                    instance->stepperMotorOne,
                    StepDirection_Backward,
                    (uint16_t)stepperStepsPerSubdivision[instance->stepperOnePositionIndex]);
                instance->stepperOnePositionIndex--;
            }
        }
        else
        {
            if(!StepperMotor_IsBusy(instance->stepperMotorTwo) &&
               instance->stepperTwoPositionIndex >= 0)
            {
                if(instance->waveAmplitudePercentage > 0.05)
                {
                    instance->waveAmplitudePercentage -= 0.05;
                }

                if(instance->shouldOutputWave)
                {
                    WaveformGenerator_SetAmplitudePercentage(
                            instance->waveformGenerator,
                            instance->waveAmplitudePercentage);
                }

                StepperMotor_DoStep(
                    instance->stepperMotorTwo,
                    StepDirection_Backward,
                    (uint16_t)stepperStepsPerSubdivision[instance->stepperTwoPositionIndex]);
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
        if(instance->outputToChange == StepperOutputToChange_One)
        {
            memcpy(&instance->lcdDisplayLine2[9], "INCLINE", 7);
            instance->outputToChange = StepperOutputToChange_Two;
        }
        else
        {
            memcpy(&instance->lcdDisplayLine2[9], " SPEED ", 7);
            instance->outputToChange = StepperOutputToChange_One;
        }
    }
}

static void BeginNormalOperation(void *context, void *args)
{
    RECAST(instance, context, Application_t *);
    IGNORE(args);
    Event_Unsubscribe(
             StepperCalibrator_GetOnCalibrationDoneEvent(&instance->stepperCalibrator),
             &instance->calibrationDoneSub.interface);
    Event_Subscribe(Input_GetOnChangeEvent(&instance->timedInput.interface), &instance->timedInputSub.interface);
    Event_Subscribe(Input_GetOnChangeEvent(instance->buttonOne), &instance->buttonOneSubscriber.interface);
    Event_Subscribe(Input_GetOnChangeEvent(instance->buttonTwo), &instance->buttonTwoSubscriber.interface);
    Event_Subscribe(Input_GetOnChangeEvent(instance->buttonThree), &instance->buttonThreeSubscriber.interface);

    instance->calibrationDone = true;

    memcpy(instance->lcdDisplayLine1, "HOLD TO |SETTING", 16);
    if(instance->outputToChange == StepperOutputToChange_One)
    {
        memcpy(instance->lcdDisplayLine2, " START  | SPEED ", 16);
    }
    else
    {
        memcpy(instance->lcdDisplayLine2, " START  |INCLINE", 16);
    }

    LcdDisplayController_SetCursorIndex(&instance->lcdDisplayController, 1, 0);
}

static void CalibrateStepperTwo(void *context, void *args)
{
    RECAST(instance, context, Application_t *);
    IGNORE(args);

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

static void StartCalibrationCycle(Application_t *instance)
{
    Event_Unsubscribe(Input_GetOnChangeEvent(&instance->timedInput.interface), &instance->timedInputSub.interface);
    Event_Unsubscribe(Input_GetOnChangeEvent(instance->buttonOne), &instance->buttonOneSubscriber.interface);
    Event_Unsubscribe(Input_GetOnChangeEvent(instance->buttonTwo), &instance->buttonTwoSubscriber.interface);
    Event_Unsubscribe(Input_GetOnChangeEvent(instance->buttonThree), &instance->buttonThreeSubscriber.interface);

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

static void ToggleWorkoutLed(void *context)
{
    RECAST(instance, context, Application_t *);
    instance->currentLedState = (GpioState_t)!instance->currentLedState;
    GpioGroup_SetState(instance->gpioGroup, GpioStatusLed, instance->currentLedState);
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
    instance->waveFrequencyHz = 0;
    instance->waveAmplitudePercentage = 0.0;
    instance->shouldOutputWave = false;
    instance->workoutTime = 0;
    instance->updateTime = false;
    instance->calibrationDone = false;

    Input_TimedButtonPress_Init(
            &instance->timedInput,
            buttonThree,
            PeriodToCountTimedPressAfterMs,
            timerModule);

    DacController_Init(
            &instance->dacController,
            spi,
            gpioGroup,
            GpioSpiCs);

    instance->waveformGenerator = WaveformGenerator_Init(&instance->dacController);
    WaveformGenerator_SetAmplitudePercentage(instance->waveformGenerator, instance->waveAmplitudePercentage);

    EventSubscriber_Synchronous_Init(&instance->timedInputSub, ToggleWaveState, instance);
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

    Timer_Periodic_Init(
            &instance->activeWorkoutLedTimer,
            timerModule,
            PeriodToToggleActiveWorkoutLedMs,
            ToggleWorkoutLed,
            instance);

    StepperCalibrator_Init(&instance->stepperCalibrator, timerModule);

    Event_Subscribe(Input_GetOnChangeEvent(&instance->timedInput.interface), &instance->timedInputSub.interface);
    Event_Subscribe(Input_GetOnChangeEvent(instance->buttonOne), &instance->buttonOneSubscriber.interface);
    Event_Subscribe(Input_GetOnChangeEvent(instance->buttonTwo), &instance->buttonTwoSubscriber.interface);
    Event_Subscribe(Input_GetOnChangeEvent(instance->buttonThree), &instance->buttonThreeSubscriber.interface);

    memcpy(instance->lcdDisplayLine1, "Calibrating...  ", 16);
    memcpy(instance->lcdDisplayLine2, "                ", 16);

    GpioGroup_SetState(instance->gpioGroup, GpioStatusLed, GpioState_High);

    Timer_Periodic_Start(&instance->writeToLcdTimer);

    StartCalibrationCycle(instance);
}
