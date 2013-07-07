#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdint.h>

#define DEBUG	0

#define SYS_INIT 		1
#define SYS_READY		2
#define SYS_WORKING 	3
#define SYS_SETTING 	4

extern uint8_t sys_state; 
extern uint8_t sd_avlb;
extern uint8_t gpx_avlb;
extern uint8_t speeder_avlb;
extern uint8_t gps_avlb;

#endif