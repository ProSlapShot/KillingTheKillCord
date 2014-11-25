/*
 * uart.h
 *
 * Created: 11/11/2014 22:10:00
 *  Author: Reece
 */ 


#ifndef UART_H_
#define UART_H_

#include <avr/io.h>

void uart_init();

void uart_tx(uint8_t c);



#endif /* UART_H_ */