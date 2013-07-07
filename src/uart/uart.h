#ifndef __UART_H__
#define __UART_H__

#include "main.h"

#include <stdint.h>
#include <avr/pgmspace.h>

extern void uart_init(void);

#if DEBUG
extern void uart_putc_hex(uint8_t b);
#endif

extern void uart_putc(uint8_t c);
extern void uart_puts(const char* str);
extern void uart_puts_p(PGM_P str);

#endif

