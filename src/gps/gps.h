#ifndef __GPS_H__
#define __GPS_H__

#include <stdint.h>

extern char gps_time[];
extern char gps_date[];
extern char gps_lat[];
extern char gps_NS;
extern char gps_lon[];
extern char gps_EW;
extern char gps_alt[];
extern char gps_state;
extern char gps_NoS[];
extern char gps_spd[];
extern char gps_dir[];


extern uint8_t gps_msg_ready;

extern void gps_init(void);
extern void gps_set_arg(void);

#if DEBUG
extern void print_gps_msg(void);
#endif

#endif