/*
 * C_005_ManualPWM.cpp
 *
 * Created: 22.12.2018 01:59:07
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
		for (uint8_t i=0; i<10; i++)
		{
			readout_0 = fetch_analog();	
		}
		ADMUX |= (1<<MUX0); // ADC Channel 1
		readout_1 = fetch_analog();
		ADMUX &= ~(1<<MUX0)|(1<<MUX1)|(1<<MUX2); // ADC Channel 0
		ADMUX |= (1<<MUX1); // ADC Channel 2
		readout_2 = fetch_analog();
		PORTD = 0xFF;
		for (uint8_t i=0; i < readout_0; i++)
		{
			_delay_us(1);
		}
		PORTD = 0;
		for (uint8_t i=0; i< 1024-readout_0; i++)
		{
			_delay_us(1);
		}
	}
}

uint8_t fetch_analog()
{
	ADCSRA |= (1<<ADSC); // Start Analog readout
	while (ADCSRA &(1<<ADSC)); // Wait until readout complete
	return ADCH;
}