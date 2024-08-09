.fopt    author, "Paul Goldstein"
.setcpu  "6502"

.segment "CODE"

START:
	JSR ROUTINE ; 6
	BRK         ; 7

ROUTINE:
	LDY #20 ; 2
	TSY     ; 2
	TXA     ; 2
	TAY     ; 2

.segment "RESET"
    .word $FF00 ; NMI
    .word $8000 ; RESET
    .word $FF00 ; BRK
