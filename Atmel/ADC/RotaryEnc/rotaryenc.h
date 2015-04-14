/*
 * rotaryenc.h
 *
 * Created: 11/02/2015 07:44:24
 *  Author: Reece
 */ 


#ifndef ROTARYENC_H_
#define ROTARYENC_H_


void rot_init();

uint8_t rtr_value();

uint8_t rtr_position(uint8_t x);

void rtr_intrpt();

void rtr_buff_update();

void rtr_speed();




#endif /* ROTARYENC_H_ */