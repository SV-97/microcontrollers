#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <math.h>

void tripple_fade();
double deg_to_rad(double deg);

int main(void)
{
	DDRD |= (1<<PD5)|(1<<PD6); //PORTD 5 and 6 as output
	DDRB |= (1<<PB3); //PORTB 3 as output

	
	TCCR0A = (1<<COM0A1)|(1<<COM0B1)|(1<<WGM00)|(1<<WGM01); //Fast PWM on 0A and 0B clear on match
	TCCR0B = (1<<CS00); //No Prescaling
	
	TCCR2A = (1<<COM2A1)|(1<<WGM20)|(1<<WGM21); //Fast PWM on 2A clear on match
	TCCR2B = (1<<CS20); //No Prescaling
	
    while (1) 
    {
		tripple_fade();
		for (int i=0; i<=255; i++)
		{
			OCR0A = i;
			OCR0B = 255-i;
			OCR2A = 128-i;
			_delay_ms(10);
		}
		_delay_ms(1000);
		for (int i=255; i>=0; i--)
		{
			OCR0A = i;
			OCR0B = 255-i;
			OCR2A = 128-i;
			_delay_ms(5);
		}
		_delay_ms(1000);
    }
}

void tripple_fade()
{
	while (1)
	{	
		for (double i=0; i<20*M_PI; i+=M_PI/180)
		{
			OCR0A = (int)(255 * sin(i));
			OCR0B = (int)(255 * sin(i + deg_to_rad(120.0)));
			OCR2A = (int)(255 * sin(i + deg_to_rad(240.0)));
			_delay_ms(100);
		}
	}
}

double deg_to_rad(double deg)
{
	return deg*2*M_PI/360;
}
