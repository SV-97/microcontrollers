;
; 004_Pendellicht.asm
;
; Created: 11.12.2018 10:00:41
; Author : volzs
;

main:		ldi r16,    low(RAMEND)
			out SPL,    r16				;set stackpointer lower byte

			ldi r16,    high(RAMEND)
			out SPH,    r16				;set stackpointer higher byte
			
			ldi r16,	0xFF
			out DDRB,	r16				;define as output

			ldi r16, 0x00
			out portb, r16

			ldi r16, 0xFF

mainloop:	wdr
			rol r16
			out portb, r16
			rjmp left
			rjmp mainloop

left:		rol r16
			out portb, r16
			rcall wait_2
			BRCC right
			BRCS left

right:		ror r16
			out portb, r16
			rcall wait_2
			BRCC mainloop
			BRCS right

wait_2:		inc r20
			brne wait_2
			inc r21
			brne wait_2
			ret