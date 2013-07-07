#include <avr/io.h>
#include "led.h"

uint8_t state;

void LED_init(void)
{
	state = 0x00;

	//DDRC |= (1<<PC2)|(1<<PC3)|(1<<PC4)|(1<<PC5);
	//PORTC |= (1<<PC2)|(1<<PC3)|(1<<PC4)|(1<<PC5);
	
	LED_PORT_B |= (1<<LED_0)|(1<<LED_1);
	LED_DDR_B |= (1<<LED_0)|(1<<LED_1);
	LED_PORT_C |= (1<<LED_2)|(1<<LED_3);
	LED_DDR_C |= (1<<LED_2)|(1<<LED_3);
}

void LED_on(uint8_t LED_n)
{
	switch(LED_n)
	{
		case LED_0:
			LED_PORT_B &= ~(1<<LED_0);
			break;
		case LED_1:
			LED_PORT_B &= ~(1<<LED_1);
			break;
		case LED_2:
			LED_PORT_C &= ~(1<<LED_2);
			break;
		case LED_3:
			LED_PORT_C &= ~(1<<LED_3);
			break;
		default:
			return;
			
	}
	state |= (1<<LED_n);
}

void LED_off(uint8_t LED_n)
{
	switch(LED_n)
	{
		case LED_0:
			LED_PORT_B |= 1<<LED_0;
			break;
		case LED_1:
			LED_PORT_B |= 1<<LED_1;
			break;
		case LED_2:
			LED_PORT_C |= 1<<LED_2;
			break;
		case LED_3:
			LED_PORT_C |= 1<<LED_3;
			break;
		default:
			return;
	}
	state &= ~(1<<LED_n);
}

void LED_switch(uint8_t LED_n)
{
	if(state & (1<<LED_n))
		LED_off(LED_n);
	else
		LED_on(LED_n);
}