#ifndef __LED_H__
#define __LED_H__

#include <avr/io.h>
#include <stdint.h>

/*
LED PORT:
	PC2 -- LED_0
	PC3 -- LED_1
	PC4 -- LED_2
	PC5 -- LED_3
	PC6 -- LED_4*
	PC7 -- LED_5*
*/
#define LED_PORT_B	PORTB
#define LED_DDR_B	DDRB

#define LED_PORT_C	PORTC
#define LED_DDR_C	DDRC

#define LED_0 	PB0
#define LED_1 	PB1
#define LED_2 	PC6
#define LED_3 	PC7

//#define LED_0_ON 	LED_PORT_B &= ~(1<<LED_0)
//#define LED_0_OFF 	LED_PORT_B |= 1<<LED_0
//
//#define LED_1_ON 	LED_PORT_B &= ~(1<<LED_1)
//#define LED_1_OFF 	LED_PORT_B |= 1<<LED_1
//
//#define LED_2_ON 	LED_PORT_C &= ~(1<<LED_2)
//#define LED_2_OFF 	LED_PORT_C |= 1<<LED_2
//
//#define LED_3_ON 	LED_PORT_C &= ~(1<<LED_3)
//#define LED_3_OFF 	LED_PORT_C |= 1<<LED_3

extern void LED_init(void);
extern void LED_on(uint8_t LED_n);
extern void LED_off(uint8_t LED_n);
extern void LED_switch(uint8_t LED_n);

#endif