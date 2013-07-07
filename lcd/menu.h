#ifndef __MENU_H__
#define __MENU_H__

#include <stdint.h>

extern char m_arrow;
extern char m_item[][9];

extern void menu_init(void);
extern uint8_t menu_start(void);

#endif