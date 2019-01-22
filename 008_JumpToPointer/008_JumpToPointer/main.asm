; 7_seg segement-mapping
.equ a = 0
.equ b = 1
.equ c = 2
.equ d = 3
.equ e = 4
.equ f = 5
.equ g = 6
.equ DP = 7

.org 0x0000
		rjmp main
.org OVF0addr
		rjmp timer0_overflow

main:
		ldi r17, HIGH(RAMEND)
		out SPH, r17
		ldi r17, LOW(RAMEND)
		out SPL, r17

		ldi r17, (1<<CS02)|(1<<CS00)
		out TCCR0, r17

		ldi r17, (1<<TOIE0)
		out TIMSK, r17

		ldi r17, 0xFF
		out DDRD, r17

		ldi r16, 0
		ldi r20, 255
		rcall seg_0
		sei

loop:	wdr
		rjmp loop

timer0_overflow:
		cpi r20, 2
		brne inc_r20
		rcall seven_seg
		com r18
		out PORTD, r18
		inc r16
		ldi r20, 255
		reti
inc_r20:
		inc r20
		reti

seven_seg:
		ldi r30, LOW(disp_seg_0)
		ldi r31, HIGH(disp_seg_0)
		add r30, r16
		ijmp

disp_seg_0: rjmp seg_0
disp_seg_1: rjmp seg_1
disp_seg_2: rjmp seg_2
disp_seg_3: rjmp seg_3
disp_seg_4: rjmp seg_4
disp_seg_5: rjmp seg_5
disp_seg_6: rjmp seg_6
disp_seg_7: rjmp seg_7
disp_seg_8: rjmp seg_8
disp_seg_9: rjmp seg_9
disp_seg_A: rjmp seg_A
disp_seg_B: rjmp seg_B
disp_seg_C: rjmp seg_C
disp_seg_D: rjmp seg_D
disp_seg_E: rjmp seg_E
disp_seg_F: rjmp seg_F

seg_0:
		ldi r18, (0xFF)&~(1<<g)
		ret
seg_1:
		ldi r18, (1<<b)|(1<<c)|(1<<DP)
		ret
seg_2:
		ldi r18, (1<<a)|(1<<b)|(1<<g)|(1<<d)|(1<<e)|(1<<DP)
		ret
seg_3:
		ldi r18, (1<<a)|(1<<b)|(1<<c)|(1<<d)|(1<<g)|(1<<DP)
		ret
seg_4:
		ldi r18, (1<<b)|(1<<c)|(1<<f)|(1<<g)|(1<<DP)
		ret
seg_5:
		ldi r18, (1<<a)|(1<<f)|(1<<g)|(1<<c)|(1<<d)|(1<<DP)
		ret
seg_6:
		ldi r18, (1<<a)|(1<<c)|(1<<d)|(1<<e)|(1<<f)|(1<<g)|(1<<DP)
		ret
seg_7:
		ldi r18, (1<<a)|(1<<b)|(1<<c)|(1<<DP)
		ret
seg_8:
		ldi r18, 0xFF
		ret
seg_9:
		ldi r18, (0xFF)&~(1<<e)
		ret
seg_A:
		ldi r18, (0xff)&~((1<<DP)|(1<<d))
		ret
seg_B:
		ldi r18, (0xFF)&~((1<<a)|(1<<b)|(1<<DP))
		ret
seg_C:
		ldi r18, (0xFF)&~((1<<b)|(1<<c)|(1<<g)|(1<<DP))
		ret
seg_D:
		ldi r18, (0xFF)&~((1<<a)|(1<<f)|(1<<DP))
		ret
seg_E:
		ldi r18, (0xFF)&~((1<<b)|(1<<c)|(1<<DP))
		ret
seg_F:
		ldi r18, (0xFF)&~((1<<b)|(1<<c)|(1<<d)|(1<<DP))
		ldi r16, 0xFF
		ret