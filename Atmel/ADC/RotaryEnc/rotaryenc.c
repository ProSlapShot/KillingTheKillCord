/*
 * rotaryenc.c
 *
 * Created: 10/02/2015 16:10:50
 *  Author: Reece
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "rotaryenc.h"
#include "uart.h"

#define RTR_DDR  DDRB
#define RTR_PIN  PINB
#define RTR_PORT PORTB
#define RTR_INVALID 255

static const uint8_t rtrlut[128] =
{
	127,63,62,58,56,184,152,24,8,72,
	73,77,79,15,47,175,191,159,31,29,
	28,92,76,12,4,36,164,166,167,135,
	151,215,223,207,143,142,14,46,38,
	6,2,18,82,83,211,195,203,235,239,
	231,199,71,7,23,19,3,1,9,41,169,
	233,225,229,245,247,243,227,163,
	131,139,137,129,128,132
	,148,212,244,240,242,250,251,249,241,209
	,193,197,196,192,64,66,74,106,122,120,121
	,125,253,252,248,232,224,226,98,96,32,33
	,37,53,61,60,188,190,254,126,124,116,112
	,113,49,48,16,144,146,154,158,30,94,95
};

static uint8_t pins = _BV(0) | _BV(1) | _BV(2) | _BV(3) | _BV(4) | _BV(5) | _BV(6) | _BV(7)  ;
static uint8_t tmp = _BV(0) | _BV(1) | _BV(2) | _BV(3) | _BV(4) | _BV(5) | _BV(6) | _BV(7)  ;
static uint8_t waiting = 0;
static uint8_t rtr_pos = 0;

void rot_init()
{
		RTR_DDR |= ~pins; //Set Port C as inputs
		RTR_PORT |= pins; // Enable pull up resistors
		
		PCICR = _BV(PCIE1); //Enable interrupts on Port B
		PCMSK1 |= pins; //Set pins on Port B as interrupts			
			
		tmp = (RTR_PIN & pins);		// Initial button status
		
}

ISR(PCINT1_vect)
{
	waiting |= (RTR_PIN & pins) ^ tmp;		//Need to mask pins
	tmp = (RTR_PIN & pins);
}

uint8_t rtr_value()
{
	return RTR_PIN & pins;
}


uint8_t rtr_position()
{
	if (waiting)
	{
		//uart_str("Entered WAITING function.\n");
		uint8_t i = 0;
	
		for(i=0 ; i<=127 ; i++)
		{
			if(rtrlut[i] == rtr_value())
			{
			/*	uart_str("Looking for value.\n");
				uart_str("i = ");
				uart_data(i);
				uart_str("\nRotary Position = ");
				uart_data(rtr_pos);
				uart_str("\n");		*/
				rtr_pos = i;
				return i;
			}
		}
	
		return RTR_INVALID;
	}
	//uart_str("No Pin change \n");
	
	return rtr_pos;
}






