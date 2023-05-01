/**
******************************************************************************
* @file adc.h
* @brief ADC header
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

#ifndef INC_ADC_H_
#define INC_ADC_H_

#include <stdint.h>

extern uint32_t ulVinRawToPhys(const uint32_t ulVinRaw);
extern uint32_t ulVoutRawToPhys(const uint32_t ulVoutRaw);

#endif /* INC_ADC_H_ */
