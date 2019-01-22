#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	uint16_t x;
	uint8_t readout_0, readout_1, readout_2;
	uint8_t fetch_analog();
	
	//PWM init
	DDRD |= (1<<PD5)|(1<<PD6); //PORTD 5 and 6 as output
	DDRB |= (1<<PB3); //PORTB 3 as output
	
	TCCR0A = (1<<COM0A1)|(1<<COM0B1)|(1<<WGM00)|(1<<WGM01); //Fast PWM on 0A and 0B clear on match
	TCCR0B = (1<<CS00); //No Prescaling
	
	TCCR2A = (1<<COM2A1)|(1<<WGM20)|(1<<WGM21); //Fast PWM on 2A clear on match
	TCCR2B = (1<<CS20); //No Prescaling
		
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
		for (int i=0; i<20; i++)
		{
			readout_0 = fetch_analog();	
			_delay_ms(1);
		}
		
		ADMUX &= ~((1<<MUX0)|(1<<MUX1)|(1<<MUX2)); // ADC Channel 0
		ADMUX |= (1<<MUX0); // ADC Channel 1
		for (int i=0; i<20; i++)
		{
			readout_1 = fetch_analog();
			_delay_ms(1);
		}
		
		ADMUX &= ~(1<<MUX0)|(1<<MUX1)|(1<<MUX2); // ADC Channel 0
		ADMUX |= (1<<MUX1); // ADC Channel 2
		for (int i=0; i<20; i++)
		{
			readout_2 = fetch_analog();
			_delay_ms(1);
		}
		OCR0A = readout_0;
		OCR0B = readout_1;
		OCR2A = readout_2;
	}
}

uint8_t fetch_analog()
{
	ADCSRA |= (1<<ADSC); // Start Analog readout
	while (ADCSRA &(1<<ADSC)); // Wait until readout complete
	return ADCH;
}