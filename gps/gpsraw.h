#ifndef __GPSLOG_H__
#define __GPSLOG_H__

#include "stdint.h"

extern uint8_t gpsraw_flag;
extern char gps_msg_type;

extern uint8_t new_logfile(void);
extern uint8_t write_gps_head(void);
extern uint8_t finish_logfile(void);
extern uint8_t write_gps_msg(void);
extern void save_position(void);

#endif