/*
 * softpot.c
 *
 * Created: 15/04/2015 17:53:31
 *  Author: Reece
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "softpot.h"
#include "uart.h"

#define SPT_DDR  DDRB
#define SPT_PIN  PINB
#define SPT_PORT PORTB

uint8_t mappings[] = {0, 0, 0, 0, 0, 1, 2 ,3};	//Map port to periphals
uint8_t spt_pins = _BV(7) | _BV(6) | _BV(5);	//PB[7,6, 5]
uint8_t tmp = _BV(7) | _BV(6) | _BV(5);
uint8_t waiting = 0;

uint8_t throttle_flag = 0;
uint8_t steering_one_flag = 0;
uint8_t steering_two_flag = 0;



void softpot_init(){
	SPT_DDR |= ~spt_pins; //Set Port B as inputs
	SPT_PORT |= spt_pins; // Enable pull up resistors
	PCICR = _BV(PCIE1); //Enable interrupts on Port B
	PCMSK1 |= spt_pins; //Set pins on Port B as interrupts
	
	
	tmp = (SPT_PIN & spt_pins);		// Initial button status
}

ISR(PCINT1_vect)
{
	waiting |= (SPT_PIN & spt_pins) ^ tmp;		//Need to mask pins
	tmp = (SPT_PIN & spt_pins);
}

void set_flag(uint8_t peripheral){
	switch(peripheral){
	case 1 :
		if(!throttle_flag)
			uart_str("Throttle Held\n");
		throttle_flag = 1;
	case 2 :
		if(!steering_one_flag)
		uart_str("Steering One Held\n");
		steering_one_flag = 1;
	case 3 :
		if(!steering_two_flag)
		uart_str("Steering Two Held\n");
		steering_two_flag = 1;
	}	
}

void reset_flag(uint8_t peripheral){
	switch(peripheral){
		case 1 :
		if(throttle_flag)
		uart_str("Throttle Released\n");
		throttle_flag = 0;
		case 2 :
		if(steering_one_flag)
		uart_str("Steering One Released\n");
		steering_one_flag = 0;
		case 3 :
		if(steering_two_flag)
		uart_str("Steering Two Released\n");
		steering_two_flag = 0;
	}
}


void check_spt_status()
{
	if (waiting){
		uint8_t j = 0;		//Alter for min pin number
		do{
			uint8_t mask = (1 << j);
			if (waiting & mask){	//Detects touch
				waiting &= ~mask;
				if(mappings[j] != 0){
					if ((SPT_PIN & spt_pins) & mask){
						reset_flag(j);
					}
					else{
						set_flag(j);
					}
				}
			}
		} while(j++ != 7);
	}
}