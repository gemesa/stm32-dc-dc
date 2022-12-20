
#include "timer.h"

volatile uint32_t ulHighFrequencyTimerTicks;

void configureTimerForRunTimeStats(void)
{
    ulHighFrequencyTimerTicks = 0u;
}

uint32_t getRunTimeCounterValue(void)
{
    return ulHighFrequencyTimerTicks;
}
