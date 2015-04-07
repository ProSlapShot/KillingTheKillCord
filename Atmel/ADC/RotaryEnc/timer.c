/*
 * timer.c
 *
 * Created: 11/11/2014 22:08:23
 *  Author: Reece
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "rotaryenc.h"
#include "uart.h"



#define F_CPU 12000000
#define timer_freq 50

//static uint8_t rot_sel = 0;
//static uint8_t[8] rot_one = {0,0,0,0,0,0,0,0};
//static uint8_t[8] rot_two = {0,0,0,0,0,0,0,0};

void timer_init()
{
	OCR0A = F_CPU / 1024 / 50;   // Set up compare value for 50 Hz interrupt generation.
	TCCR0A = (1 << WGM01);               // Enable CTC mode
	TIMSK0 = _BV(OCIE0A);                // Enable Timer0 Compare Match A interrupt
	TCCR0B = (1 << CS02) | (0 << CS01) | (1 << CS00); // Initilise timer1 with /1024 prescaler	
}

ISR(TIMER0_COMPA_vect)
{
	rtr_intrpt();
	
	//uart_str("Rotary Encoder: ");
	//uart_data(rot_sel);
//	uart_str("\n");
	//uart_data(rtr_position(rot_sel));
	//rot_sel = !rot_sel;
	
	
	//rtr_position();
	//uart_data(rtr_position());
	//uart_data(rtr_value());
}
