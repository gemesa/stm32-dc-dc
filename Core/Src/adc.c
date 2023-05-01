/**
******************************************************************************
* @file adc.c
* @brief ADC source
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

#include <stdint.h>
#include "hw.h"
#include "pd.h"

#define adcMAX_ADC_VAL ((uint32_t)4095u)

uint32_t ulVinRawToPhys(const uint32_t ulVinRaw)
{
    uint32_t ulVinPhys;

    ulVinPhys = (((ulVinRaw * hwREAL_3V3) / adcMAX_ADC_VAL) * pdSCALE_10K) / hwREAL_VIN_R_RATIO;
    return ulVinPhys;
}

uint32_t ulVoutRawToPhys(const uint32_t ulVoutRaw)
{
    uint32_t ulVoutPhys;

    ulVoutPhys = (((ulVoutRaw * hwREAL_3V3) / adcMAX_ADC_VAL) * pdSCALE_10K) / hwREAL_VOUT_R_RATIO;
    return ulVoutPhys;
}
