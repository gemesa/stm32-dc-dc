/**
******************************************************************************
* @file timer.h
* @brief timer header
* @author gemesa
*
* @attention
*
* Copyright (c) 2023 András Gémes. All rights reserved.
*
* This software component is licensed by András Gémes under MIT license.
* You may obtain a copy of the license at: opensource.org/licenses/mit
*
* SPDX-License-Identifier: MIT
******************************************************************************
*/

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include <stdint.h>

extern void configureTimerForRunTimeStats(void);
extern uint32_t getRunTimeCounterValue(void);

extern volatile uint32_t ulHighFrequencyTimerTicks;

#endif /* INC_TIMER_H_ */
