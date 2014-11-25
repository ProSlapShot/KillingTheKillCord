/*
 * CFile1.c
 *
 * Created: 24/11/2014 15:50:45
 *  Author: Reece
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "buttons.h"
#include "timer.h"
#include "softpot.h"

#define BTN_DDR  DDRB
#define BTN_PIN  PINB
#define BTN_PORT PORTB

uint8_t mappings[] = { 1, 2, 0, 0, 0, 0, 0, 0 }; //RH
static uint8_t pins = _BV(0) | _BV(1) ;	//PB[3,4,5,6,7]
static uint8_t tmp = _BV(0) | _BV(1) ;
static uint8_t waiting = 0;

void buttons_init()
{
	BTN_DDR |= ~pins; //Set Port B as inputs
	BTN_PORT |= pins; // Enable pull up resistors
	PCICR = _BV(PCIE1); //Enable interrupts on Port B
	PCMSK1 |= pins; //Set pins on Port B as interrupts
	
	
	tmp = (BTN_PIN & pins);		// Initial button status
}


ISR(PCINT1_vect)  //interrupt for pin A0
{
	waiting |= (BTN_PIN & pins) ^ tmp;		//Need to mask pins
	tmp = (BTN_PIN & pins);
}

void check_button_status()
{
	
	if (waiting)
	{
		uint8_t i = 0;
		
		do
		{
			uint8_t mask = (1 << i);
			if (waiting & mask)
			{
				waiting &= ~mask;
				if(mappings[i] != 0)
				{
					if ((BTN_PIN & pins) & mask)
					{
						/* Button depress action */
					}
					else
					{
						if(mappings[i] == 1)
							engine_start();		
						if(mappings[i] == 2)
							calibrate();
					}
				}
			}
		} while(i++ != 7);
	}
}