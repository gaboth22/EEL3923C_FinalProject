#ifndef I_INTERRUPT_H
#define I_INTERRUPT_H

#include "I_Event.h"

typedef struct
{
    I_Event_t *onInterrupt;
} I_Interrupt_t;

#endif
