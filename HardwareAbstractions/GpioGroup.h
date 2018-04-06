#ifndef GPIOGROUP_H
#define GPIOGROUP_H

#include "I_GpioGroup.h"
#include "GpioTable.h"

typedef struct
{
    I_GpioGroup_t interface;
} GpioGroup_t;

/*
 * Get the singleton instance for the GPIO group
 */
GpioGroup_t * GpioGroup_Init(void);

#endif
