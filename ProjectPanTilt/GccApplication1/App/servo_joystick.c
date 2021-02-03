
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include "..\lib\uart.h"
#define BAUD_RATE 9600
#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)
char str_print[20];

void ADC_Init(){
	cli();
	//Power Reduction ADC on PRR bit 0
	PRR &= ~(1<<PRADC);
	//Reference Voltage to AVcc with external capacitor at AREF pin at AMUX (01)
	ADMUX |=(1<<REFS0);
	ADMUX &= ~(1<<REFS1);
	//ADC frequency pre-scaler, target between 50-200 KHz, pre-scaler of 128
	ADCSRA |=(1<<ADPS2);
	ADCSRA |=(1<<ADPS1);
	ADCSRA |=(1<<ADPS0);
	//set up Channel to read, use channel 0, ADMUX reg
	ADMUX &= ~(1<<MUX0);
	ADMUX &= ~(1<<MUX1);
	ADMUX &= ~(1<<MUX2);
	ADMUX &= ~(1<<MUX3);
	//set up trigger mode, auto trigger bit5 ADATE at ADCSRA
	ADCSRA|= (1<<ADATE);
	//set up free running 000 at ADCSRB
	ADCSRB &= ~(1<<ADTS0);
	ADCSRB &= ~(1<<ADTS0);
	ADCSRB &= ~(1<<ADTS0);
	//disable input buffer, ADC0D at DIDR0
	DIDR0 |= (1<<ADC0D);
	//Enable ADC, bit 7 ADEN:ADC Enable at ADCSRA
	ADCSRA |= (1<<ADEN);
	//Start Conversion, Bit 6 ADSC:ADC Start Conversion at ADCSRA
	ADCSRA |= (1<<ADSC);
	sei();
}
void PWM_Init(){
	cli();//disable all interrupts
	//PD5 as output
	DDRD |= (1<<DDD5);
	
	//Configure timer mode to Phase Correct PWM (101)
	//(101) TOP vale is OCRA instead of set value of 0XFF
	TCCR0A |= (1<<WGM00);
	TCCR0A &= ~(1<<WGM01);
	TCCR0B |= (1<<WGM02);
	
	//Configure timer pre-scaler (100) /256
	TCCR0B &= ~(1<<CS00);
	TCCR0B &= ~(1<<CS01);
	TCCR0B |= (1<<CS02);

	//Configure Output Compare Register A/B
	OCR0A=80;
	OCR0B=OCR0A/2;
	//Non-inverting mode
	//Clear on compare match
	TCCR0A |= (1<<COM0B1);
	sei(); //Enable global interrupts
	
}


//servo1 is on PB1
void deg_to_servo1 (int deg){
	OCR1A=(deg*2.7778)+125;
}
//servo2 is on PB2
void deg_to_servo2 (int deg){
	OCR1B=(deg*2.7778)+125;
}
void initialize_pwm_2servos (){
	cli();
	DDRB |= (1<<DDB1)|(1<<DDB2);	/* Make OC1A pin (~D9) OC1B (~D10) as output */
	TCNT1 = 0;		/* Set timer1 count zero */
	ICR1 = 4999;		/* Set TOP count for timer1 in ICR1 register */
	/* Set Fast PWM, TOP in ICR1, Clear OC1A on compare match, clk/64 */
	//TCCR1A = (1<<WGM11)|(1<<COM1A1);
	//TCCR1B = (1<<WGM12)|(1<<WGM13)|(1<<CS10)|(1<<CS11);
	//clear OC1A/OC1B on compare match
	//for servo1
	TCCR1A |= (1<<COM1A1);
	TCCR1A &= ~(1<<COM1A0);
	//for servo2
	TCCR1A |= (1<<COM1B1);
	TCCR1A &= ~(1<<COM1B0);
	//Fast PWM Mode
	TCCR1A &= ~(1<<WGM10);
	TCCR1A |= (1<<WGM11);
	TCCR1B |= (1<<WGM12);
	TCCR1B |= (1<<WGM13);
	//Configure timer pre-scaler (011) /64
	TCCR1B |= (1<<CS10);
	TCCR1B |= (1<<CS11);
	TCCR1B &= ~(1<<CS12);
	sei();
}
void initialize_pwm_joyservo (){
	cli();//disable all interrupts
	DDRD |= (1<<DDD5);	/* Make OC1A pin (~D9) OC1B (~D10) as output */
	//Use Timer 0 for joystick to servo
	//prescale of 1024 (101)
	TCCR0B |= (1<<CS02);
	TCCR0B &= ~(1<<CS01);
	TCCR0B |= (1<<CS00);
	
	//Fast PWM mode
	TCCR0A |= (1<<WGM00);
	TCCR0A |= (1<<WGM01);
	TCCR0B |= (1<<WGM02);
	
	//OCR0A sets frequency
	//OCR0B sets duty cycle
	OCR0A=255;
	OCR0B=OCR0A/2;
	
	//Non-inverting mode
	//Clear on compare match
	TCCR0A |= (1<<COM1B1);
	sei(); //Enable global interrupts
	
}