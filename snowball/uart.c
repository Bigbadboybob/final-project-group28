#include "uart.h"

void uart_init() {
	//set baud rate
	UBRR0H = (unsigned char)(BAUD_PRESCALER>>8);
	UBRR0L = (unsigned char)BAUD_PRESCALER;
	//enable receiver and transmiter
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	//set frame format 2 stop bits, 8 data bits
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);
	UCSR0C |- (1<<USBS0);
}

void uart_send(unsigned char data) {
	while(!(UCSR0A & (1<<UDRE0)));
	
	UDR0 = data;
}

void uart_putstring(char* StringPtr) {
	while(*StringPtr != 0x00) {
		uart_send(*StringPtr);
	StringPtr++;
	}
}

