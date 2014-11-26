/*
 * softpot.c
 *
 * Created: 11/11/2014 21:14:57
 *  Author: Reece
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "softpot.h"
#include "timer.h"

volatile const uint8_t wheel =  (0<<MUX4) | (0<<MUX3) | (0<<MUX2) | (0<<MUX1) | (0<<MUX0);			//ADC0
volatile const uint8_t throttle = (0<<MUX4) | (0<<MUX3) | (0<<MUX2) | (0<<MUX1) | (1<<MUX0);		//ADC1
volatile const uint8_t tposition = (0<<MUX4) | (0<<MUX3) | (0<<MUX2) | (1<<MUX1) | (0<<MUX0);		//ADC2

uint16_t wheel_cal = 1;
uint16_t throttle_cal = 1;
uint16_t tposition_cal = 1;



int cal = 0;


void adc_init()
{
	ADCSRA = _BV(ADEN) | _BV(ADIE) | _BV(ADPS2) | _BV(ADPS1);	//Enable ADC with prescaler of 64 & interrupt
	ADMUX = wheel;												//AVCC, initializes on ADC0
}

void adc_conv()
{
	ADCSRA |= _BV(ADSC);
}

uint16_t adc_value()
{
	uint16_t a;
	while(!ADSC && ADIF);		//Wait for conversion to complete
	a = (ADCL | (ADCH <<8));		
	return a;
}

void adc_cal()		// Initializes system on power up
{
	uint16_t buff = 1;
	adc_conv();
	buff = adc_value();		//Should hold here until conversion is complete
	wheel_cal = buff;
	ADMUX = throttle;
	
	adc_conv();
	throttle_cal = adc_value();
	ADMUX = wheel;
}

ISR(ADC_vect)
{
	
	
		uint16_t result = ADCL | (ADCH <<8);		// Take ADC result
	
		if(ADMUX == wheel)
		{
			if(cal == 0 | cal == 1)
			{
				wheel_cal = result;
				cal ++;
			}
			else
			{
				if(result > (wheel_cal + 25))		//25 is ballpark value, don't want false detection from noise
					reset_wheel_flag();
				else
					set_wheel_flag();
			}
			ADMUX = throttle;
			adc_conv();
		}
		
		else if(ADMUX == throttle)
		{
			if(!(cal == 0 | cal == 1))
			{
				if(result > (throttle_cal + 25))
					reset_throttle_flag();
				else
					set_throttle_flag();	

			}
			else 
			{
				throttle_cal = result;
					cal ++;
			}
			ADMUX = wheel;	
			adc_conv();
		}
		else
			PINB |= _BV(PB7);

}

void calibrate()
{
	cal = 0;
}