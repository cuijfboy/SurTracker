#include "timer.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "gps.h"
#include "gpsraw.h"
#include "led.h"
#include "screen.h"

uint32_t time_0 = 0;
uint32_t time_1 = 0;
uint8_t timer0_flag = 0;
uint8_t timer1_flag = 0;

void timer0_init(void)
{
	time_0 = 0;
	
    // Timer/Counter 0 initialization
    // Clock source: System Clock
    // Clock value: 12.5 kHz
    // Mode: Normal top=FFh
	// Interrupt every 1 ms.
    TCCR0 = 0x00;
	//TCCR0 = 0x03;	//64
    TCNT0 = 0x80;
	
	// Timer(s)/Counter(s) Interrupt(s) initialization
    TIMSK |= (1<<0);
}

void timer0_start(void)
{
	TCCR0 = 0x03;	//64
}

void timer0_stop(void)
{
	TCCR0 = 0x00;
}

void timer0_reset(void)
{
	TCNT0 = 0x80;
}

void timer1_init(void)
{
	time_1 = 0;

    // Timer/Counter 1 initialization
    // Clock source: System Clock
    // Clock value: 7812.5 Hz
    // Mode: Normal top=FFFFh
    // Noise Canceler: Off
    // Input Capture on Falling Edge
    // Timer 1 Overflow Interrupt: On
    TCCR1A = 0x00;
    //TCCR1B = 0x05;	//1024
	TCCR1B = 0x00;
    TCNT1H = 0x67;
    TCNT1L = 0x6B;
	
	// Timer(s)/Counter(s) Interrupt(s) initialization
    TIMSK |= (1<<2);
	//TIMSK = 0x05;
}

void timer1_start(void)
{
	TCCR1A = 0x00;
	TCCR1B = 0x05;
}

void timer1_stop(void)
{
	TCCR1A = 0x00;
	TCCR1B = 0x00;
}

void timer1_reset(void)
{
	TCNT1H = 0x67;
    TCNT1L = 0x6B;
}


ISR(TIMER0_OVF_vect) 
{ 
	cli();
	
	timer0_reset();
	time_0++;
	timer0_flag = 1;
	
	LED_switch(LED_0);
	
	sei();
} 

ISR(TIMER1_OVF_vect) 
{ 
	cli();
	
	timer1_reset();
	time_1++;
	timer1_flag = 1;
	
	LED_switch(LED_1);
	
	sei();
}