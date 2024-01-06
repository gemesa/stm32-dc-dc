/**
******************************************************************************
* @file timer.c
* @brief timer source
* @author gemesa
*
* @attention
*
* Copyright (c) 2024 András Gémes. All rights reserved.
*
* This software component is licensed by András Gémes under MIT license.
* You may obtain a copy of the license at: opensource.org/licenses/mit
*
* SPDX-License-Identifier: MIT
******************************************************************************
*/

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
