/*
 * uart.h
 *
 * Created: 11/11/2014 22:10:00
 *  Author: Reece
 */ 

#ifndef UART_H
#define UART_H

#include <avr/io.h>

void uart_init(void);

char get_ch(void);

void uart_tx(char ch);
void uart_data(uint8_t d);

void uart_str(char *str);

void uart_number(uint8_t x);


#endif