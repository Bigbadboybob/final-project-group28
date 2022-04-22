#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>

#define F_CPU 16000000UL
#define BAUD_RATE 9600
#define BAUD_PRESCALER ((F_CPU/ (BAUD_RATE*16UL))-1)

void uart_init();
void uart_send(unsigned char data);
void uart_putstring(char* StringPtr);