#ifndef ADC_10_H
#define ADC_10_H

#include "I_Adc.h"

typedef struct
{
    I_Adc_t interface;
} Adc_10_t;

/*
 * Get singleton handle to Adc 10
 */
Adc_10_t * Adc_10_Init(void);

#endif
