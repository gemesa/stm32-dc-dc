/**
******************************************************************************
* @file dc_dc_prv.c
* @brief DC-DC private source
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

/*
 * buck mode:
 * TA1 = PWMA
 * TA2 = /PWMA
 * TB1 = low
 * TB2 = high
 * --> D1 (TA) is changing, D2 (TB) is constant 0%
 *
 * boost mode:
 * TA1 = high
 * TA2 = low
 * TB1 = PWMB
 * TB2 = /PWMB
 * --> D1 (TA) is constant 100%, D2 (TB) is changing
 *
 * mixed mode:
 * TA1 = PWMA
 * TA2 = /PWMA
 * TB1 = PWMB
 * TB2 = /PWMB
 * --> both D1 (TA) and D2 (TB) are changing
 */

#include "main.h"
#include "hw.h"
#include "dc_dc_prv.h"
#include <stdbool.h>
#include "pd.h"

extern HRTIM_HandleTypeDef hhrtim1;

/*
 * set D1 (TA - buck) and D2 (TB - boost) duty cycles
 *
 * examples:
 *
 * buck mode:
 * D1 = 70.67%, D2 = 0.00% --> vSetDutyCycle(7067u, 0u);
 *
 * boost mode:
 * D1 = 100.0%, D2 = 50.81% --> vSetDutyCycle(10000u, 5081u);
 *
 * mixed mode:
 * D1 = 80.00%, D2 = 20.00% --> vSetDutyCycle(8000u, 2000u);
 */
hwCCMRAM
void vSetDutyCycle(const uint32_t ulDA, const uint32_t ulDB)
{
    uint32_t ulCmpA;
    uint32_t ulCmpB;

    /* buck mode:
     * CMP2 = PER, the duty cycle is 0% (CMP2 reset and PER set happens at the same time but CMP2 reset wins over PER set as it has higher priority) */
    if (0u == ulDB)
    {
        ulCmpB = (uint32_t) mainHRTIM_PERIOD;
    }
    /* boost or mixed mode */
    else
    {
        ulCmpB = ulDB * (uint32_t) mainHRTIM_PERIOD / pdSCALE_10K;
    }

    /* boost mode:
     * CMP1 > PER, the duty cycle is 100% (no CMP1 reset event is generated) */
    if (10000u == ulDA)
    {
        ulCmpA = (uint32_t) mainHRTIM_PERIOD + 1u;
    }
    /* buck or mixed mode */
    else
    {
        ulCmpA = ulDA * (uint32_t) mainHRTIM_PERIOD / pdSCALE_10K;
    }

    __HAL_HRTIM_SETCOMPARE(&hhrtim1, HRTIM_TIMERINDEX_TIMER_A, HRTIM_COMPAREUNIT_1, ulCmpA);
    __HAL_HRTIM_SETCOMPARE(&hhrtim1, HRTIM_TIMERINDEX_TIMER_B, HRTIM_COMPAREUNIT_2, ulCmpB);
}

/*
 * get D1 (TA - buck) and D2 (TB - boost) duty cycles
 */
hwCCMRAM
DutyCycle_t xGetDutyCycle(void)
{
    uint32_t ulCmpA;
    uint32_t ulCmpB;
    DutyCycle_t xDutyCycle;

    ulCmpA = __HAL_HRTIM_GETCOMPARE(&hhrtim1, HRTIM_TIMERINDEX_TIMER_A, HRTIM_COMPAREUNIT_1);
    ulCmpB = __HAL_HRTIM_GETCOMPARE(&hhrtim1, HRTIM_TIMERINDEX_TIMER_B, HRTIM_COMPAREUNIT_2);

    /* buck mode:
     * CMP2 = PER, the duty cycle is 0% (CMP2 reset and PER set happens at the same time but CMP2 reset wins over PER set as it has higher priority) */
    if ((uint32_t) mainHRTIM_PERIOD == ulCmpB)
    {
        xDutyCycle.ulDB = 0u;
    }
    /* boost or mixed mode */
    else
    {
        xDutyCycle.ulDB = ulCmpB * pdSCALE_10K / (uint32_t) mainHRTIM_PERIOD;
    }

    /* boost mode:
     * CMP1 > PER, the duty cycle is 100% (no CMP1 reset event is generated) */
    if (((uint32_t) mainHRTIM_PERIOD + 1u) == ulCmpA)
    {
        xDutyCycle.ulDA = 10000u;
    }
    /* buck or mixed mode */
    else
    {
        xDutyCycle.ulDA = ulCmpA * pdSCALE_10K / (uint32_t) mainHRTIM_PERIOD;
    }

    return xDutyCycle;
}

hwCCMRAM
DcDcMode_t xPiEvalModeSwitch(const uint32_t ulModeOld, const uint32_t ulCntTop, const uint32_t ulCntBottom)
{
    DcDcMode_t xDcDcMode = { .ulMode = ulModeOld, .xSwitch = false };

    if (dcdcMODE_BUCK == ulModeOld)
    {
        if (dcdcMAX_CNT == ulCntTop)
        {
            xDcDcMode.ulMode = dcdcMODE_MIXED;
            xDcDcMode.xSwitch = true;
        }
    }
    else if (dcdcMODE_MIXED == ulModeOld)
    {
        if (dcdcMAX_CNT == ulCntTop)
        {
            xDcDcMode.ulMode = dcdcMODE_BOOST;
            xDcDcMode.xSwitch = true;
        }
        else if (dcdcMAX_CNT == ulCntBottom)
        {
            xDcDcMode.ulMode = dcdcMODE_BUCK;
            xDcDcMode.xSwitch = true;
        }
    }
    else /* dcdcMODE_BOOST */
    {
        if (dcdcMAX_CNT == ulCntBottom)
        {
            xDcDcMode.ulMode = dcdcMODE_MIXED;
            xDcDcMode.xSwitch = true;
        }
    }

    return xDcDcMode;
}

hwCCMRAM
int32_t lPiLimitIntTerm(const int32_t lIntTermOld)
{
    int32_t lIntTerm = lIntTermOld;

    if (dcdcINT_TERM_LIMIT < lIntTerm)
    {
        lIntTerm = dcdcINT_TERM_LIMIT;
    }
    else if (-dcdcINT_TERM_LIMIT > lIntTerm)
    {
        lIntTerm = -dcdcINT_TERM_LIMIT;
    }

    return lIntTerm;
}

hwCCMRAM
PiOut_t ulPiLimitPiOut(const uint32_t ulPiOutDOld, const uint32_t ulMode)
{
    PiOut_t xPiOut = { .ulD = ulPiOutDOld, .xTopHit = false, .xBottomHit = false };

    if (dcdcMODE_BUCK == ulMode)
    {
        if (dcdcMAX_D_A <= ulPiOutDOld)
        {
            xPiOut.ulD = dcdcMAX_D_A;
            xPiOut.xTopHit = true;
        }
        else if (dcdcMIN_D_A >= ulPiOutDOld)
        {
            xPiOut.ulD = dcdcMIN_D_A;
            xPiOut.xBottomHit = true;
        }
    }
    else if (dcdcMODE_MIXED == ulMode)
    {
        if (dcdcMAX_D_B_MIXED <= ulPiOutDOld)
        {
            xPiOut.ulD = dcdcMAX_D_B_MIXED;
            xPiOut.xTopHit = true;
        }
        else if (dcdcMIN_D_B_MIXED >= ulPiOutDOld)
        {
            xPiOut.ulD = dcdcMIN_D_B_MIXED;
            xPiOut.xBottomHit = true;
        }
    }
    else /* dcdcMODE_BOOST */
    {
        if (dcdcMAX_D_B <= ulPiOutDOld)
        {
            xPiOut.ulD = dcdcMAX_D_B;
            xPiOut.xTopHit = true;
        }
        else if (dcdcMIN_D_B >= ulPiOutDOld)
        {
            xPiOut.ulD = dcdcMIN_D_B;
            xPiOut.xBottomHit = true;
        }
    }

    return xPiOut;
}
