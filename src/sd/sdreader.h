#ifndef __SDREAFER_H__
#define __SDREAFER_H__

#include <stdint.h>

extern char filename[];
extern char sd_buff[];
extern uint8_t sd_buff_len;

extern uint8_t sdreader_init(void);
extern uint8_t new_file(void);
extern uint8_t finish_file(void);
extern uint8_t write_buff(void);

#endif