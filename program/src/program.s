; Paul Goldstein
; 1723251405
; It counts to 10!
.setcpu  "6502"

.segment "CODE"

START:
	JSR INX10 ; 6 cycles: Jump to INX10, saves return address
	STA $2000 ; 4 cycles: Store accumulator to memory location $2000
	BRK       ; 7 cycles: Break, stops execution

INX10:
	INX          ; 2 cycles: Increment accumulator (A)
	TXA          ; 2 cycles: Transfer accumulator to X register
	CMP #10      ; 2 cycles: Compare X with 10
	BEQ INX10END ; 2 cycles (plus 1 cycle if branch taken): Branch to INX10END if X = 10
	JMP INX10    ; 3 cycles: Jump to INX10 if comparison not equal
INX10END:
	RTS ; 6 cycles: Return from subroutine

.segment "RESET"
    .word $FF00 ; NMI vector location
    .word $8000 ; Reset vector location
    .word $FF00 ; BRK vector location
