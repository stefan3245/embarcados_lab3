/*****************************************************************************
 *
 * systick_clock.h
 *
 ******************************************************************************/

#ifndef _SYSTICK_CLOCK_
#define _SYSTICK_CLOCK_

#include "mcu_regs.h"

void SysTick_Handler(void);

int getTicks(void);

#endif
