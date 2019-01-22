.ORG 0x0000
rjmp main
nop
nop
nop
;.ORG 0x0004 Alternativ rjmp des Interrupts über nops auf korrekte Adresse setzen
rjmp interrupt1

main: 
			ldi		r16, high(RAMEND)
			out		SPH, r16
			ldi		r16, low(RAMEND)
			out		SPL, r16
			
			clr		r16
			out		ddrd, r16			;ddrd as input
			ser		r16
			out		ddrb, r16			;ddrb as output
			out		portb, r16
			
			ldi		r16, 0b10000000
			out		gicr, r16
			ldi		r16, 0b00001000
			out		mcucr, r16

			sei

mainloop:	wdr
			cbi		portb, 7
			rcall	delay
			sbi		portb, 7
			rcall	delay
			rjmp	mainloop

delay:		ldi r20, 0
			ldi r21, 0
del1:		inc		r20
			brne	del1
			inc		r21
			brne	del1
			ret

interrupt1:	in		r17, portb
			sbrc	r17, 3
			cbi		portb, 3
			sbrs	r17, 3
			sbi		portb, 3
			reti