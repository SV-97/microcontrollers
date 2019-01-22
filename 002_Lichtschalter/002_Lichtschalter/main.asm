;
; 002_Lichtschalter.asm
;
; Created: 28.11.2018 10:20:15
; Author : volzstefan
;

begin:		rjmp main	               ;1 POWER ON RESET

main:		ldi r16,    low(RAMEND)
			out SPL,    r16				;set stackpointer lower byte

			ldi r16,    high(RAMEND)
			out SPH,    r16				;set stackpointer higher byte
			
			ldi r16,	0x00
			out DDRD,   r16				;define as input
			
			ldi r16,	0xFF
			out DDRB,	r16				;define as output

mainloop:	wdr							;watchdog reset

			ldi r16,	0xFF			;set r16 to all "1"s
			in	r16,	PIND			;load first bit from ddrd
			sbrc r16,	0				;skip if first bit in r17 is not set (button pressed)
			cbi DDRB,	0				;clear bit => reversed on stk500
			sbrs r16,	0
			sbi DDRB,	0

			rjmp mainloop			