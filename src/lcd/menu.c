#include "menu.h"
#include "main.h"
#include <string.h>
#include <stdint.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "key.h"
#include "lcd1602.h"
#include "screen.h"
#include "gps.h"
#include "gpsraw.h"

struct m_item 
{
	uint8_t index;
	char* text;
	struct m_item* next;
	struct m_item* child;
};

char m_arrow = 0x7E;
struct m_item* mi_entry;
uint8_t menu_type = 0;
uint8_t arrow_Pos_H = 0;
uint8_t arrow_Pos_V = 0;
uint8_t mi_index = 0;
uint8_t mi_index_max = 0;

static void draw_menu(void);
static void move_arrow_left(void);
static void move_arrow_right(void);
static void move_arrow_up(void);
static void move_arrow_down(void);
static void menu_key_loop(void);

static uint8_t menu_READY_SCN(void);
static uint8_t menu_GPS_MAIN_SCN(void);

struct m_item mi_ready = {0,"GPS TRACKER",0,0};
struct m_item mi_start = {0,"Start",0,0};
struct m_item mi_settings = {1,"Settings",0,0};
struct m_item mi_menu = {0,"MENU",0,0};
struct m_item mi_save_pos = {0,"Save Pos",0,0};
struct m_item mi_finish = {1,"Finish",0,0};
struct m_item mi_cancel = {2,"Cancel",0,0};

void menu_init(void)
{
	mi_ready.child = &mi_start;
	mi_start.next = &mi_settings;
	mi_settings.next = &mi_start;
	
	mi_menu.child = &mi_save_pos;
	mi_save_pos.next = &mi_finish;
	mi_finish.next = &mi_cancel;
	mi_cancel.next = &mi_save_pos;
}

uint8_t menu_start(void)
{
	uint8_t m_result = 0;
	
	_delay_ms(200);
	switch(scn_mode)
	{
		case READY_SCN:
			menu_type = 1;
			mi_entry = &mi_ready;
			mi_index = 0;
			mi_index_max = 1;
			draw_menu();
			m_result = menu_READY_SCN();
			break;
		case GPS_UPDATE_SCN:
			break;
		case GPS_MAIN_SCN:
			menu_type = 0;
			mi_entry = &mi_menu;
			mi_index = 0;
			mi_index_max = 2;
			draw_menu();
			m_result = menu_GPS_MAIN_SCN();
			break;
		case GPS_POS_SCN:
			break;
		case GPS_TIME_SCN:
			break;
		case SETTING_SCN:
			break;
		default:
			break;
	}
	
	return m_result;
}

void draw_menu(void)
{
	struct m_item* mi_child;
	uint8_t index_temp = 0;
	uint8_t i = 0;

	Disp_Clear();
	
	if(menu_type)
	{
		XY_Str(2,0,(*mi_entry).text);
		mi_child = (*mi_entry).child;
		XY_Str(1,1,(*mi_child).text);
		mi_child = (*mi_child).next;
		XY_Str(8,1,(*mi_child).text);
		
		if(arrow_Pos_H == 0)
			XY_Char(0,1,m_arrow);
		else
			XY_Char(7,1,m_arrow);
	}
	else
	{
		Locate_xy(0,0);
		Disp_Str((*mi_entry).text);
		
		Locate_xy(4,0);
		Disp_Char('.');
		Disp_Char('|');
		
		if(arrow_Pos_V == 0)
		{
			index_temp = mi_index;
			Disp_Char(m_arrow);
		}
		else
		{		
			index_temp = mi_index - 1;
			XY_Char(6,1,m_arrow);
		}
		
		Locate_xy(5,1);
		Disp_Char('|');
		
		mi_child = (*mi_entry).child;
		for(i=0;i<2;i++)
		{
			while((*mi_child).index != index_temp + i)
				mi_child = (*mi_child).next;
				
			XY_Str(7,i,(*mi_child).text);
			if((*mi_child).child == 0)
				XY_Char(15,i,'|');
			else
				XY_Char(15,i,'>');
		}
	}
}

void move_arrow_left(void)
{
	if(mi_index > 0)
		mi_index--;
	
	if(arrow_Pos_H > 0)
		arrow_Pos_H--;

	draw_menu();
}

void move_arrow_right(void)
{
	if(mi_index < mi_index_max)
		mi_index++;
	
	if(arrow_Pos_H < 1)
		arrow_Pos_H++;

	draw_menu();
}

void move_arrow_up(void)
{
	if(mi_index > 0)
		mi_index--;
	
	if(arrow_Pos_V > 0)
		arrow_Pos_V--;
	
	draw_menu();
}

void move_arrow_down(void)
{
	if(mi_index < mi_index_max)
		mi_index++;
	
	if(arrow_Pos_V < 1)
		arrow_Pos_V++;
	
	draw_menu();
}

void menu_key_loop(void)
{
	while(1)
	{
		key_scan();
		
		_delay_ms(120);
		
		if(menu_type)
		{
			if(keycode == KEY_LEFT)
			{
				move_arrow_left();
				continue;
			}
			
			if(keycode == KEY_RIGHT)
			{
				move_arrow_right();
				continue;
			}
		}
		else
		{
			if(keycode == KEY_UP)
			{
				move_arrow_up();
				continue;
			}
			
			if(keycode == KEY_DOWN)
			{
				move_arrow_down();
				continue;
			}	
		}
		
		if(keycode == KEY_OK || keycode == KEY_CANCEL)
			return;
	}
}

uint8_t menu_READY_SCN(void)
{
	menu_key_loop();
	
	if(keycode == KEY_OK)
	{
		switch(mi_index)
		{
			case 0:
				sys_state = SYS_WORKING;
				break;
			case 1:
				sys_state = SYS_SETTING;
				break;
			default:
				break;
		}
		return 1;
	}
	else
	{
		sys_state = SYS_READY;
		return 1;
	}
	return 0;
}
  
uint8_t menu_GPS_MAIN_SCN(void)
{
	menu_key_loop();
	
	if(keycode == KEY_OK)
	{
		switch(mi_index)
		{
			case 0:
				save_position();
				break;
			case 1:
				finish_logfile();
				sys_state = SYS_READY;
				break;
			case 2:
				return 2;
				break;
			default:
				break;
		}
		return 1;
	}
	else
	{
		return 2;
	}
	return 0;
}