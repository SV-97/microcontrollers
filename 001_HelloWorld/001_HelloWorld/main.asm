;
; 001_HelloWorld.asm
;
; Created: 20.11.2018 12:27:12
; Author : volzs
;


; Replace with your application code
begin:		rjmp main	;1 POWER ON RESET

main:		ldi r16, low(RAMEND)
			out SPL, r16
			ldi r16, high(RAMEND)
			out SPH, r16

mainloop:	wdr
			;ldi r16, 0xFF
			;out DDRB, r16
			;ldi r16, 0b00000001
			;com r16
			;out portb, r16
			sbi DDRB, 0
			cbi portB, 0
			rjmp mainloop

