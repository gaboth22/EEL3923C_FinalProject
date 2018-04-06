#ifndef DACCONTROLLER_H
#define DACCONTROLLER_H

#include "I_Spi.h"
#include "I_GpioGroup.h"
#include "types.h"

typedef struct
{
    I_Spi_t *spiForDac;
    I_GpioGroup_t *gpioGroup;
    GpioChannel_t spiCsChannel;
} DacController_t;

/*
 * Send one datum to the SPI DAC
 */
void DacController_SendInputCode(DacController_t *instance, uint16_t inputCode);

/*
 * Initialize the controller
 */
void DacController_Init(
        DacController_t *instance,
        I_Spi_t *spiForDac,
        I_GpioGroup_t *gpioGroup,
        GpioChannel_t spiCsChannel);

#endif
