#include "main.h"
#include <string.h>
#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "gps.h"
#include "uart.h"
#include "led.h"

/*
gps_message_flag:
	0 -- no
	1 -- GPS
	2 -- GPGGA
	3 -- GPRMC
*/

uint8_t gps_msg_ready;

static uint8_t gps_msg_flag;
static char char_buff;
static uint8_t char_count;
static uint8_t arg_count;
static uint8_t arg_char_count;

char gps_time[] = "tim000.000";
char gps_date[] = "dat0000";
char gps_state = 's';
char gps_lat[] = "lat0.0000";
char gps_NS = 'n';
char gps_lon[] = "lon00.0000";
char gps_EW = 'e';
char gps_alt[] = "alt00";
char gps_NoS[] = "n0";
char gps_spd[] = "spd00";
char gps_dir[] = "dir000";

void gps_init(void)
{
	char_buff = 0xFF;
	gps_msg_flag = 0;
	gps_msg_ready = 0;
	char_count = 0;
	arg_count = 0;
	arg_char_count = 0;	
}

void gps_set_arg(void)
{
	uart_puts_p(PSTR("$PSRF103,02,00,00,01*26"));
	uart_putc(0x0D);
	uart_putc(0x0A);
	uart_puts_p(PSTR("$PSRF103,03,00,00,01*27"));
	uart_putc(0x0D);
	uart_putc(0x0A);
	uart_puts_p(PSTR("$PSRF103,05,00,01,01*20"));
	uart_putc(0x0D);
	uart_putc(0x0A);
}

ISR(USART_RXC_vect)
{
	cli();
	
	char_buff = UDR;
	//uart_putc(char_buff);
	char_count++;

	if(gps_msg_flag)
	{
		if(char_count == 4)
		{
			switch(char_buff)
			{
				case 'G':
					gps_msg_flag = 2;
					break;
				case 'M':
					gps_msg_flag = 3;
					break;
				default:
					gps_msg_flag = 0;
					char_count = 0;
					arg_count = 0;
			}
		}
		else
		{
			if(char_buff == ',')
			{
				arg_count++;
				arg_char_count = 0;
			}
			else if((char_buff == 0x0A) || (char_buff == 0x0D))
			{
				char_count = 0;
				if(gps_msg_flag == 3)
				{
					gps_msg_ready = 1;
				}
				LED_off(LED_3);
				gps_msg_flag = 0;
				//print_gps_msg();
			}
			else
			{
				if(gps_msg_flag == 2)
				{
					switch(arg_count)
					{
						case 7:
							gps_NoS[arg_char_count] = char_buff;
							break;
						case 9:
							gps_alt[arg_char_count] = char_buff;
							break;
						default:
							break;
					}
					arg_char_count++;
				}
				else if(gps_msg_flag == 3)
				{
					switch(arg_count)
					{
						case 1:
							gps_time[arg_char_count] = char_buff;
							break;
						case 2:
							gps_state = char_buff;
							break;
						case 3:
							gps_lat[arg_char_count] = char_buff;
							break;
						case 4:
							gps_NS = char_buff;
							break;
						case 5:
							gps_lon[arg_char_count] = char_buff;
							break;
						case 6:
							gps_EW = char_buff;
							break;
						case 7:
							gps_spd[arg_char_count] = char_buff;
							break;
						case 8:
							gps_dir[arg_char_count] = char_buff;
							break;
						case 9:
							gps_date[arg_char_count] = char_buff;
							break;	
						default:
							break;
					}
					arg_char_count++;
				}
			}
		}
	}
	else
	{
		if(char_buff == '$')
		{
			gps_msg_flag = 1;
			arg_count = 0;
			char_count = 0;
			LED_on(LED_3);
		}
	}
	
	sei();
}



#if DEBUG
void print_gps_msg(void)
{
	uart_puts_p(PSTR("\n\r*******START********\n\r"));
	
	uart_puts_p(PSTR(" Time : "));
	uart_puts(gps_time);
	
	uart_puts_p(PSTR("\n\r Date : "));
	uart_puts(gps_date);
	
	uart_puts_p(PSTR("\n\r Lat : "));
	uart_puts(gps_lat);
	uart_putc(gps_NS);
	
	uart_puts_p(PSTR("\n\r Lon : "));
	uart_puts(gps_lon);
	uart_putc(gps_EW);
	
	uart_puts_p(PSTR("\n\r Alt : "));
	uart_puts(gps_alt);
	
	uart_puts_p(PSTR("\n\r State : "));
	uart_putc(gps_state);
	
	uart_puts_p(PSTR("\n\r NoS : "));
	uart_puts(gps_NoS);
	
	uart_puts_p(PSTR("\n\r Speed : "));
	uart_puts(gps_spd);
	
	uart_puts_p(PSTR("\n\r Speed Direction : "));
	uart_puts(gps_dir);
		
	uart_puts_p(PSTR("\n\r********END*********\n\r"));
	
}
#endif