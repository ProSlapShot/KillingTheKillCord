/*
 * timer.c
 *
 * Created: 11/11/2014 22:08:23
 *  Author: Reece
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "buttons.h"


#define F_CPU 12000000
#define timer_freq 50

int count = 0;
int i = 0;
int flash_count = 0;

int throttle_flag = 0;
int wheel_flag = 0;
int led_flash = 0;

void timer_init()
{
	OCR0A = F_CPU / 1024 / timer_freq;   // Set up compare value for 50 Hz interrupt generation.
	TCCR0A = (1 << WGM01);               // Enable CTC mode
	TIMSK0 = _BV(OCIE0A);                // Enable Timer0 Compare Match A interrupt
	TCCR0B = (1 << CS02) | (0 << CS01) | (1 << CS00); // Initilise timer1 with /1024 prescaler	
}

void reset_wheel_flag()
{
	wheel_flag = 0;
}

void set_wheel_flag()
{
	wheel_flag = 1;
}

void reset_throttle_flag()
{
	throttle_flag = 0;
}

void set_throttle_flag()
{
	throttle_flag = 1;
}

ISR(TIMER0_COMPA_vect)
{
	/* Check length of time controls are left untouched */
	
	if(throttle_flag | wheel_flag)
	{
		count ++;
		if(count == 250)		// can be changed depending on delay 
			PORTB |= _BV(PB7);
	}
	else
	count = 0;
	
	/* Debouncing for input buttons */
	check_button_status();
	
	/*Flash LED on unsuccessful start up*/
	if(led_flash)
	{
		if(i>=25)
		{
			PINB |= _BV(PB7);
			i++;
		}
		else
		{
			i = 0;
			flash_count++;			
		}
		if (flash_count >= 6)
		{
			led_flash = 0;
			PORTB |= _BV(PB7);
		}
	}
}

void engine_start()
{
	if(!(throttle_flag|wheel_flag))
		{
			PORTB ^= ~_BV(PB7);			//LED shows status of engine
		}
	else
		led_flash = 1;
}