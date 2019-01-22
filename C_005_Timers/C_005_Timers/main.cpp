/*
 * C_005_Timers.cpp
 *
 * Created: 26.12.2018 15:00:46
 * Author : volzs
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

uint16_t timer_overflow_counter_0 = 0;

void init_timer_0()
{
	TCCR0A = 0x00;
	TCCR0B = (1<<CS00); // Prescale with 1
	TIMSK0 = (1<<TOIE0);	// Timer overflow interrupt enable
	TCNT0 = 0;	// Preload timer with 0
}

int main(void)
{
	DDRD = 0xFF; // PORTD as output
	PORTD |= (1<<PD0);
	
	init_timer_0();
	sei();
	while (1)
	{
		asm("NOP");
	}
}

ISR(TIMER0_OVF_vect)
{
	if (timer_overflow_counter_0 <= 62500)
		timer_overflow_counter_0++;
	else
	{
		PORTD ^= (1<<PD1);
		timer_overflow_counter_0 = 0;
	}
}
