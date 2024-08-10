.fopt	author, "Paul Goldstein"
.fopt	epoch, ""
.fopt	name, "It counts to 10!"
.setcpu  "6502"

.segment "CODE"

START:
	JSR INX10   ; 6
	STA $2000   ; 4
	BRK         ; 7

INX10:
	INX         ; 2
	TXA         ; 2
	CMP #10     ; 2
	BEQ INX10END; 2 (+1 on the end)
	JMP INX10   ; 3
INX10END:
	RTS         ; 6

.segment "RESET"
    .word $FF00 ; NMI
    .word $8000 ; RESET
    .word $FF00 ; BRK
