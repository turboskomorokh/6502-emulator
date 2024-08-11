; Paul Goldstein
; 1723251405
; It counts to 10!

.setcpu  "6502"

.segment "CODE"

START:
	JSR INX10 ;
	STA $2000 ; Store result
	LDA #0
	BRK       ; Finish

INX10:
	CLC
	ADC #10   ;
	RTS
.segment "RESETVEC"
    .word $FF00 ; NMI vector location
    .word $8000 ; Reset vector location
    .word $FF00 ; BRK vector location
