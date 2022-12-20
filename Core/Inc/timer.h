
#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include <stdint.h>

extern void configureTimerForRunTimeStats(void);
extern uint32_t getRunTimeCounterValue(void);

extern volatile uint32_t ulHighFrequencyTimerTicks;

#endif /* INC_TIMER_H_ */
