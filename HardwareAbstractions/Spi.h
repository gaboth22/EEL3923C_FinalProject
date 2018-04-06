#ifndef SPI_H
#define SPI_H

#include "I_Spi.h"

typedef struct
{
    I_Spi_t interface;
} Spi_t;

/*
 * Initialize the SPI peripheral and get singleton instance to it
 */
Spi_t * Spi_Init(void);

#endif
