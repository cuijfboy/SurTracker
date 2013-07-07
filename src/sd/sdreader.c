#include "main.h"
#include <avr/pgmspace.h>
#include "fat16.h"
#include "fat16_config.h"
#include "partition.h"
#include "sd_raw.h"
#include "sd_raw_config.h"
#include "uart.h"
#include "gps.h"
#include "sdreader.h"
#include "led.h"

char filename[] = "00000000.000";
char sd_buff[] = "123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_";
uint8_t sd_buff_len = 0;

uint8_t retry;

static struct partition_struct* partition;
static struct fat16_fs_struct* fs;
static struct fat16_dir_entry_struct rd;
static struct fat16_dir_struct* dd;

struct fat16_dir_entry_struct logfile_entry;
static struct fat16_file_struct* fd;

uint8_t sdreader_init(void)
{
	//LED_2_ON;
	retry = 5;
	
	/* setup sd card slot */
	while(!sd_raw_init() && retry)
	{
		retry--;
#if DEBUG
		uart_puts_p(PSTR("MMC/SD init failed\n"));
#endif	
	}
	
	if(retry)
		retry = 5;
	else
		return 1;

	/* open first partition */
	partition = partition_open(	sd_raw_read,
								sd_raw_read_interval,
								sd_raw_write,
								sd_raw_write_interval,
								0
							   );
	
	if(!partition)
	{
		/* If the partition did not open, assume the storage device
		 * is a "superfloppy", i.e. has no MBR.
		 */
		partition = partition_open(sd_raw_read,
								   sd_raw_read_interval,
								   sd_raw_write,
								   sd_raw_write_interval,
								   -1
								  );
		if(!partition)
		{
#if DEBUG
			uart_puts_p(PSTR("open partition failed\n"));
#endif
			return 2;
		}
	}

	/* open file system */
	fs = fat16_open(partition);
	if(!fs)
	{
#if DEBUG
		uart_puts_p(PSTR("open filesystem failed\n"));
#endif
		return 3;
	}

	/* open root directory */
	//fat16_get_dir_entry_of_path(fs, "/", &rd);
	fat16_get_root_dir_entry(fs, &rd);

	dd = fat16_open_dir(fs, &rd);
	if(!dd)
	{
#if DEBUG
		uart_puts_p(PSTR("opening root directory failed\n"));
#endif
		return 4;
	}
	
	return 0;
}

uint8_t new_file(void)
{
	retry = 26;
	
	while(retry && !fat16_create_file(dd, filename, &logfile_entry))
	{
		retry--;
#if DEBUG	
		uart_puts_p(PSTR("error creating file: "));
		uart_puts(filename);
		uart_puts_p(PSTR(". It existed!\n"));
#endif
		filename[7] += 1;
	}
	
	if(retry)
		retry = 10;
	else
	{
#if DEBUG	
		uart_puts_p(PSTR("error creating file: "));
		uart_puts(filename);
		uart_putc('\n');
#endif
		return 1;
	}
	
	fd = fat16_open_file(fs, &logfile_entry);
	if(!fd)
	{
#if DEBUG	
		uart_puts_p(PSTR("error open_file disk\n"));
		uart_putc('\n');
#endif		
		return 2;
	}
	
	return 0;		
}

uint8_t finish_file(void)
{
	fat16_close_file(fd);
	return 0;
}

uint8_t write_buff(void)
{
	if(fat16_write_file(fd, (uint8_t*) sd_buff, sd_buff_len) != sd_buff_len)
	{
#if DEBUG		
		uart_puts_p(PSTR("error write_file disk\n"));
		uart_putc('\n');
#endif			
		return 1;
	}
		
	if(!sd_raw_sync())
	{
#if DEBUG		
		uart_puts_p(PSTR("error syncing disk\n"));
		uart_putc('\n');
#endif			
		return 2;
	}
	
	return 0;
}
