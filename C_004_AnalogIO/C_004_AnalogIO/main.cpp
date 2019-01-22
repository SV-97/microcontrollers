/*
 * C_004_AnalogIO.cpp
 *
 * Created: 22.12.2018 00:10:01
 * Author : volzs
 */ 

#include <util/delay.h>
#include <avr/io.h>

int main(void)
{
	uint16_t x;
	uint8_t readout_0, readout_1, readout_2;
	uint8_t fetch_analog();
	
	// DO init
	DDRD = 0xFF;
	
	// ADC init
	ADCSRA |= (1<<ADEN); // ADC on
	ADCSRA |= (1<<ADPS0)|(1<<ADPS1); // Prescaler to 8
	ADMUX &= ~(1<<MUX0)|(1<<MUX1)|(1<<MUX2); // ADC Channel 0
	ADMUX |= (1<<ADLAR); // Left adjust the result
	DIDR0 |= (1<<ADC0D); // disable DI on Channel 0
	
	// Dummy Readout
	x = fetch_analog();
	
	while (1)
	{
		
		ADMUX &= ~((1<<MUX0)|(1<<MUX1)|(1<<MUX2)); // ADC Channel 0
		for (uint8_t i = 0; i <= 2000; i++)
		{
			readout_0 = fetch_analog();
			PORTD = readout_0;
			_delay_ms(100);
		}
		
		ADMUX &= ~((1<<MUX0)|(1<<MUX1)|(1<<MUX2)); // ADC Channel 0
		ADMUX |= (1<<MUX0); // ADC Channel 1
		for (uint8_t i = 0; i <= 200; i++)
		{
			readout_1 = fetch_analog();
			PORTD = readout_1;
			_delay_ms(100);
		}
		
		ADMUX &= ~(1<<MUX0)|(1<<MUX1)|(1<<MUX2); // ADC Channel 0
		ADMUX |= (1<<MUX1); // ADC Channel 2
		for (uint8_t i = 0; i <= 200; i++)
		{
			readout_2 = fetch_analog();
			PORTD = readout_2;
			_delay_ms(100);
		}
		
	}
}

uint8_t fetch_analog() 
{
	ADCSRA |= (1<<ADSC); // Start Analog readout
	while (ADCSRA &(1<<ADSC)); // Wait until readout complete
	return ADCH;
}