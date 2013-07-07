
#ifndef __LCD1602_H__
#define __LCD1602_H__

#include <stdint.h>
#include <avr/pgmspace.h>

#define chkbit(PORT,BIT) (PORT&(1<<(BIT)))

#define LCD_PORT PORTA      //LCD_DATA_PORT
#define LCD_DDR  DDRA
#define LCD_PIN  PINA

#define RS PA1
#define RW PA2
#define E  PA3
#define BF PA7

//¶¨ÒåÒº¾§16*2
//#define L  16
//#define H  2

extern void LCD_ReadBusy(void);
extern void Write_Comand(uint8_t Comand);
extern void Write_Data(uint8_t data);
extern void Locate_xy(uint8_t x,uint8_t y);
extern void Disp_Char(char data);
extern void XY_Char(uint8_t x,uint8_t y,char data);
extern void Disp_Str(char *String);
extern void XY_Str(uint8_t x,uint8_t y,char *String);
extern void Disp_Str_P(PGM_P str);
extern void XY_Str_P(uint8_t x,uint8_t y,PGM_P str);
extern void Disp_Num(uint8_t num);
extern void XY_Num(uint8_t x,uint8_t y,uint8_t num);
extern void Disp_Clear(void);
extern void LCD_Init(void);

#endif
