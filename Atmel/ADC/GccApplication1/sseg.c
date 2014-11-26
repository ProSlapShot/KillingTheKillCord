/*
 * sseg.c
 *
 * Created: 26/11/2014 01:06:58
 *  Author: Reece
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "sseg.h"

#define SSEG_DDR DDRD
#define SSEG_PORT PORTD

#define zero	_BV(0)| _BV(1)| _BV(2)| _BV(3)| _BV(4)| _BV(5)
#define one		_BV(1)| _BV(2)	
#define two		_BV(0)| _BV(1)| _BV(3)| _BV(4)| _BV(6)
#define three	_BV(0)| _BV(1)| _BV(2)| _BV(3)| _BV(6)
#define four	_BV(1)| _BV(2)| _BV(5)| _BV(6)
#define five	_BV(0)| _BV(2)| _BV(3)| _BV(5)| _BV(6)
#define six		_BV(0)| _BV(2)| _BV(3)| _BV(4)| _BV(5)| _BV(6)
#define seven	_BV(0)| _BV(1)| _BV(2)
#define eight	_BV(0)| _BV(1)| _BV(2)| _BV(3)| _BV(4)| _BV(5) | _BV(6)


static uint8_t sseg_pins = _BV(PD0) |_BV(PD1) |_BV(PD2) |_BV(PD3) |_BV(PD4) |_BV(PD5) |_BV(PD6);


void sseg_init()
{
	SSEG_DDR = sseg_pins;
	SSEG_PORT = sseg_pins;
}

void sseg_disp(int speed)
{
	switch(speed){
				case 0: SSEG_PORT = zero;
						break;
				case 1: SSEG_PORT = one;
						break;
				case 2: SSEG_PORT = two;
						break;
				case 3: SSEG_PORT = three;
						break;
				case 4: SSEG_PORT = four;
						break;
				case 5: SSEG_PORT = five;
						break;
				case 6: SSEG_PORT = six;
						break;
				case 7: SSEG_PORT = seven;
						break;
				case 8: SSEG_PORT = eight;
						break;
				default: SSEG_PORT = 0;
	}		
}