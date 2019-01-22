/*
 * C_003_OverflowTest.cpp
 *
 * Created: 21.12.2018 20:51:28
 * Author : volzs
 */ 

#include <util/delay.h>
#include <avr/io.h>

int main(void)
{
	DDRD = 0xFF;
	PORTD = 0;
    while (1) 
    {
		PORTD++;
		_delay_ms(200);
    }
}

