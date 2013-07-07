#include "screen.h"
#include <stdint.h>
#include <avr/pgmspace.h>
#include "lcd1602.h"
#include "menu.h"
#include "gps.h"

uint8_t scn_mode = 0;
static uint8_t last_mode = 0;

static uint8_t flag = 0;

static uint8_t timer_update_0 = 0;
static uint8_t timer_update_1 = 0;
static uint8_t timer_update_2 = 0;

static char utc_0_org = 0;
static char utc_1_org = 0;
static uint8_t utc_val = 0;
static uint8_t utc_0_val = 0;
static uint8_t utc_1_val = 0;

static void disp_BOOT_SCN(void);
static void disp_SPLASH_SCN(void);
static void disp_GPS_UPDATE_SCN(void);
static void disp_GPS_MAIN_SCN(void);
static void disp_GPS_POS_SCN(void);

void set_screen(uint8_t mode)
{
	last_mode = scn_mode;
	scn_mode = mode;
	
	if(last_mode != scn_mode)
	{
		Disp_Clear();
		flag = 0;
	}
	
	switch(scn_mode)
	{
		case BOOT_SCN:
			disp_BOOT_SCN();
			break;
		case SPLASH_SCN:
			disp_SPLASH_SCN();
			break;
		case GPS_UPDATE_SCN:
			disp_GPS_UPDATE_SCN();
			break;
		case GPS_MAIN_SCN:
			disp_GPS_MAIN_SCN();
			break;
		case GPS_POS_SCN:
			disp_GPS_POS_SCN();
			break;
		case GPS_TIME_SCN:
			break;
		case SETTING_SCN:
			break;
		default:
			break;
	}
}

void switch_scn_flag(uint8_t fg)
{
	if(scn_mode == GPS_MAIN_SCN)
	{
		if(fg < 3)
		{
			flag++;
			if(flag >= 3)
				flag = 0;
		}
		else
		{
			flag = fg;
		}
	}
}

void disp_BOOT_SCN(void)
{
	if(flag && flag<=8)
	{
		Disp_Char('>');
		Disp_Char('>');
		flag++;
	}
	else
	{
		XY_Str_P(0,0,PSTR("starting..."));
		XY_Str_P(0,1,PSTR("================"));
		flag++;
		Locate_xy(0,1);
	}
}

void disp_SPLASH_SCN(void)
{
	XY_Str_P(4,0,PSTR("WELCOME!"));
	XY_Str_P(2,1,PSTR("GPS TRACKER"));
}

void disp_GPS_UPDATE_SCN(void)
{
	XY_Str_P(0,0,PSTR("GPS UPDATE..."));

	//Locate_xy(13,0);
	
	timer_update_0++;
	if(timer_update_0 > 9)
	{
		timer_update_0 -= 10;
		timer_update_1++;
		if(timer_update_1 > 9)
		{
			timer_update_1 -= 10;
			timer_update_2++;
			if(timer_update_2 > 9)
			{
				timer_update_0 = 9;
				timer_update_1 = 9;
				timer_update_2 = 9;
			}
		}
	}
	Disp_Num(timer_update_2);
	Disp_Num(timer_update_1);
	Disp_Num(timer_update_0);
	
	Locate_xy(0,1);
	Disp_Char(gps_date[4]);
	Disp_Char(gps_date[5]);
	Disp_Char(gps_date[2]);
	Disp_Char(gps_date[3]);
	Disp_Char(gps_date[0]);
	Disp_Char(gps_date[1]);
	
	Disp_Str(gps_time);
}

void disp_GPS_MAIN_SCN(void)
{
	Locate_xy(0,0);
	
	if(gps_time[1] != utc_1_org || gps_time[0] != utc_0_org)
	{
		utc_0_org = gps_time[0];
		utc_1_org = gps_time[1];
		
		utc_val = ( utc_0_org - 0x30 ) * 10 + ( utc_1_org - 0x30 ) + 8;
		if(utc_val >= 24)
			utc_val -= 24;
			
		utc_0_val = utc_val / 10;
		utc_1_val = utc_val - utc_0_val * 10;
	}
	
	Disp_Num(utc_0_val);
	Disp_Num(utc_1_val);
	Disp_Char(':');
	Disp_Char(gps_time[2]);
	Disp_Char(gps_time[3]);
	Disp_Char(' ');
	
	if(gps_dir[2] == '.')
	{
		Disp_Char(' ');
		Disp_Char(gps_dir[0]);
		Disp_Char(gps_dir[1]);
	}
	else
	{
		Disp_Char(gps_dir[0]);
		Disp_Char(gps_dir[1]);
		Disp_Char(gps_dir[2]);
	}
	Disp_Char(0xDF);
	
	Disp_Char(gps_spd[0]);
	Disp_Char(gps_spd[1]);
	Disp_Char(gps_spd[2]);
	Disp_Char(gps_spd[3]);
	Disp_Char('K');
	Disp_Char('n');
	
	XY_Str(0,1,gps_NoS);
	
	Locate_xy(3,1);
	switch(flag)
	{
		case 0:
			Disp_Char(' ');
			Disp_Char(gps_lat[0]);
			Disp_Char(gps_lat[1]);
			Disp_Char(0xDF);
			Disp_Char(gps_lat[2]);
			Disp_Char(gps_lat[3]);
			Disp_Char(gps_lat[4]);
			Disp_Char(gps_lat[5]);
			Disp_Char(gps_lat[6]);
			Disp_Char(gps_lat[7]);
			Disp_Char(gps_lat[8]);
			Disp_Char(0x27);
			Disp_Char(gps_NS);
			break;
		case 1:
			Disp_Char(gps_lon[0]);
			Disp_Char(gps_lon[1]);
			Disp_Char(gps_lon[2]);
			Disp_Char(0xDF);
			Disp_Char(gps_lon[3]);
			Disp_Char(gps_lon[4]);
			Disp_Char(gps_lon[5]);
			Disp_Char(gps_lon[6]);
			Disp_Char(gps_lon[7]);
			Disp_Char(gps_lon[8]);
			Disp_Char(gps_lon[9]);
			Disp_Char(0x27);
			Disp_Char(gps_EW);
			break;
		case 2:
			Disp_Str_P(PSTR(" Alt.= "));
			
			//if((( gps_NoS[0] - 0x30 ) * 10 + gps_NoS[1] - 0x30 ) < 4)
			//	Disp_Str(" --- ");
			//else
			Disp_Str(gps_alt);
				
			Disp_Char('m');
			break;
		case 3:
			Disp_Str_P(PSTR("Save Position"));
			break;
		default:
			break;
	}
}

void disp_GPS_POS_SCN(void)
{
	XY_Str_P(0,0,PSTR("Lat: "));
	XY_Str(6,0,gps_lat);
	XY_Char(15,0,gps_NS);
	XY_Str_P(0,1,PSTR("Lon: "));
	XY_Str(5,1,gps_lon);
	XY_Char(15,1,gps_EW);
}