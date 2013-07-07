#ifndef __SCREEN_H__
#define __SCREEN_H__

#include <stdint.h>

#define BOOT_SCN		1
#define SPLASH_SCN		2
#define READY_SCN		3
#define GPS_UPDATE_SCN	4
#define GPS_MAIN_SCN	5
#define GPS_POS_SCN		6
#define GPS_TIME_SCN	7
#define SETTING_SCN		8

extern uint8_t scn_mode;

extern void set_screen(uint8_t mode);
extern void switch_scn_flag(uint8_t fg);
#endif