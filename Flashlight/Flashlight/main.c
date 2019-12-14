/*
 * Flashlight.c
 *
 * Created: 10.12.2019 19:49:50
 * Author : volzs
 */
 #define F_CPU 1000000UL // 1.6MHz internal clock

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void init() {
	// Digital I/O setup
	DDRB |= 1<<PB0; // PB0 as output
	DDRB &= ~(1<<PB2); // PB2 as input
	PORTB |= 1<<PB2; // Enable internal pull-up resistor
	
	DIDR0 |= (1<<AIN1D)|(1<<AIN0D); // Disable digital input buffer on AIN1/0 for power reduction
	
	// PWM setup
	TCCR0B |= (1<<CS00); // No prescaler
	TCCR0A |= (1<<COM0A1)|(1<<WGM00)|(1<<WGM01); // Fast PWM on OC0A clear on match
	OCR0A = 0; // Set PWM duty cycle to 0 initially
	
	// Interrupt setup
	MCUCR |= (1<<ISC01); // Rising edge of INT0 generates interrupt
	GIMSK |= (1<<INT0); // Enable INT0
	
	// Sleep mode setup
	MCUCR |= (1<<SE); // Enable sleep mode
	MCUCR &= ~((1<<SM1)|(1<<SM0)); // Set sleep mode to Idle
	
	// Power reduction setup
	PRR |= (1<<PRTIM1)|(1<<PRADC); // disable Timer/Counter 1 and ADC
	
	// ADC setup
	ADCSRA &= ~(1<<ADEN); // disable ADC
	ACSR &= ~(1<<ACD); // disable Analog comparator
	
	// Disable watchdog timer
	asm volatile("WDR");
	MCUSR &= ~(1<<WDRF); // Clear WDRF
	WDTCR |= (1<<WDCE)|(1<<WDE);
	WDTCR = 0x00; // Turn off WDT
	
	sei(); // Enable interrups
}

int button_pressed() {
	return !(PINB & (1<<PINB2)); // inverted because of active low switch
}

int main(void)
{
	init();
    while (1) 
    {
		asm volatile ("SLEEP");
    }
}

ISR (INT0_vect) {
	_delay_ms(200);
	if (button_pressed()) {
		// soft on
		while (button_pressed()) {
			_delay_ms(20);
			if (OCR0A < 255) {
				OCR0A++;
			} else { // blink to show brightness is at max level
				OCR0A = 0;
				_delay_ms(25);
				OCR0A = 255;
			}
		}
	} else {
		// hard-on or off
		if (OCR0A == 0) { // toggle on if it's off
			OCR0A = 255;
		} else { // or toggle off if it's on
			OCR0A = 0;
		}
	}
	while (!(PINB & (1<<PINB2))); //ghetto debounce
	_delay_ms(500);
}
