#ifndef __TIMER_H__
#define __TIMER_H__

#include "stdint.h"

extern uint32_t time_0;
extern uint32_t time_1;
extern uint8_t timer0_flag;
extern uint8_t timer1_flag;

extern void timer0_init(void);
extern void timer0_start(void);
extern void timer0_stop(void);
extern void timer0_reset(void);

extern void timer1_init(void);
extern void timer1_start(void);
extern void timer1_stop(void);
extern void timer1_reset(void);

#endif