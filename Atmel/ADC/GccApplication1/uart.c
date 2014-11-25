/*
 * uart.c
 *
 * Created: 11/11/2014 22:09:44
 *  Author: Reece
 */ 
#include "uart.h"

#define UART_BAUD 9600
#define F_CPU 12000000



void uart_init()
{
	// Configure UART0 baud rate
	UBRR0H = (F_CPU/(UART_BAUD*16L)-1) >> 8;
	UBRR0L = (F_CPU/(UART_BAUD*16L)-1);
	
	UCSR0B = _BV(RXEN0) | _BV(RXCIE0);		// Enable UART0 receiver and RX complete interrupt
	UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);		// one start bit, 8-bit, no parity and one stop
	
}

void uart_tx(uint8_t c)
{
	UDR1 = c;		//Send c
}

