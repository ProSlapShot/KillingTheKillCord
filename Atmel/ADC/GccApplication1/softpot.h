/*
 * softpot.h
 *
 * Created: 11/11/2014 21:15:50
 *  Author: Reece
 */ 


#ifndef SOFTPOT_H_
#define SOFTPOT_H_

#include <avr/io.h>

void adc_init();

void adc_conv();

uint16_t adc_value();

void adc_cal();

void calibrate();

#endif /* SOFTPOT_H_ */