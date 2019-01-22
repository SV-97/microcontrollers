;
; 003_Lauflicht.asm
;
; Created: 04.12.2018 10:27:24
; Author : volzs
;


; Replace with your application code

main:		ldi r16,    low(RAMEND)
			out SPL,    r16				;set stackpointer lower byte

			ldi r16,    high(RAMEND)
			out SPH,    r16				;set stackpointer higher byte
			
			ldi r16,	0xFF
			out DDRB,	r16				;define as output
			ldi r16, 0xFF
			ldi r17, 0xFF

mainloop:	wdr

			rol r16
			out portb, r16
			rcall wait_2
			rjmp mainloop

wait_2:		inc r20
			brne wait_2
			inc r21
			brne wait_2
			ret