/*
 * main.c
 *
 * Created: 11/11/2014 21:12:39
 *  Author: Reece
 */ 

#define F_CPU 12000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include "softpot.h"
#include "timer.h"
#include "uart.h"
#include "buttons.h"
#include "sseg.h"



int main(void)
{
	
	DDRB |= _BV(PB7);	//LED lights up when engine is off
	
	sseg_init();
	adc_init();
	buttons_init();
	timer_init();
	sei();
	
	adc_conv();		// starts continuous conversions between ADC
	//sseg_disp(8);
	
    while(1)
    {
	
    }

	return 0;
}