#include <util/delay.h>
#include "key.h"

uint8_t keycode = KEY_NULL;

void key_init(void)
{
	KEY_DDR_C &= ~((1<<KEY_CANCEL_PORT)|(1<<KEY_RIGHT_PORT));
	KEY_PORT_C |= (1<<KEY_CANCEL_PORT)|(1<<KEY_RIGHT_PORT);
	//DDRC &= ~((1<<PC0)|(1<<PC1));
	//PORTC |= (1<<PC0)|(1<<PC1);
	
	KEY_DDR_D &= ~((1<<KEY_LEFT_PORT)|(1<<KEY_DOWN_PORT)
					|(1<<KEY_UP_PORT)|(1<<KEY_OK_PORT));
	KEY_PORT_D |= (1<<KEY_LEFT_PORT)|(1<<KEY_DOWN_PORT)
					|(1<<KEY_UP_PORT)|(1<<KEY_OK_PORT);	
}

uint8_t key_scan(void)
{
	if(((KEY_PIN_D & 0xF0) != 0xF0) || ((KEY_PIN_C & 0x03) != 0x03))
		_delay_ms(20);
		
	if(((KEY_PIN_D & 0xF0) != 0xF0) || ((KEY_PIN_C & 0x03) != 0x03))
	{
		if(!(KEY_PIN_D & (1<<KEY_UP_PORT)))
		{	
			keycode = KEY_UP;
			return keycode;
		}
		
		if(!(KEY_PIN_D & (1<<KEY_DOWN_PORT)))
		{	
			keycode = KEY_DOWN;
			return keycode;
		}
		
		if(!(KEY_PIN_D & (1<<KEY_LEFT_PORT)))
		{	
			keycode = KEY_LEFT;
			return keycode;
		}
		
		if(!(KEY_PIN_C & (1<<KEY_RIGHT_PORT)))
		{	
			keycode = KEY_RIGHT;
			return keycode;
		}
		
		if(!(KEY_PIN_D & (1<<KEY_OK_PORT)))
		{	
			keycode = KEY_OK;
			return keycode;
		}
		
		if(!(PINC & (1<<KEY_CANCEL_PORT)))
		{	
			keycode = KEY_CANCEL;
			return keycode;
		}
	}	
	
	keycode = KEY_NULL;
	return keycode;
}