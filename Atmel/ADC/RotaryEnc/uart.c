/*
 * uart.c
 *
 * Created: 11/11/2014 22:09:44
 *  Author: Reece
 */ 
#include "uart.h"
#include <avr/io.h>

#define UART_BAUD 9600
#define F_CPU 12000000




void uart_init(void)
{
	/* Configure 9600 baud, 8-bit, no parity and one stop bit */
	UBRR0H = (F_CPU/(UART_BAUD*16L)-1) >> 8;
	UBRR0L = (F_CPU/(UART_BAUD*16L)-1);
	UCSR0B = _BV(RXEN0) | _BV(TXEN0);
	UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
}

char get_ch(void)
{
	while(!(UCSR0A & _BV(RXC0)));
	return UDR0;
}


void uart_tx(char ch)
{

	while (!(UCSR0A & _BV(UDRE0)));
	UDR0 = ch;

}

void uart_data(uint8_t d)
{

	while (!(UCSR0A & _BV(UDRE0)));
	UDR0 = d;

}

void uart_str(char *str)
{
	int i;
	for(i=0; str[i]; i++) uart_tx(str[i]);
}

