#include <avr/io.h>
#include <stdint.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "lcd1602.h"

static void write(uint8_t temp);

/************************
* ���æ�ź�
************************/
void LCD_ReadBusy(void)
{
	LCD_DDR&=~(1<<BF);  //����
	LCD_PORT|=1<<BF;
	LCD_PORT&=~(1<<RS);
	LCD_PORT|=1<<RW;
	LCD_PORT|=1<<E;
  	while(chkbit(LCD_PIN,BF));  //��ѯ
	LCD_PORT&=~(1<<E);
	LCD_PORT&=~(1<<RS);
	LCD_PORT&=~(1<<RW);
	LCD_PORT|=1<<BF;//���
  	_delay_us(1); 
}

/************************
* lcd1602_дָ��
* data: Ҫд��LCD�Ĵ�����ָ��ֵ
************************/
void Write_Comand(uint8_t Comand)
{
  	LCD_ReadBusy();					//��ѯ״̬
  	LCD_PORT&=~(1<<RS);
	write(Comand);
	
	//LCD_PORT&=~(1<<RW);
 // 	LCD_DDR|=0xFF;    				//����Ϊ���
 // 	LCD_PORT|=1<<E;
	//LCD_PORT&=0x0F;   				//����λ����,������������
	//LCD_PORT|=Comand&0xF0; 			//�������λ
 // 	LCD_PORT&=~(1<<E);
 // 	LCD_DDR|=0xFF;    				//����Ϊ���
 // 	LCD_PORT|=1<<E;
	//LCD_PORT&=0x0F;  				//����λ����,������������
	//LCD_PORT|=(Comand<<4)&0xF0; 	//�������λ
 // 	LCD_PORT&=~(1<<E);
}

/************************
* lcd1602_д����
* data: Ҫд��LCD�Ĵ���������ֵ
************************/
void Write_Data(uint8_t data)
{
  	LCD_ReadBusy();				//��ѯ״̬
  	LCD_PORT|=1<<RS;
	write(data);
	
	//LCD_PORT&=~(1<<RW);
 // 	LCD_DDR|=0xFF;    			//����Ϊ���
 // 	LCD_PORT|=1<<E;
	//LCD_PORT&=0x0F;   			//����λ����,������������
	//LCD_PORT|=data&0xF0; 		//�������λ
 // 	LCD_PORT&=~(1<<E);
 // 	LCD_DDR|=0xFF;    			//����Ϊ���
 // 	LCD_PORT|=1<<E;
	//LCD_PORT&=0x0F;   			//����λ����,������������
	//LCD_PORT|=(data<<4)&0xF0; 	//�������λ
 // 	LCD_PORT&=~(1<<E);
}

void write(uint8_t temp)
{
	LCD_PORT&=~(1<<RW);
  	LCD_DDR|=0xFF;    			//����Ϊ���
  	LCD_PORT|=1<<E;
	LCD_PORT&=0x0F;   			//����λ����,������������
	LCD_PORT|=temp&0xF0; 		//�������λ
  	LCD_PORT&=~(1<<E);
  	LCD_DDR|=0xFF;    			//����Ϊ���
  	LCD_PORT|=1<<E;
	LCD_PORT&=0x0F;   			//����λ����,������������
	LCD_PORT|=(temp<<4)&0xF0; 	//�������λ
  	LCD_PORT&=~(1<<E);
}

/************************
* ��궨λ
* x,y: λ��
************************/
void Locate_xy(uint8_t x,uint8_t y)
{
	uint8_t address = (y == 0) ? (0x80 + x) : (0xc0 + x);
	Write_Comand(address);		
}

/************************
* ��ʾһ���ַ�
* data: Ҫ��ʾ���ַ�,ΪASCII��
************************/
void Disp_Char(char data)
{
  	Write_Data(data);
}

/************************
* ��(x,y)λ����ʾһ���ַ�
* x,y: λ��
* data: Ҫ��ʾ���ַ�,ΪASCII��
************************/
void XY_Char(uint8_t x,uint8_t y,char data)
{
	Locate_xy(x,y);
	Write_Data(data);
}

/************************
* ��ʾһ���ַ���
* String: Ҫ��ʾ���ַ���
************************/
void Disp_Str(char *String)
{
	unsigned int i=0;
	while(String[i]!='\0')
		Write_Data(String[i++]);
}

/************************
* ��(x,y)λ����ʾһ���ַ���
* x,y: λ��
* String: Ҫ��ʾ���ַ���
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
* ��ʾһ��0-9������
* num: Ҫ��ʾ������ 0~9
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
* ��(x,y)λ����ʾһ��0-9������
* x,y: λ��
* num: Ҫ��ʾ������ 0~9
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
* Һ����ʼ��
************************/
void LCD_Init(void)
{
	_delay_ms(20);
	_delay_ms(30);
	Write_Comand(0x30);
	Write_Comand(0x30);
	Write_Comand(0x30);
	Write_Comand(0x02);
	
	Write_Comand(0x28);		/*4λ����....*/
	Write_Comand(0x01);		/*����*/
	Write_Comand(0x02);		/*��λ*/
	Write_Comand(0x0c);		/*��ʾ������꿪����˸��*/
	Write_Comand(0x06);		/*����Զ����ƣ���ʾ���ֲ���*/
	Write_Comand(0x80);		/*������ʾ��ַ*/
}