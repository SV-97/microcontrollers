;
; STK500_Test.asm
;
; Created: 20.11.2018 11:01:06
; Author : volzs
;


; Replace with your application code
start:
		ldi r16, 0xFF
		out DDRB, r16

		ldi r16, 0x00
		out DDRD, r16

loop:
		in r16, PIND
		out PORTB, r16
		rjmp loop
