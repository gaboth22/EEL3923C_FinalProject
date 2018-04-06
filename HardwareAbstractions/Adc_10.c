#include <msp430.h>
#include "I_Adc.h"
#include "Adc_10.h"
#include "utils.h"

static Adc_10_t instance;

static AdcCounts_t GetAdcCounts(I_Adc_t *_instance)
{
    IGNORE(_instance);

    ADC10CTL0 |= 0x0003; // Enable conversion, and start it
    return ADC10MEM;
}

static const AdcApi_t adc10Api =
    { GetAdcCounts };

Adc_10_t * Adc_10_Init(void)
{
    P2SEL = 0x01; // Set P2.0 as second peripheral usage - A0
    P2DIR &= ~BIT0; // P2.0 as input, it's A0
    ADC10CTL0 = 0b0001100011110000; // VCC and GND as reference for ADC
    ADC10CTL1 = 0b0000000000011000; // enable A0
    ADC10AE0 = 0x01; // Enable A0 for input only
    ADC10DTC0 = 0x04; // Continuous readings

    instance.interface.api = &adc10Api;
    return &instance;
}
