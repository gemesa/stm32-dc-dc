
#ifndef INC_DC_DC_PRV_H_
#define INC_DC_DC_PRV_H_

#include "main.h"
#include <stdbool.h>

typedef struct xDUTY_CYCLE
{
    uint32_t ulDA;
    uint32_t ulDB;
} DutyCycle_t;

typedef struct xDC_DC_MODE
{
    uint32_t ulMode;
    bool xSwitch;
} DcDcMode_t;

typedef struct xPIOUT
{
    uint32_t ulD;
    bool xTopHit;
    bool xBottomHit;
} PiOut_t;

#define dcdcINT_TERM_LIMIT ((int32_t)(mainHRTIM_PERIOD/2))

#define dcdcMIN_D_A        ((uint32_t)1500u)
#define dcdcMAX_D_A        ((uint32_t)8000u)

#define dcdcMIN_D_B        ((uint32_t)500u)
#define dcdcMAX_D_B        ((uint32_t)8000u)

#define dcdcMIN_D_B_MIXED  ((uint32_t)500u)
#define dcdcMAX_D_B_MIXED  ((uint32_t)4500u)

#define dcdcMAX_CNT        ((uint32_t)1000u)

#define dcdcMODE_BUCK      ((uint32_t)0u)
#define dcdcMODE_BOOST     ((uint32_t)1u)
#define dcdcMODE_MIXED     ((uint32_t)2u)

extern void vSetDutyCycle(const uint32_t ulDA, const uint32_t ulDB);
extern DutyCycle_t xGetDutyCycle(void);
extern DcDcMode_t xPiEvalModeSwitch(const uint32_t ulModeOld, const uint32_t ulCntTop, const uint32_t ulCntBottom);
extern int32_t lPiLimitIntTerm(const int32_t lIntTermOld);
extern PiOut_t ulPiLimitPiOut(const uint32_t ulPiOutDOld, const uint32_t ulMode);

#endif /* INC_DC_DC_PRV_H_ */
