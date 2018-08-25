/*
 * FlowerWatering.c
 *
 * Created: 15.08.2018 18:36:57
 * Author : Michal Trojnarski, Maria Mierzwa
 */ 

//#define F_CPU 4800000

#include <avr/io.h>
#include <util/delay.h>

#define LEDY PB4
#define LEDG PB1
#define ADC_PORT 1

//void initADC();
uint16_t readFromADC(uint8_t adc_num);
void blinkLed(uint8_t times);

int main(void)
{
	_delay_ms(5000);

	
	DDRB |= (1 << LEDY) | (1 << LEDG);
	uint16_t readedValue = 0;
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

    while (1) 
    {
		//readedValue = readFromADC(ADC_PORT) / 10.24;
		//uint8_t percentValue = 99 - (readedValue & 0xFF);
	//
		//PORTB &= ~(1 << LEDY);
		//PORTB &= ~(1 << LEDG);
//
		//if (percentValue < 30)
		//{
			//PORTB |= (1 << LEDY) | (1 << LEDG);
		//}
		//else if (percentValue < 90)
		//{
			//PORTB |= (1 << LEDY);
		//}
		//else
		//{
			//PORTB |= (1 << LEDG);
		//}
    }
}

// void initADC()
// {
// 	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);	// 128 prescale for 8MHz
// 	ADCSRA |= (1 << ADEN);	// enable ADC
// }

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