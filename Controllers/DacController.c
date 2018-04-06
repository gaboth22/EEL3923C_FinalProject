#include <msp430.h>
#include "DacController.h"

void DacController_SendInputCode(DacController_t *instance, uint16_t inputCode)
{
    uint16_t word = 0xF000 | (inputCode << 2);
    GpioGroup_SetState(instance->gpioGroup, instance->spiCsChannel, GpioState_Low); // Enable active low CS
    uint8_t highByte = (uint8_t)((word >> 8) & 0x00FF);
    uint8_t lowByte = (uint8_t)(word & 0x00FF);
    Spi_SendByte(instance->spiForDac, highByte);
    Spi_SendByte(instance->spiForDac, lowByte);
    while(UCB0STAT & 0x01); // while sending out data
    GpioGroup_SetState(instance->gpioGroup, instance->spiCsChannel, GpioState_High); // Disable active low CS
}

void DacController_Init(
        DacController_t *instance,
        I_Spi_t *spiForDac,
        I_GpioGroup_t *gpioGroup,
        GpioChannel_t spiCsChannel)
{
    instance->spiForDac = spiForDac;
    instance->gpioGroup = gpioGroup;
    instance->spiCsChannel = spiCsChannel;
}
