#include "gpsraw.h"

#include <stdint.h>
#include "gps.h"
#include "led.h"
#include "sdreader.h"
#include "screen.h"

uint8_t gpsraw_flag = 0;
char gps_msg_type = 'N';

uint8_t new_logfile(void)
{
	
	LED_switch(LED_2);
	
	filename[0] = gps_date[4];
	filename[1] = gps_date[5];
	filename[2] = gps_date[2];
	filename[3] = gps_date[3];
	filename[4] = gps_date[0];
	filename[5] = gps_date[1];
	filename[6] = '_';
	filename[7] = 'A';
	filename[8] = '.';
	filename[9] = 'r';
	filename[10] = 'a';
	filename[11] = 'w';
	
	gpsraw_flag = 1;
	
	return new_file();
}

uint8_t write_gps_head(void)
{
	uint8_t sd_buff_count = 0;
	uint8_t count = 0;
	
	LED_switch(LED_2);
	
	sd_buff[sd_buff_count] = 'G';
	sd_buff_count++;
	sd_buff[sd_buff_count] = 'P';
	sd_buff_count++;
	sd_buff[sd_buff_count] = 'S';
	sd_buff_count++;
	sd_buff[sd_buff_count] = '_';
	sd_buff_count++;
	sd_buff[sd_buff_count] = 'S';
	sd_buff_count++;
	sd_buff[sd_buff_count] = 'T';
	sd_buff_count++;
	sd_buff[sd_buff_count] = 'A';
	sd_buff_count++;
	sd_buff[sd_buff_count] = 'R';
	sd_buff_count++;
	sd_buff[sd_buff_count] = 'T';
	sd_buff_count++;
	sd_buff[sd_buff_count] = '@';
	sd_buff_count++;
	sd_buff[sd_buff_count] = '\"';
	sd_buff_count++;
	
	for(count=0;count<12;count++)
	{
		sd_buff[sd_buff_count] = filename[count];
		sd_buff_count++;
	}
	
	sd_buff[sd_buff_count] = '\"';
	sd_buff_count++;
	sd_buff[sd_buff_count] = '\n';
	sd_buff_count++;
	
	sd_buff_len = sd_buff_count;
	
	gpsraw_flag = 2;
	
	return write_buff();
}

uint8_t finish_logfile(void)
{
	uint8_t result;
	uint8_t sd_buff_count = 0;
	
	LED_switch(LED_2);
	
	sd_buff[sd_buff_count] = 'G';
	sd_buff_count++;
	sd_buff[sd_buff_count] = 'P';
	sd_buff_count++;
	sd_buff[sd_buff_count] = 'S';
	sd_buff_count++;
	sd_buff[sd_buff_count] = '_';
	sd_buff_count++;
	sd_buff[sd_buff_count] = 'F';
	sd_buff_count++;
	sd_buff[sd_buff_count] = 'I';
	sd_buff_count++;
	sd_buff[sd_buff_count] = 'N';
	sd_buff_count++;
	sd_buff[sd_buff_count] = 'I';
	sd_buff_count++;
	sd_buff[sd_buff_count] = 'S';
	sd_buff_count++;
	sd_buff[sd_buff_count] = 'H';
	sd_buff_count++;
	sd_buff[sd_buff_count] = '\n';
	sd_buff_count++;
	
	sd_buff_len = sd_buff_count;
	
	result = write_buff();
	if(result)
	{
		return result;
	}
	else
	{
		gpsraw_flag = 0;
		return finish_file();
	}
	
	return 0;
}

void save_position(void)
{
	switch_scn_flag(3);
	set_screen(GPS_MAIN_SCN);
	gps_msg_type = 'A';
	write_gps_msg();
	gps_msg_type = 'N';
}

uint8_t write_gps_msg(void)
{
	LED_switch(LED_2);
	
	uint8_t sd_buff_count = 0;
	uint8_t count = 0;
	
	sd_buff[sd_buff_count] = '$';
	sd_buff_count++;
	sd_buff[sd_buff_count] = '|';
	sd_buff_count++;
		
	for(count=0;count<6;count++)
	{
		sd_buff[sd_buff_count] = gps_date[count];
		sd_buff_count++;
	}
	
	sd_buff[sd_buff_count] = '|';
	sd_buff_count++;
	
	for(count=0;count<10;count++)
	{
		sd_buff[sd_buff_count] = gps_time[count];
		sd_buff_count++;
	}
	
	sd_buff[sd_buff_count] = '|';
	sd_buff_count++;
	
	sd_buff[sd_buff_count] = gps_state;
	sd_buff_count++;

	sd_buff[sd_buff_count] = '|';
	sd_buff_count++;
	
	for(count=0;count<9;count++)
	{
		sd_buff[sd_buff_count] = gps_lat[count];
		sd_buff_count++;
	}
	
	sd_buff[sd_buff_count] = '|';
	sd_buff_count++;
	
	sd_buff[sd_buff_count] = gps_NS;
	sd_buff_count++;

	sd_buff[sd_buff_count] = '|';
	sd_buff_count++;
	
	for(count=0;count<10;count++)
	{
		sd_buff[sd_buff_count] = gps_lon[count];
		sd_buff_count++;
	}
	
	sd_buff[sd_buff_count] = '|';
	sd_buff_count++;
	
	sd_buff[sd_buff_count] = gps_EW;
	sd_buff_count++;
	
	sd_buff[sd_buff_count] = '|';
	sd_buff_count++;
	
	for(count=0;count<5;count++)
	{
		sd_buff[sd_buff_count] = gps_alt[count];
		sd_buff_count++;
	}
	
	sd_buff[sd_buff_count] = '|';
	sd_buff_count++;
	
	for(count=0;count<2;count++)
	{
		sd_buff[sd_buff_count] = gps_NoS[count];
		sd_buff_count++;
	}
	
	sd_buff[sd_buff_count] = '|';
	sd_buff_count++;
	
	for(count=0;count<5;count++)
	{
		sd_buff[sd_buff_count] = gps_spd[count];
		sd_buff_count++;
	}
	
	sd_buff[sd_buff_count] = '|';
	sd_buff_count++;
	
	for(count=0;count<6;count++)
	{
		sd_buff[sd_buff_count] = gps_dir[count];
		sd_buff_count++;
	}
	
	sd_buff[sd_buff_count] = '|';
	sd_buff_count++;
	
	sd_buff[sd_buff_count] = gps_msg_type;
	sd_buff_count++;
	
	sd_buff[sd_buff_count] = '|';
	sd_buff_count++;
	
	sd_buff[sd_buff_count] = '#';
	sd_buff_count++;
	
	sd_buff[sd_buff_count] = '\n';
	sd_buff_count++;
	
	sd_buff_len = sd_buff_count;
	
#if DEBUG            
		uart_puts(sd_buff);
#endif
	
	return write_buff();
}