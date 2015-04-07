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

#define RTR_DDR  DDRA
#define RTR_PIN  PINA
#define RTR_PORT PORTA	
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
	131,139,137	,129,128,132
	,148,212,244,240,242,250,251,249,241,209
	,193,197,196,192,64,66,74,106,122,120,121
	,125,253,252,248,232,224,226,98,96,32,33
	,37,53,61,60,188,190,254,126,124,116,112
	,113,49,48,16,144,146,154,158,30,94,95
};

static uint8_t pins = _BV(0) | _BV(1) | _BV(2) | _BV(3) | _BV(4) | _BV(5) | _BV(6) | _BV(7)  ;
static uint8_t tmp_one = _BV(0) | _BV(1) | _BV(2) | _BV(3) | _BV(4) | _BV(5) | _BV(6) | _BV(7)  ;
static uint8_t waiting_one = 0;
static uint8_t rtr_pos_one = 0;
static uint8_t tmp_two = _BV(0) | _BV(1) | _BV(2) | _BV(3) | _BV(4) | _BV(5) | _BV(6) | _BV(7)  ;
static uint8_t waiting_two = 0;
static uint8_t rtr_pos_two = 0;
static uint8_t sel = 0;

static uint8_t cbuf_one[75];
static uint8_t cbuf_one_dif[74];
static uint8_t cbuf_two[75];
static uint8_t cbuf_two_dif[74];


void rot_init()
{
		RTR_DDR |= ~pins; //Set Port C as inputs
		RTR_PORT |= pins; // Enable pull up resistors
		
		PCICR = _BV(PCIE0); //Enable interrupts on Port B
		PCMSK0 |= pins; //Set pins on Port B as interrupts	
		
		DDRD |= _BV(PIND3);			//Rotary Select Pin	
		PORTD |= _BV(PIND3);
			
		tmp_two = (RTR_PIN & pins);		// Initial encoder status (2)
		
		PORTD &= ~_BV(PIND3);
		tmp_one = (RTR_PIN & pins);		
}

ISR(PCINT0_vect)
{
	if(!sel){
		waiting_one |= (RTR_PIN & pins) ^ tmp_one;		//Need to mask pins
		tmp_one = (RTR_PIN & pins);
	}
	else{
		waiting_two |= (RTR_PIN & pins) ^ tmp_two;		//Need to mask pins
		tmp_two = (RTR_PIN & pins);
	}
}

uint8_t rtr_value()
{
	return RTR_PIN & pins;
}


uint8_t rtr_position(uint8_t x)
{
	if(!x){								//Retrieves encoder one position (0-128) 
		PORTD |= _BV(PIND3);
		if (waiting_one)
		{
			uint8_t i = 0;
			
			for(i=0 ; i<=127 ; i++)
			{
				if(rtrlut[i] == rtr_value())
				{
					rtr_pos_one = i;
					return i;
				}
			}
			
			return RTR_INVALID;
		}
		
		return rtr_pos_one;
	}
	else if(x){							//Retrieves encoder two position (0-128) 
		PORTD &= ~_BV(PIND3);
		if (waiting_two)
		{
			uint8_t i = 0;
			
			for(i=0 ; i<=127 ; i++)
			{
				if(rtrlut[i] == rtr_value())
				{
					rtr_pos_two = i;
					return i;
				}
			}
			
			return RTR_INVALID;
		}
		
		return rtr_pos_two;
	}
	else
	return RTR_INVALID;
}

void rtr_intrpt()
{
	if(!sel){
		static uint8_t buff_one = 0;		// Monitors if position has changed = easier to read information
		rtr_pos_one = rtr_position(sel);
		if(buff_one != rtr_pos_one){
			uart_str("\nRotary ");
			uart_number(sel);
			uart_str(" Pos :");
			uart_number(rtr_pos_one);
		}
		buff_one = rtr_pos_one;
		sel = 1;
	}else{
		static uint8_t buff_two = 0;
		rtr_pos_two = rtr_position(sel);
		if(buff_two != rtr_pos_two){
			uart_str("\nRotary ");
			uart_number(sel);
			uart_str(" Pos :");
			uart_number(rtr_pos_two);
		}
		buff_two = rtr_pos_two;
		sel = 0;
	}
}






