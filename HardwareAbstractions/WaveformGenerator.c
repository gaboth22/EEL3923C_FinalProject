#include <msp430.h>
#include "WaveformGenerator.h"
#include "utils.h"

#define TimerB0_Init() TBCTL = TBSSEL_2 | MC_1 | ID_3; TBCCR0 = CountForOneHz
#define TimerB0_EnableInterrupt() TBR = 0; TBCCTL0 = CCIE
#define TimerB0_DisableInterrupt() TBCCTL0 = 0x00

static WaveformGenerator_t instance;

enum
{
    CountForOneHz = 33500
};

void WaveformGenerator_SetFrequencyInHz(WaveformGenerator_t *_instance, uint16_t frequency)
{
    IGNORE(_instance);
    TBCCR0 = (CountForOneHz / frequency);
}

void WaveformGenerator_SetAmplitudePercentage(WaveformGenerator_t *_instance, float amplitudePercentage)
{
    IGNORE(_instance);
    instance.amplitudePercentage = amplitudePercentage;
}

void WaveformGenerator_Stop(WaveformGenerator_t *_instance, uint16_t outputDigitalVoltage)
{
    IGNORE(_instance);
    TimerB0_DisableInterrupt();
    DacController_SendInputCode(instance.dacController, outputDigitalVoltage);
}

void WaveformGenerator_Start(WaveformGenerator_t *_instance, const uint16_t *waveform, uint8_t waveformSize)
{
    IGNORE(_instance);
    instance.waveform = waveform;
    instance.waveformSize = waveformSize;
    TBCCR0 = CountForOneHz;
    TimerB0_EnableInterrupt();
}

WaveformGenerator_t * WaveformGenerator_Init(DacController_t *dacController)
{
    TimerB0_Init();
    TimerB0_DisableInterrupt();
    instance.dacController = dacController;
    instance.currentIndex = 0;
    instance.amplitudePercentage = 0.0;
    DacController_SendInputCode(dacController, 0);
    return &instance;
}

#pragma vector = TIMERB0_VECTOR
__interrupt void TB0_ISR(void)
{
    if(instance.currentIndex > (instance.waveformSize - 1))
    {
        instance.currentIndex = 0;
    }

    DacController_SendInputCode(
            instance.dacController,
            (uint16_t)(instance.waveform[instance.currentIndex++] * instance.amplitudePercentage));
}
