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

void init() {
	cli();
	
	DDRB |= (1<<DDB4);
	DDRB |= (1<<DDB5);
	DDRB |= (1<<DDB6);
	DDRB |= (1<<DDB7);
	
	PORTB |= (1<<PB4);
		
	//Timer 1 internal div by 64 = 250Khz
	TCCR1B &= ~(1<<CS10);
	TCCR1B &= ~(1<<CS11);
	TCCR1B |= (1<<CS12);
	
	//Set Timer 0 to PWM Phase Correct
	TCCR1A |= (1<<WGM10);
	TCCR1A &= ~(1<<WGM11);
	TCCR1B |= (1<<WGM12);
	
	//Set OCR0A as TOP
	TCCR1A &= ~(1<<COM1A0);
	TCCR1A |= (1<<COM1A1);
	
	TCCR1A &= ~(1<<COM1B0);
	TCCR1A |= (1<<COM1B1);
	
	OCR1A = 200;
	//OCR1B = 200;
	
	sei();
	
}

void setMotor1(int s) {
	if (s > 0) {
		OCR1B= 0;
		OCR1A = s;
	} else {
		OCR1A = 0;
		OCR1B = -s;
	}
}

int main(void)
{
    init();
    while (1)
    {
		OCR1A = 255;
		OCR1B = 0;
		_delay_ms(2000);
		OCR1A = 100;
		OCR1B = 0;
		_delay_ms(2000);
		OCR1A = 0;
		OCR1B = 255;
		_delay_ms(2000);
		OCR1A = 0;
		OCR1B = 100;
		_delay_ms(2000);
    }
}

