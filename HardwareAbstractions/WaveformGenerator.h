#ifndef WAVEFORMGENERATOR_H
#define WAVEFORMGENERATOR_H

#include "types.h"
#include "DacController.h"

typedef struct
{
    const uint16_t *waveform;
    uint8_t waveformSize;
    uint8_t currentIndex;
    float amplitudePercentage;
    DacController_t *dacController;
} WaveformGenerator_t;

/*
 * Set frequency to output wave at
 */
void WaveformGenerator_SetFrequencyInHz(WaveformGenerator_t *instance, uint16_t frequency);

/*
 * Set divider to divide output data by
 */
void WaveformGenerator_SetAmplitudePercentage(WaveformGenerator_t *instance, float amplitudePercentage);

/*
 * Stop the waveform output
 */
void WaveformGenerator_Stop(WaveformGenerator_t *instance, uint16_t outputDigitalVoltage);

/*
 * Start the waveform generator with the given wave
 */
void WaveformGenerator_Start(WaveformGenerator_t *instance, const uint16_t *waveform, uint8_t waveformSize);

/*
 * Get singleton instance of waveform generator
 */
WaveformGenerator_t * WaveformGenerator_Init(DacController_t *dacController);

#endif
