; Paul Goldstein
; 1723251405
; It counts to 10!

.setcpu  "6502"

.segment "CODE"

	SEC
	LDA #0
    SBC #0
    BRK


.segment "RESETVEC"
    .word $FF00 ; NMI vector location
    .word $8000 ; Reset vector location
    .word $FF00 ; BRK vector location
