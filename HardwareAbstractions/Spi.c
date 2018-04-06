#include <msp430.h>
#include "I_Spi.h"
#include "Spi.h"
#include "types.h"

static Spi_t instance;

static void SendByte(I_Spi_t *instance, uint8_t byte)
{
    while(IFG2 != (IFG2 | 0x08));
    UCB0TXBUF = byte;
}

static const SpiApi_t api =
    { SendByte };

Spi_t * Spi_Init(void)
{
    UCB0CTL1 |= 0x01; // Keep USCI in reset state
    UCB0CTL0 = 0b10101001;
    UCB0CTL1 |= 0b10000001; // ORing to not clear reset state
    UCB0BR1 = 0x02;
    UCB0BR0 = 0x00;
    P3SEL |= 0b00001010; // Primary peripheral use
    P3DIR |= BIT1; // SDA
    P3DIR |= BIT3; // SCK
    UCB0CTL1 &= ~UCSWRST; //Release USCI for operation by setting B0 = 0
    instance.interface.api = &api;

    return &instance;
}
