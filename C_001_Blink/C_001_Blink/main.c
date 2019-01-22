/*
 * C_001_Blink.c
 *
 * Created: 09.12.2018 15:29:32
 * Author : volzs
 */ 

#include <util/delay.h>
#include <avr/io.h>

uint8_t x = 1, y=0;
int main(void)
{
	DDRD = 0xFF;
	_delay_us(200);
	PORTD = (1<<PD0);
	
	uint8_t right = 0;
    while (1) 
    {
		if (right)
		{
			PORTD = (PORTD>>1);
			if (PORTD == 0x01)
			{
				right = 0;
			}
		} else
		{
			PORTD = (PORTD<<1);
			if (PORTD == 0x80)
			{
				right = 1;
			}
		}
		_delay_ms(1000);
    }
}

