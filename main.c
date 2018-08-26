/*
 * FlowerWatering.c
 *
 * Created: 15.08.2018 18:36:57
 * Author : Michal Trojnarski, Maria Mierzwa
 */ 

//#define F_CPU 4800000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define LEDY PB4
#define LEDG PB0
#define BUTTON PB1
#define ADC_PORT 1

//void initADC();
uint16_t readFromADC(uint8_t adc_num);
void blinkLed(uint8_t times);

volatile uint16_t readedValue;

int main(void)
{
	// define interrupts
	DDRB &= ~(1 << BUTTON); //set as input
	PORTB |= (1 << BUTTON); //pull-up resistor on
	MCUCR &= ~(1 << ISC00);
	MCUCR |= (1 << ISC01); //falling edge of INT0 (PB1) generates interrupt
	GIMSK |= (1 << INT0);
	sei(); //enable global interrupt
	
	
	DDRB |= (1 << LEDY) | (1 << LEDG);

    while (1) 
    {
		
		//readedValue = readFromADC(ADC_PORT) / 10.24;
		//uint8_t percentValue = 99 - (readedValue & 0xFF);
    }
}


void blinkLed(uint8_t times)
{
	for (int j = 0; j < times; j++)
	{
		PORTB |= (1 << LEDG);
		_delay_ms(500);
		PORTB &= ~(1 << LEDG);
		_delay_ms(500);
	}
}

uint16_t readFromADC(uint8_t adc_num)
{
	uint16_t readed_value;

	//ADMUX = adc_num;
	ADMUX |= (1 << MUX0);					// use AVCC as the reference
	ADMUX &= ~(1 << ADLAR);					// right-adjusted
	ADCSRA |= (1 << ADPS1) | (1 << ADPS0);	// 128 prescale for 8MHz
	ADCSRA |= (1 << ADEN);					// enable ADC

	ADCSRA |= (1 << ADSC);					// starting conversion
	while (ADCSRA & (1 << ADSC)) {}			// wait until conversion is over

	readed_value = ADCL;
	readed_value += (ADCH << 8);

	return readed_value;
}

ISR (INT0_vect)
{
	//initADC();
	readedValue = readFromADC(ADC_PORT);// / 10.24;
	//uint8_t percentValue = 99 - ((readedValue >> 2) & 0xFF);
	
	PORTB &= ~(1 << LEDY);
	PORTB &= ~(1 << LEDG);
	
	blinkLed(readedValue / 100);
	PORTB |= (1 << LEDY);
	_delay_ms(1000);
	PORTB &= ~(1 << LEDY);
	_delay_ms(1000);
	blinkLed((readedValue % 100) / 10);
	PORTB |= (1 << LEDY);
	_delay_ms(1000);
	PORTB &= ~(1 << LEDY);
	_delay_ms(1000);
	blinkLed((readedValue % 10));
}