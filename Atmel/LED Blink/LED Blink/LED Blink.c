/*
 * LED_Blink.c
 *
 * Created: 18/11/2014 19:53:13
 *  Author: Reece
 */ 

#define F_CPU 12000000

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRB |= _BV(PB7);
	PORTB |= _BV(PB7);
    while(1)
    {
       _delay_ms(100);
	   PINB |= _BV(PB7); 
    }
}