/*
 * RotaryEnc.c
 *
 * Created: 10/02/2015 16:09:18
 *  Author: Reece
 */ 



/*  Author: Steve Gunn
 * Licence: This work is licensed under the Creative Commons Attribution License. 
 *          View this license at http://creativecommons.org/about/licenses/
 *   Notes: F_CPU must be defined to match the clock frequency
 */


#define F_CPU 12000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>
#include "uart.h"
#include "rotaryenc.h"
#include "softpot.h"
#include "timer.h"



int main(void)
{
	uart_init();
	timer_init();
	rot_init();
	//softpot_init();
	sei();


    while(1)
    {
        //TODO:: Please write your application code 
    }
}
