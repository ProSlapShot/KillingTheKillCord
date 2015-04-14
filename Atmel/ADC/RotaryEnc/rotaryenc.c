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

static uint8_t rtrlut_fast[256] = {0};

static uint8_t pins = _BV(0) | _BV(1) | _BV(2) | _BV(3) | _BV(4) | _BV(5) | _BV(6) | _BV(7)  ;		//Pins for encoder
static uint8_t tmp_one = _BV(0) | _BV(1) | _BV(2) | _BV(3) | _BV(4) | _BV(5) | _BV(6) | _BV(7)  ;	//Variables to hold states
static uint8_t waiting_one = 0;
static uint8_t rtr_pos_one = 0;
static uint8_t tmp_two = _BV(0) | _BV(1) | _BV(2) | _BV(3) | _BV(4) | _BV(5) | _BV(6) | _BV(7)  ;
static uint8_t waiting_two = 0;
static uint8_t rtr_pos_two = 0;
static uint8_t sel = 0;			//Encoder Select

static uint8_t cbuf_one[75] = {0};		//Circular buffers & differences
static int cbuf_one_dif[75] = {0};
static uint8_t cbuf_two[75] = {0};
static int cbuf_two_dif[75] = {0};

static uint8_t one_bpos = 0;		//Circular buffer positions
static uint8_t two_bpos = 0;


void rot_init()
{
		RTR_DDR |= ~pins; //Set Port C as inputs
		RTR_PORT |= pins; // Enable pull up resistors
		
		//PCICR = _BV(PCIE0); //Enable interrupts on Port B
		//PCMSK0 |= pins; //Set pins on Port B as interrupts	
		
		DDRD |= _BV(PIND3);			//Rotary Select Pin	
		PORTD |= _BV(PIND3);
			
		tmp_two = (RTR_PIN & pins);		// Initial encoder status (2)
		
		PORTD &= ~_BV(PIND3);
		tmp_one = (RTR_PIN & pins);		
		
		int fill = 0;
		for(fill = 0; fill <= 255; fill++)	// Set all invalid en
			rtrlut_fast[fill] = 255;
		for(fill = 0; fill <= 127; fill++)
			rtrlut_fast[rtrlut[fill]] = fill; 
		
			
}

ISR(PCINT0_vect)
{
	uart_str("Button interrupt\n");
	if(!sel){
		waiting_one |= (RTR_PIN & pins) ^ tmp_one;		//Need to mask pins
		tmp_one = (RTR_PIN & pins);
	}
	else{
		waiting_two |= (RTR_PIN & pins) ^ tmp_two;		//Need to mask pins
		tmp_two = (RTR_PIN & pins);
	}
}
uint8_t rtr_qpos(uint8_t x)
{
	if(!x){								//Retrieves encoder one position (0-128)
		PORTD |= _BV(PIND3);
		if (waiting_one){
			rtr_pos_one = rtrlut_fast[rtr_value()];
			return rtrlut_fast[rtr_value()];
		}
		
		return rtr_pos_one;
	}
	else if(x){							//Retrieves encoder two position (0-128)
		PORTD &= ~_BV(PIND3);
		if (waiting_two)
			if (waiting_one){
				rtr_pos_two = rtrlut_fast[rtr_value()];
				return rtrlut_fast[rtr_value()];
			}
			
		
		return rtr_pos_two;
	}
	else
	return RTR_INVALID;
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

void rtr_buff_update()
{
	uint8_t im1;
	uint8_t jm1;

	if(one_bpos == 0)			//Take care of circlular buffer "overflow"
		im1 = 74;
	else
		im1 = one_bpos-1;

	if(two_bpos == 0)
		jm1 = 74;
	else
		jm1 = two_bpos-1;

	if(!sel){
		cbuf_one[one_bpos] = rtr_qpos(sel);					//Update Cbuf and Cdif 
		if((cbuf_one[one_bpos] <2) && (cbuf_one[im1] >126))
			cbuf_one_dif[one_bpos] = 1;
		else if((cbuf_one[one_bpos] >126) && (cbuf_one[im1] <2))
			cbuf_one_dif[one_bpos] = -1;
		else
			cbuf_one_dif[one_bpos] = cbuf_one[one_bpos] - cbuf_one[im1];

		if(one_bpos == 74)
			one_bpos = 0;
		else
			one_bpos++;
	}
	else{
		cbuf_two[two_bpos] = rtr_qpos(sel);
		if((cbuf_two[two_bpos] <2) && (cbuf_two[jm1] >126))
			cbuf_two_dif[two_bpos] = 1;
		else if((cbuf_two[two_bpos] >126) && (cbuf_two[jm1] <2))
			cbuf_two_dif[two_bpos] = -1;
		else
			cbuf_two_dif[two_bpos] = cbuf_two[two_bpos] - cbuf_two[jm1];

		if(two_bpos == 74)
			two_bpos = 0;
		else
			two_bpos++;
		//uart_number(cbuf_two_dif[jm1]);
		//uart_str("\n");
	}

		
}


void rtr_speed()
{
	int speed_acc_one = 0;
	int speed_acc_two = 0;
	uint8_t index = 0;

	uint8_t acc;		//25 positions per second = speed over a second
	for(acc = 0; acc <= 25; acc++){
		if(one_bpos-acc <= -1)
			index = 74 - (acc-one_bpos);
		else 
			index = one_bpos-acc; 
		if(two_bpos-acc <= -1)
			index = 74 - (acc-two_bpos);
		else
			index = two_bpos-acc;

		if((index < 0) | (index >74))		//Check for out of bounds access
			uart_str("INDEX ERROR");
			
		speed_acc_one += cbuf_one_dif[index];
		speed_acc_two += cbuf_two_dif[index];
	}
	//uart_number(speed_acc_one);
	//uart_str("\r\n");
	if(speed_acc_one > 32)
		uart_str("Rotary encoder 1 WARNING\n");
	if(speed_acc_two > 32)
		uart_str("Rotary encoder 2 WARNING\n");
}

void rtr_speed_update()
{
	static int speed_update_acc_one = 0;
	static int speed_update_acc_two = 0;
	
	static int speed_counter_one = 0;
	static int speed_counter_two = 0;
	
	static uint8_t speed_flag_one = 0;
	static uint8_t speed_flag_buff = 0;
	static uint8_t speed_flag_two = 0;
	
	if(cbuf_one_dif[one_bpos] != 0){
		speed_update_acc_one += cbuf_one_dif[one_bpos];
		speed_counter_one = 0;
	}
	else if((cbuf_one_dif[one_bpos] == 0) && (speed_update_acc_one > 0)){	//Will set the accumulator back to zero & allows for 'cool off' period
			speed_update_acc_one -= 1;
	}
	else if((cbuf_one_dif[one_bpos] == 0) && (speed_update_acc_one < 0)){	//Will set the accumulator back to zero & allows for 'cool off' period
		speed_update_acc_one += 1;
	}
	//else if((cbuf_one_dif[one_bpos] == 0) && (speed_counter_one = 3) && (speed_update_acc_one != 0)){	// Will set the accumulator back to zero & allows for 'cool off' period
		//speed_update_acc_one -= -3;
		//speed_counter_one = 0;
	//}
	
	if((speed_update_acc_one > 32) | (speed_update_acc_one < -32))
		speed_flag_one = 1;
	else if((speed_update_acc_one < 32) && (speed_update_acc_one > -32))
		speed_flag_one = 0;
	
	//uart_number(speed_update_acc_one);
	//uart_str("				");
	if(cbuf_one_dif[one_bpos] !=0){
	uart_number(cbuf_one_dif[one_bpos]);
	uart_str("				");
	}
	//if(speed_flag_one)
		//uart_str("Rot 1 Warn");
	//else if((speed_flag_buff == 1) && (!speed_flag_one))
		//uart_str("Rot 1 Clear");
		
	speed_counter_one++;
	speed_flag_buff = speed_flag_one;
	
}

void rtr_intrpt()
{	
	if(!sel){
		static uint8_t buff_one = 0;		// Monitors if position has changed = easier to read information
		rtr_pos_one = rtr_qpos(sel);
		if(buff_one != rtr_pos_one){
			//uart_str("\nRotary ");
			//uart_number(sel);
			//uart_str(" Pos :");
			//uart_number(rtr_pos_one);
		}
		buff_one = rtr_pos_one;
	}else{
		static uint8_t buff_two = 0;
		rtr_pos_two = rtr_qpos(sel);
		if(buff_two != rtr_pos_two){
			//uart_str("\nRotary ");
			//uart_number(sel);
			//uart_str(" Pos :");
			//uart_number(rtr_pos_two);
		}
		buff_two = rtr_pos_two;
	}
	//uart_str("Checked encoder pos\n");
	rtr_buff_update();
		if(cbuf_one_dif[one_bpos] !=0){
			uart_number(cbuf_one_dif[one_bpos]);
			uart_str("\n");
		}
	//rtr_speed_update();
	sel = !sel;
}






