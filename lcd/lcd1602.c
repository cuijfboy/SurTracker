#include <avr/io.h>
#include <stdint.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "lcd1602.h"

static void write(uint8_t temp);

/************************
* 检测忙信号
************************/
void LCD_ReadBusy(void)
{
	LCD_DDR&=~(1<<BF);  //输入
	LCD_PORT|=1<<BF;
	LCD_PORT&=~(1<<RS);
	LCD_PORT|=1<<RW;
	LCD_PORT|=1<<E;
  	while(chkbit(LCD_PIN,BF));  //查询
	LCD_PORT&=~(1<<E);
	LCD_PORT&=~(1<<RS);
	LCD_PORT&=~(1<<RW);
	LCD_PORT|=1<<BF;//输出
  	_delay_us(1); 
}

/************************
* lcd1602_写指令
* data: 要写到LCD寄存器的指令值
************************/
void Write_Comand(uint8_t Comand)
{
  	LCD_ReadBusy();					//查询状态
  	LCD_PORT&=~(1<<RS);
	write(Comand);
	
	//LCD_PORT&=~(1<<RW);
 // 	LCD_DDR|=0xFF;    				//设置为输出
 // 	LCD_PORT|=1<<E;
	//LCD_PORT&=0x0F;   				//高四位清零,即数据线清零
	//LCD_PORT|=Comand&0xF0; 			//输出高四位
 // 	LCD_PORT&=~(1<<E);
 // 	LCD_DDR|=0xFF;    				//设置为输出
 // 	LCD_PORT|=1<<E;
	//LCD_PORT&=0x0F;  				//高四位清零,即数据线清零
	//LCD_PORT|=(Comand<<4)&0xF0; 	//输出低四位
 // 	LCD_PORT&=~(1<<E);
}

/************************
* lcd1602_写数据
* data: 要写到LCD寄存器的数据值
************************/
void Write_Data(uint8_t data)
{
  	LCD_ReadBusy();				//查询状态
  	LCD_PORT|=1<<RS;
	write(data);
	
	//LCD_PORT&=~(1<<RW);
 // 	LCD_DDR|=0xFF;    			//设置为输出
 // 	LCD_PORT|=1<<E;
	//LCD_PORT&=0x0F;   			//高四位清零,即数据线清零
	//LCD_PORT|=data&0xF0; 		//输出高四位
 // 	LCD_PORT&=~(1<<E);
 // 	LCD_DDR|=0xFF;    			//设置为输出
 // 	LCD_PORT|=1<<E;
	//LCD_PORT&=0x0F;   			//高四位清零,即数据线清零
	//LCD_PORT|=(data<<4)&0xF0; 	//输出低四位
 // 	LCD_PORT&=~(1<<E);
}

void write(uint8_t temp)
{
	LCD_PORT&=~(1<<RW);
  	LCD_DDR|=0xFF;    			//设置为输出
  	LCD_PORT|=1<<E;
	LCD_PORT&=0x0F;   			//高四位清零,即数据线清零
	LCD_PORT|=temp&0xF0; 		//输出高四位
  	LCD_PORT&=~(1<<E);
  	LCD_DDR|=0xFF;    			//设置为输出
  	LCD_PORT|=1<<E;
	LCD_PORT&=0x0F;   			//高四位清零,即数据线清零
	LCD_PORT|=(temp<<4)&0xF0; 	//输出低四位
  	LCD_PORT&=~(1<<E);
}

/************************
* 光标定位
* x,y: 位置
************************/
void Locate_xy(uint8_t x,uint8_t y)
{
	uint8_t address = (y == 0) ? (0x80 + x) : (0xc0 + x);
	Write_Comand(address);		
}

/************************
* 显示一个字符
* data: 要显示的字符,为ASCII码
************************/
void Disp_Char(char data)
{
  	Write_Data(data);
}

/************************
* 在(x,y)位置显示一个字符
* x,y: 位置
* data: 要显示的字符,为ASCII码
************************/
void XY_Char(uint8_t x,uint8_t y,char data)
{
	Locate_xy(x,y);
	Write_Data(data);
}

/************************
* 显示一个字符串
* String: 要显示的字符串
************************/
void Disp_Str(char *String)
{
	unsigned int i=0;
	while(String[i]!='\0')
		Write_Data(String[i++]);
}

/************************
* 在(x,y)位置显示一个字符串
* x,y: 位置
* String: 要显示的字符串
************************/
void XY_Str(uint8_t x,uint8_t y,char *String)
{
	unsigned int i=0;
	Locate_xy(x,y);
	//DispString(*String);
	while(String[i]!='\0')
		Write_Data(String[i++]);
}

/************************
* 显示一个0-9的数字
* num: 要显示的数字 0~9
************************/
void Disp_Num(uint8_t num)
{
  	Write_Data(num+0x30);
}

void Disp_Str_P(PGM_P str)
{
	uint8_t b;
	while(1)
    {
        b = pgm_read_byte_near(str++);
        if(!b)
            break;

        Write_Data(b);
    }
}

void XY_Str_P(uint8_t x,uint8_t y,PGM_P str)
{
	uint8_t b;
	Locate_xy(x,y);
	while(1)
    {
        b = pgm_read_byte_near(str++);
        if(!b)
            break;

        Write_Data(b);
    }
}

/************************
* 在(x,y)位置显示一个0-9的数字
* x,y: 位置
* num: 要显示的数字 0~9
************************/
void XY_Num(uint8_t x,uint8_t y,uint8_t num)
{
	Locate_xy(x,y);
	Write_Data(num+0x30);
}

void Disp_Clear(void)
{
	Write_Comand(0x01);	
	//_delay_ms(2);
}

/************************
* 液晶初始化
************************/
void LCD_Init(void)
{
	_delay_ms(20);
	_delay_ms(30);
	Write_Comand(0x30);
	Write_Comand(0x30);
	Write_Comand(0x30);
	Write_Comand(0x02);
	
	Write_Comand(0x28);		/*4位总线....*/
	Write_Comand(0x01);		/*清屏*/
	Write_Comand(0x02);		/*归位*/
	Write_Comand(0x0c);		/*显示开，光标开，闪烁开*/
	Write_Comand(0x06);		/*光标自动右移，显示文字不动*/
	Write_Comand(0x80);		/*设置显示地址*/
}