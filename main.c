#include <msp430.h> 
#include "Application.h"
#include "Adc_10.h"
#include "GpioGroup.h"
#include "I_GpioGroup.h"
#include "I_Interrupt.h"
#include "Input_Button.h"
#include "Input_StepperMotorHomeSensor.h"
#include "Interrupt_1MsSystemTick.h"
#include "LcdDisplayParallel2Line.h"
#include "I_TimeSource.h"
#include "Spi.h"
#include "TimeSource_Interrupt1MsSystemTick.h"
#include "StepperMotor.h"
#include "TimerModule.h"
#include "utils.h"

int main(void)
{
    DisableWatchdog();
    SetClockTo16Mhz();

    I_Interrupt_t *oneMsTick;
    oneMsTick = Interrupt_1MsSystemTick_Init();

    I_TimeSource_t *timeSource;
    timeSource = TimeSource_Interrupt1MsSystemTick_Init(oneMsTick);

    TimerModule_t *timerModule;
    timerModule = TimerModule_Init(timeSource);

    GpioGroup_t *gpioGroup;
    gpioGroup = GpioGroup_Init();

    LcdDisplayParallel2Line_t *lcdDisplay;
    lcdDisplay = LcdDisplayParallel2Line_Init(
            &gpioGroup->interface,
            GpioLcdDisplayD7,
            GpioLcdDisplayD6,
            GpioLcdDisplayD5,
            GpioLcdDisplayD4,
            GpioLcdDisplayEnable,
            GpioLcdDisplayRW,
            GpioLcdDisplayRS,
            GpioLcdDisplayBusy,
            timerModule);

    const StepperMotorConfig_t stepperOneConfig =
        {
             GpioStepperOnePwm1,
             GpioStepperOnePwm2,
             GpioStepperOnePwm3,
             GpioStepperOnePwm4,
        };

    StepperMotor_t stepperOne;
    StepperMotor_Init(&stepperOne, timerModule, &gpioGroup->interface, &stepperOneConfig);

    const StepperMotorConfig_t stepperTwoConfig =
        {
             GpioStepperTwoPwm1,
             GpioStepperTwoPwm2,
             GpioStepperTwoPwm3,
             GpioStepperTwoPwm4,
        };

    StepperMotor_t stepperTwo;
    StepperMotor_Init(&stepperTwo, timerModule, &gpioGroup->interface, &stepperTwoConfig);

    Input_Button_t buttonOne;
    Input_Button_Init(
            &buttonOne,
            &gpioGroup->interface,
            GpioTactileSwitchOne,
            timerModule,
            GpioState_High);

    Input_Button_t buttonTwo;
    Input_Button_Init(
            &buttonTwo,
            &gpioGroup->interface,
            GpioTactileSwitchTwo,
            timerModule,
            GpioState_High);

    Input_Button_t buttonThree;
    Input_Button_Init(
            &buttonThree,
            &gpioGroup->interface,
            GpioTactileSwitchThree,
            timerModule,
            GpioState_High);

    Input_StepperMotorHomeSensor_t stepperOneHomeSensor;
    Input_StepperMotorHomeSensor_Init(
            &stepperOneHomeSensor,
            timerModule,
            &gpioGroup->interface,
            GpioStepperOneSensor);

    Input_StepperMotorHomeSensor_t stepperTwoHomeSensor;
    Input_StepperMotorHomeSensor_Init(
            &stepperTwoHomeSensor,
            timerModule,
            &gpioGroup->interface,
            GpioStepperTwoSensor);

    Adc_10_t *adc;
    adc = Adc_10_Init();

    Spi_t *dacSpi;
    dacSpi = Spi_Init();

    Application_t application;
    Application_Init(
            &application,
            &gpioGroup->interface,
            &lcdDisplay->interface,
            &stepperOne.interface,
            &stepperTwo.interface,
            &buttonOne.interface,
            &buttonTwo.interface,
            &buttonThree.interface,
            &stepperOneHomeSensor.interface,
            &stepperTwoHomeSensor.interface,
            &adc->interface,
            &dacSpi->interface,
            timerModule);

    EnableInterrupts();

    while(1)
    {
        TimerModule_Run(timerModule);
        Application_Run(&application);
    }

    return 0;
}
