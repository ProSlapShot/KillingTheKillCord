/*
 * timer.h
 *
 * Created: 11/11/2014 22:08:34
 *  Author: Reece
 */ 


#ifndef TIMER_H_
#define TIMER_H_

#include <avr/io.h>

void timer_init();

void reset_wheel_flag();

void set_wheel_flag();

void reset_throttle_flag();

void set_throttle_flag();

void engine_start();

#endif /* TIMER_H_ */