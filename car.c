/*
 * snowball.c
 *
 * Created: 4/8/2022 9:05:14 AM
 * Author : bigba
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "uart.h"


void serial_init() {
	UBRR1 = 0;
	
	//XCK1_DDR |= (1<<XCK1);
	
	UCSR1C |= (1<<UMSEL11);
	UCSR1C |= (1<<UMSEL10); //| (0<<UCPHA1) | (0<<UCP0L1);
	
	UCSR1B = (1<<RXEN1) | (1<<TXEN1);
	
	//set baud rate
	UBRR1H = (unsigned char)(BAUD_PRESCALER>>8);
	UBRR1L = (unsigned char)BAUD_PRESCALER;
}

char data[17];

unsigned char serial_get() {
	while( !(UCSR1A & (1<<UDRE1)) );
	UDR1 = data;
	
	while (!(UCSR1A) & (1<<RXC1)) ;
	
	return UDR1;
}


void init() {
	cli();
	uart_init();
	
	//motor 1
	DDRB |= (1<<DDB5);
	DDRB |= (1<<DDB6);
	
	//motor 2
	DDRB |= (1<<DDB4);
	DDRH |= (1<<DDH6);
	
	//motor 3
	DDRE |= (1<<DDE3);
	DDRE |= (1<<DDE4);
	
	//motor 4
	DDRH |= (1<<DDH3);
	DDRH |= (1<<DDH4);
	
		
	//Timer 1 internal div by 64 = 250Khz
	TCCR1B &= ~(1<<CS10);
	TCCR1B &= ~(1<<CS11);
	TCCR1B |= (1<<CS12);
	
	//Set Timer 1 to PWM Phase Correct
	TCCR1A |= (1<<WGM10);
	TCCR1A &= ~(1<<WGM11);
	TCCR1B &= ~(1<<WGM12);
	
	//Set OCR1A
	TCCR1A &= ~(1<<COM1A0);
	TCCR1A |= (1<<COM1A1);
	
	//Set OCR1B
	TCCR1A &= ~(1<<COM1B0);
	TCCR1A |= (1<<COM1B1);
	
	OCR1A = 0; //12
	OCR1B = 0; //11
	
	//Timer 2 internal div by 64 = 250Khz
	TCCR2B &= ~(1<<CS20);
	TCCR2B &= ~(1<<CS21);
	TCCR2B |= (1<<CS22);
	
	//Set Timer 2 to PWM Phase Correct
	TCCR2A |= (1<<WGM20);
	TCCR2A &= ~(1<<WGM21);
	TCCR2B &= ~(1<<WGM22);
	
	//Set OCR2A
	TCCR2A &= ~(1<<COM2A0);
	TCCR2A |= (1<<COM2A1);
	
	//Set OCR2B
	TCCR2A &= ~(1<<COM2B0);
	TCCR2A |= (1<<COM2B1);
	
	OCR2A = 0; //10
	OCR2B = 0; //9
	
	//Timer 3 internal div by 64 = 250Khz
	TCCR3B &= ~(1<<CS30);
	TCCR3B &= ~(1<<CS31);
	TCCR3B |= (1<<CS32);
	
	//Set Timer 3 to PWM Phase Correct
	TCCR3A |= (1<<WGM30);
	TCCR3A &= ~(1<<WGM31);
	TCCR3B &= ~(1<<WGM32);
	
	//Set OCR3A
	TCCR3A &= ~(1<<COM3A0);
	TCCR3A |= (1<<COM3A1);
	
	//Set OCR3B
	TCCR3A &= ~(1<<COM3B0);
	TCCR3A |= (1<<COM3B1);
	
	OCR3A = 0; //5
	OCR3B = 0; //2
	
	//Timer 4 internal div by 64 = 250Khz
	TCCR4B &= ~(1<<CS40);
	TCCR4B &= ~(1<<CS41);
	TCCR4B |= (1<<CS42);
	
	//Set Timer 4 to PWM Phase Correct
	TCCR4A |= (1<<WGM40);
	TCCR4A &= ~(1<<WGM41);
	TCCR4B &= ~(1<<WGM42);
	
	//Set OCR4A
	TCCR4A &= ~(1<<COM4A0);
	TCCR4A |= (1<<COM4A1);
	
	//Set OCR4B
	TCCR4A &= ~(1<<COM4B0);
	TCCR4A |= (1<<COM4B1);
	
	OCR4A = 0; //6
	OCR4B = 0; //7
	
	sei();
	
}

ISR(ADC_vect) {
}

void setMotor1(int s) {
	s = -s;
	if (s > 0) {
		OCR1B= 0;
		OCR1A = s;
	} else {
		OCR1A = 0;
		OCR1B = -s;
	}
}

void setMotor2(int s) {
	s = -s;
	if (s > 0) {
		OCR2B= 0;
		OCR2A = s;
		} else {
		OCR2A = 0;
		OCR2B = -s;
	}
}

void setMotor3(int s) {
	if (s > 0) {
		OCR3B= 0;
		OCR3A = s;
		} else {
		OCR3A = 0;
		OCR3B = -s;
	}
}

void setMotor4(int s) {
	if (s > 0) {
		OCR4B= 0;
		OCR4A = s;
		} else {
		OCR4A = 0;
		OCR4B = -s;
	}
}

void testMotors() {
	setMotor1(255);
	_delay_ms(500);
	setMotor1(-255);
	_delay_ms(500);
	setMotor1(100);
	_delay_ms(500);
	setMotor1(255);
	_delay_ms(1000);
	
	setMotor2(255);
	_delay_ms(500);
	setMotor2(-255);
	_delay_ms(500);
	setMotor2(100);
	_delay_ms(500);
	setMotor2(255);
	_delay_ms(1000);
	
	setMotor3(255);
	_delay_ms(500);
	setMotor3(-255);
	_delay_ms(500);
	setMotor3(100);
	_delay_ms(500);
	setMotor3(255);
	_delay_ms(1000);
	
	setMotor4(255);
	_delay_ms(500);
	setMotor4(-255);
	_delay_ms(500);
	setMotor4(100);
	_delay_ms(500);
	setMotor4(255);
	_delay_ms(1000);
}

void forward(int s) {
	setMotor1(s);
	setMotor2(s);
	setMotor3(s);
	setMotor4(s);
}

void back(s) {
	setMotor1(-s);
	setMotor2(-s);
	setMotor3(-s);
	setMotor4(-s);
}


void left(int s) {
	setMotor1(-s);
	setMotor2(s);
	setMotor3(s);
	setMotor4(-s);
}

void right(int s) {
	setMotor1(s);
	setMotor2(-s);
	setMotor3(-s);
	setMotor4(s);
}

void fl(int s) {
	setMotor1(0);
	setMotor2(s);
	setMotor3(s);
	setMotor4(0);
}

void fr(int s) {
	setMotor1(s);
	setMotor2(0);
	setMotor3(0);
	setMotor4(s);
}

void bl(int s) {
	setMotor1(-s);
	setMotor2(0);
	setMotor3(0);
	setMotor4(-s);
}

void br(int s) {
	setMotor1(0);
	setMotor2(-s);
	setMotor3(-s);
	setMotor4(0);
}

void rotateR(int s) {
	setMotor1(-s);
	setMotor2(s);
	setMotor3(-s);
	setMotor4(s);
}

void rotateL(int s) {
	setMotor1(s);
	setMotor2(-s);
	setMotor3(s);
	setMotor4(-s);
}

void stop() {
	setMotor1(0);
	setMotor2(0);
	setMotor3(0);
	setMotor4(0);
}

void demo() {
	_delay_ms(5000);
	forward(255);
	_delay_ms(1000);
	bl(255);
	_delay_ms(500);
	back(255);
	_delay_ms(500);
	br(255);
	_delay_ms(500);
	right(255);
	_delay_ms(500);
	fr(255);
	forward(255);
	_delay_ms(500);
	fl(255);
	_delay_ms(500);
	left(255);
	_delay_ms(500);
	stop();
	_delay_ms(3000);
	back(100);
	_delay_ms(1000);
	rotateR(200);
	_delay_ms(3000);
	rotateL(200);
	_delay_ms(3000);
	stop();
	_delay_ms(10000);
}

void handleInput(int x, int y, int m) {
	if (m == 1) {
		if (x == 2 && y == 1) {
			right(200);
		} else if (x==2 && y ==2) {
			fr(100);
		} else if (x == 1 && y==2) {
			forward(100);
		} else if (x == 0 && y==2) {
			fl(100);
		} else if (x==0 && y ==1) {
			left(100);
		} else if (x==0 && y==0) {
			bl(100);
		} else if (x==1 && y ==0) {
			back(100);
		} else if (x == 2 && y==0) {
			br(100);
		}
	} else if (m == 2) {
		if (x == 2) {
			rotateR(100);
		} else if (x==0) {
			rotateL(100);
		}
	}
}

int main(void)
{
    init();
	//uart_send(60);
	//uart_send(10);
	//uart_putstring("test\n");
	demo();
    while (1)
    {
		demo();
    }
}

