
#ifndef INC_DC_DC_H_
#define INC_DC_DC_H_

#include <stdint.h>

extern void vSetDutyCycleBuck(const uint32_t ulDA);
extern void vSetDutyCycleBoost(const uint32_t ulDB);
extern void vSetDutyCycleMixed(const uint32_t ulDA, const uint32_t ulDB);
extern void vDcDcControl(const uint32_t ulVout, const uint32_t ulVoutTarget);
extern void vSetAdcTriggerPoint(const uint32_t ulVal);

#endif /* INC_DC_DC_H_ */
