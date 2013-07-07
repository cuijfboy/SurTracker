#include "main.h"
#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "uart.h"
#include "key.h"
#include "led.h"
#include "lcd1602.h"
#include "screen.h"
#include "menu.h"
#include "timer.h"
#include "sdreader.h"
#include "gps.h"
#include "gpsraw.h"

uint8_t sys_state; 
uint8_t sd_avlb;
uint8_t gpx_avlb;
uint8_t speeder_avlb;
uint8_t gps_avlb;

static uint8_t init(void);
static void get_ready(void);
static void task(void);
static void LCD_task(void);
static void SD_task(void);
static void Key_task(void);

int main(void)
{
	uint8_t result;
	
	///////////////////////////////////////
	sys_state = SYS_INIT;
	sd_avlb = 1;
	speeder_avlb = 1;
	gps_avlb = 1;
	///////////////////////////////////////
	
/////////////////////////////////////////////////////////////
	result = init();
	if(result)
	{
#if DEBUG
		uart_puts_p(PSTR("\nINIT Failed! \n\n"));
#endif		
		Disp_Clear();
		XY_Str_P(0,0,PSTR("NO SD Card! -"));
		XY_Num(13,0,result);
		XY_Char(11,1,m_arrow);
		XY_Str_P(12,1,PSTR("SKIP"));
		//XY_Str_P(12,1,m_item[4]);
		sd_avlb = 0;
		timer1_start();
		
		while(!(key_scan() == KEY_OK))
			_delay_ms(100);
		sei();
	}
#if DEBUG
	else
		uart_puts_p(PSTR("\nINIT Completed! \n\n"));
#endif
	sys_state = SYS_READY;
/////////////////////////////////////////////////////////////


	while(1)
	{
	/////////////////////////////////////////////////////////////	
#if DEBUG	
		uart_puts_p(PSTR("get_ready() start! \n"));
#endif
		while(sys_state == SYS_READY)
			get_ready();
	/////////////////////////////////////////////////////////////

				
	/////////////////////////////////////////////////////////////	
#if DEBUG	
		uart_puts_p(PSTR("task() start! \n"));
#endif	
		while(sys_state == SYS_WORKING)
			task();
	/////////////////////////////////////////////////////////////
	}
	return 0;
}

uint8_t init(void)
{
	uint8_t result;
	
	uart_init();
#if DEBUG
	uart_puts_p(PSTR("INIT Started! \n\n"));
	uart_puts_p(PSTR("uart_init() Completed! \n"));
#endif	
		
	LED_init();
#if DEBUG	
	uart_puts_p(PSTR("LED_init() Completed! \n"));
#endif

	key_init();
#if DEBUG	
	uart_puts_p(PSTR("key_init() Completed! \n"));
#endif
		
	LCD_Init();
#if DEBUG	
	uart_puts_p(PSTR("LCD_Init() Completed! \n"));
#endif	
	set_screen(BOOT_SCN);
	_delay_ms(100);

	set_screen(BOOT_SCN);
	_delay_ms(100);
	set_screen(BOOT_SCN);
	_delay_ms(100);
	set_screen(BOOT_SCN);
	_delay_ms(100);

	gps_init();
#if DEBUG	
	uart_puts_p(PSTR("gps_init() Completed! \n"));
#endif	
	set_screen(BOOT_SCN);
	_delay_ms(100);
	
	timer0_init();
#if DEBUG	
	uart_puts_p(PSTR("timer0_Init() Completed! \n"));
#endif	
	set_screen(BOOT_SCN);
	_delay_ms(100);

	timer1_init();
#if DEBUG	
	uart_puts_p(PSTR("timer1_Init() Completed! \n"));
#endif
	set_screen(BOOT_SCN);
	_delay_ms(100);
	
	result = sdreader_init();
	if(result)
	{
#if DEBUG		
		uart_puts_p(PSTR("ERROR : sdreader_init()"));
		uart_putc_hex(result);
		uart_puts_p(PSTR("\n\r"));
#endif	
		return result;
	}
#if DEBUG	
	else
		uart_puts_p(PSTR("sdreader_init() copmleted! \n\r"));
#endif
	set_screen(BOOT_SCN);
	_delay_ms(100);
	
	sei();
	set_screen(BOOT_SCN);
	_delay_ms(500);
	
	return 0;
}

void get_ready(void)
{
	menu_init();
#if DEBUG	
	uart_puts_p(PSTR("menu_init() Completed! \n"));
#endif

	LED_on(LED_0);
	LED_on(LED_1);
	LED_on(LED_2);
	LED_on(LED_3);
	
	set_screen(SPLASH_SCN);
	_delay_ms(2000);
	
	gps_set_arg();
	
	LED_off(LED_0);
	LED_off(LED_1);
	LED_off(LED_2);
	LED_off(LED_3);
	
	set_screen(READY_SCN);
	menu_start();	
}

void task(void)
{
	//timer1_start();
		
	LCD_task();
	SD_task();
	Key_task();
	
	if(gps_msg_ready > 1)
		gps_msg_ready = 0;
		
	if(timer1_flag > 1)
		timer1_flag = 0;
		
	_delay_ms(100);	
	
}

void LCD_task(void)
{
	if(gps_avlb)
	{
		if(gps_msg_ready)
		{
			gps_msg_ready++;
#if DEBUG				
			print_gps_msg();
#endif
			//if(1)
			if(gps_state == 'A')
			{
				if(timer1_flag)
				{
					timer1_flag++;
					switch_scn_flag(0);
				}
					
				set_screen(GPS_MAIN_SCN);
			}
			else
			{
				LED_on(LED_3);
				set_screen(GPS_UPDATE_SCN);
			}
		}
	}
}

void SD_task(void)
{
	if(gps_avlb)
	{
		if(gps_msg_ready)
		{
			gps_msg_ready++;
			
			if(sd_avlb)
			{
				if(timer1_flag && gpsraw_flag == 2)
				{
					timer1_flag++;
					write_gps_msg();
				}
				else
				{
					//if(gpsraw_flag == 0)
					if(gpsraw_flag == 0 && gps_state == 'A')
					{
						new_logfile();
						write_gps_head();
						write_gps_msg();
						timer1_start();
					}
				}
			}
		}
	}
}

void Key_task(void)
{
	key_scan();
	
	if(keycode != KEY_NULL)
	{
		if(scn_mode == GPS_MAIN_SCN)
		{
			switch(keycode)
			{
				case KEY_UP:
				case KEY_DOWN:
					switch_scn_flag(0);
					set_screen(GPS_MAIN_SCN);
					break;
				case KEY_LEFT:
					save_position();
					break;	
				case KEY_OK:
				case KEY_CANCEL:
					menu_start();
					break;
				default:
					break;
			}
		}
	}
}
