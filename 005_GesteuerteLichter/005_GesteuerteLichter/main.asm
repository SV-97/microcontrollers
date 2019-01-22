;
; 005_GesteuerteLichter.asm
;
; Created: 11.12.2018 11:36:46
; Author : volzs
;


; Replace with your application code
init:		ldi r16, high(ramend)
			out SPH, r16

			ldi r16, low(ramend)
			out SPL, r16

			ldi r16, 0x00
			out ddrd, r16

			ldi r16, 0xFF
			out ddrb, r16
			out portb, r16
			ldi r17, 0xFF

mainloop:	wdr
			sbis pind, 0
			rjmp left
			sbis pind, 1
			rjmp right
			sbis pind, 2
			rjmp p_left
			sbis pind, 3
			rjmp mainloop
			rjmp mainloop

left:		wdr
			rol r16
			out portb, r16
			sbis pind, 1
			rjmp right
			sbis pind, 2
			rjmp p_left
			sbis pind, 3
			rjmp mainloop
			rcall delay
			rjmp left

right:		wdr
			ror r16
			out portb, r16
			sbis pind, 0
			rjmp left
			sbis pind, 2
			rjmp p_right
			sbis pind, 3
			rjmp mainloop
			rcall delay
			rjmp right

p_left:		wdr
			sbis pind, 0
			rjmp left
			sbis pind, 1
			rjmp right
			rcall delay
			rol r16
			out portb, r16
			sbis pind, 3
			rjmp mainloop
			BRCC p_right
			BRCS p_left

p_right:	wdr
			sbis pind, 0
			rjmp left
			sbis pind, 1
			rjmp right
			rcall delay
			ror r16
			out portb, r16
			sbis pind, 3
			rjmp mainloop
			BRCC p_left
			BRCS p_right

delay:		ldi  r20, 0x09
WGLOOP0:	ldi  r21, 0xBC
WGLOOP1:	ldi  r22, 0x12
WGLOOP2:	dec  r22
			brne WGLOOP2
			dec  r21
			brne WGLOOP1
			dec  r20
			brne WGLOOP0
			ret
