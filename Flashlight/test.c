
 #define F_CPU 1600000UL

#include <avr/io.h>
#include <util/delay.h>

void init() {
	DDRB |= 1<<PB0; // PB0 as output
	DDRB &= ~(1<<PB2); // PB2 as input
	PORTB |= 1<<PB2; // enable internal pull-up resistor
}

int main(void)
{
	init();
    while (1) 
    {
		if (PINB & (1<<PINB2)) {
			PORTB ^= 1;	
			while (PINB & (1<<PINB2));
		}
    }
}
