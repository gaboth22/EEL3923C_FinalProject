#ifndef GPIOTABLE_H
#define GPIOTABLE_H

typedef enum
{
    GpioMode_Input = 0,
    GpioMode_Output = 1
} GpioMode_t;

  //HOW(name, index in this table, port, bit, startingMode)
#define GPIO_TABLE(HOW) \
    HOW(GpioHeartbeatLed,       0,  4, 2, GpioMode_Output) \
    HOW(GpioLcdDisplayD7,       1,  4, 4, GpioMode_Output) \
    HOW(GpioLcdDisplayD6,       2,  4, 5, GpioMode_Output) \
    HOW(GpioLcdDisplayD5,       3,  4, 6, GpioMode_Output) \
    HOW(GpioLcdDisplayD4,       4,  4, 7, GpioMode_Output) \
    HOW(GpioLcdDisplayEnable,   5,  3, 4, GpioMode_Output) \
    HOW(GpioLcdDisplayRW,       6,  3, 5, GpioMode_Output) \
    HOW(GpioLcdDisplayRS,       7,  3, 6, GpioMode_Output) \
    HOW(GpioLcdDisplayBusy,     8,  3, 7, GpioMode_Input)  \
    HOW(GpioStepperOnePwm1,     9,  2, 3, GpioMode_Output) \
    HOW(GpioStepperOnePwm2,     10, 2, 4, GpioMode_Output) \
    HOW(GpioStepperOnePwm3,     11, 1, 0, GpioMode_Output) \
    HOW(GpioStepperOnePwm4,     12, 1, 1, GpioMode_Output) \
    HOW(GpioStepperTwoPwm1,     13, 1, 3, GpioMode_Output) \
    HOW(GpioStepperTwoPwm2,     14, 1, 2, GpioMode_Output) \
    HOW(GpioStepperTwoPwm3,     15, 4, 0, GpioMode_Output) \
    HOW(GpioStepperTwoPwm4,     16, 4, 1, GpioMode_Output) \
    HOW(GpioTactileSwitchOne,   17, 2, 1, GpioMode_Input)  \
    HOW(GpioTactileSwitchTwo,   18, 2, 2, GpioMode_Input)  \
    HOW(GpioTactileSwitchThree, 19, 3, 2, GpioMode_Input)  \
    HOW(GpioStatusLed,          20, 3, 0, GpioMode_Output) \
    HOW(GpioStepperOneSensor,   21, 2, 5, GpioMode_Input)  \
    HOW(GpioStepperTwoSensor,   22, 2, 7, GpioMode_Input)  \
    HOW(GpioSpiCs,              23, 2, 6, GpioMode_Output) \

#define EXPAND_AS_ENUM(_gpioName, _index, _port, _bit, _initialMode) \
    _gpioName = _index, \

enum
{
    GPIO_TABLE(EXPAND_AS_ENUM)
    GpioMax
};

#endif
