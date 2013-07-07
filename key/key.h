#ifndef __KEY_H__
#define __KEY_H__

#include <avr/io.h>

/*
KEY PORT:
	PC0 -- KEY_OK_PORT
	PC1 -- KEY_RIGHT_PORT
	PD4 -- KEY_LEFT_PORT
	PD5 -- KEY_DOWN_PORT
	PD6 -- KEY_UP_PORT
	PD7 -- KEY_CANCEL_PORT
*/
#define KEY_PORT_C		PORTC
#define KEY_DDR_C		DDRC
#define KEY_PIN_C		PINC
#define KEY_PORT_D		PORTD
#define KEY_DDR_D		DDRD
#define KEY_PIN_D		PIND

#define KEY_CANCEL_PORT	PC0
#define KEY_RIGHT_PORT	PC1
#define KEY_LEFT_PORT	PD4
#define KEY_DOWN_PORT	PD5
#define KEY_UP_PORT		PD6
#define KEY_OK_PORT		PD7

#define KEY_NULL	0
#define KEY_UP		1
#define KEY_DOWN	2
#define KEY_LEFT	3
#define KEY_RIGHT	4
#define KEY_OK		5
#define KEY_CANCEL	6

extern uint8_t keycode;

extern void key_init(void);
extern uint8_t key_scan(void);

#endif