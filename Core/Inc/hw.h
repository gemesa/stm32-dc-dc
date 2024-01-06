/**
******************************************************************************
* @file hw.h
* @brief HW header
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

#ifndef INC_HW_H_
#define INC_HW_H_

#include <stdint.h>

#define hwREAL_3V3          ((uint32_t)3305u)

/* theoretical bridge resistor Vin ratio: (6.8K/(6.8K + 27K))*10000 = 2012
 * the real ratio can be measured and calculated, refer to AN4449 */
#define hwREAL_VIN_R_RATIO  ((uint32_t)1970u)

/* theoretical bridge resistor Vout ratio (3.3K/(3.3K + 13.3K))*10000 = 1988
 * the real ratio can be measured and calculated, refer to AN4449 */
#define hwREAL_VOUT_R_RATIO ((uint32_t)1978u)

#define hwCCMRAM            __attribute__((section(".ccmram")))

#endif /* INC_HW_H_ */
