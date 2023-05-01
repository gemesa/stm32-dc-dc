/**
******************************************************************************
* @file dc_dc.c
* @brief DC-DC source
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

#include "main.h"
#include "hw.h"
#include "dc_dc.h"
#include "dc_dc_prv.h"
#include <stdbool.h>
#include "pd.h"

extern HRTIM_HandleTypeDef hhrtim1;

hwCCMRAM
void vSetDutyCycleBuck(const uint32_t ulDA)
{
    vSetDutyCycle(ulDA, 0u);
}

hwCCMRAM
void vSetDutyCycleBoost(const uint32_t ulDB)
{
    vSetDutyCycle(10000u, ulDB);
}

hwCCMRAM
void vSetDutyCycleMixed(const uint32_t ulDA, const uint32_t ulDB)
{
    vSetDutyCycle(ulDA, ulDB);
}

hwCCMRAM
void vDcDcControl(const uint32_t ulVout, const uint32_t ulVoutTarget)
{
    const int32_t lKp = 2000;
    const int32_t lKi = 2000;
    const int32_t lPiScale = 10000;
    const int32_t lPiOutOffs = 5000;

    static int32_t lIntTerm;
    static uint32_t ulCntTopHit;
    static uint32_t ulCntBottomHit;
    static DcDcMode_t xDcDcMode = { .ulMode = dcdcMODE_BUCK };

    int32_t lError;
    PiOut_t xPiOut;

    lError = (int32_t) ulVoutTarget - (int32_t) ulVout;

    lIntTerm += lKi * lError / lPiScale;

    lIntTerm = lPiLimitIntTerm(lIntTerm);

    xPiOut.ulD = (uint32_t) (lKp * lError / lPiScale + lIntTerm + lPiOutOffs);

    xPiOut = ulPiLimitPiOut(xPiOut.ulD, xDcDcMode.ulMode);

    if (true == xPiOut.xTopHit)
    {
        ulCntTopHit++;
    }
    else
    {
        if (0u != ulCntTopHit)
        {
            ulCntTopHit--;
        }
    }

    if (true == xPiOut.xBottomHit)
    {
        ulCntBottomHit++;
    }
    else
    {
        if (0u != ulCntBottomHit)
        {
            ulCntBottomHit--;
        }
    }

    xDcDcMode = xPiEvalModeSwitch(xDcDcMode.ulMode, ulCntTopHit, ulCntBottomHit);

    if (true == xDcDcMode.xSwitch)
    {
        lIntTerm = 0;
        ulCntTopHit = 0u;
        ulCntBottomHit = 0u;

        if (dcdcMODE_BUCK == xDcDcMode.ulMode)
        {
            vSetDutyCycleBuck(5000u);
            vSetAdcTriggerPoint(2500u);
        }
        else if (dcdcMODE_MIXED == xDcDcMode.ulMode)
        {
            vSetDutyCycleMixed(8000u, 2000u);
            vSetAdcTriggerPoint(6000u);
        }
        else /* dcdcMODE_BOOST */
        {
            vSetDutyCycleBoost(5000u);
            vSetAdcTriggerPoint(2500u);
        }
    }
    else
    {
        if (dcdcMODE_BUCK == xDcDcMode.ulMode)
        {
            vSetDutyCycleBuck(xPiOut.ulD);

            if (5000u < xPiOut.ulD)
            {
                vSetAdcTriggerPoint(xPiOut.ulD / 2u);
            }
            else
            {
                vSetAdcTriggerPoint((10000u - xPiOut.ulD) / 2u + xPiOut.ulD);
            }
        }
        else if (dcdcMODE_MIXED == xDcDcMode.ulMode)
        {
            vSetDutyCycleMixed(8000u, xPiOut.ulD);
            vSetAdcTriggerPoint(6000u);
        }
        else /* dcdcMODE_BOOST */
        {
            vSetDutyCycleBoost(xPiOut.ulD);

            if (5000u < xPiOut.ulD)
            {
                vSetAdcTriggerPoint(xPiOut.ulD / 2u);
            }
            else
            {
                vSetAdcTriggerPoint((10000u - xPiOut.ulD) / 2u + xPiOut.ulD);
            }
        }
    }
}

hwCCMRAM
void vSetAdcTriggerPoint(const uint32_t ulVal)
{
    uint32_t ulCmp;

    ulCmp = ulVal * (uint32_t) mainHRTIM_PERIOD / pdSCALE_10K;
    __HAL_HRTIM_SETCOMPARE(&hhrtim1, HRTIM_TIMERINDEX_TIMER_A, HRTIM_COMPAREUNIT_4, ulCmp);
}
