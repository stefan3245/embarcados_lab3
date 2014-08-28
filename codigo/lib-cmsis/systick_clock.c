/*****************************************************************************
 *
 * systick_clock.c
 *
 ******************************************************************************/

#include "mcu_regs.h"
#include "systick_clock.h"

static int msTicks = 0;

void SysTick_Handler(void)
{
    msTicks++;
}

int getTicks(void)
{
    return msTicks;
}
