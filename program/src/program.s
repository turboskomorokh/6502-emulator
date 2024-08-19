; Paul Goldstein
; 1724083321
; It counts to 10!

.setcpu  "6502"

.segment "CODE"

    LDA #$10  ;
    LDX #$15  ;
    STX $2001 ;
    ADC $2001 ;
    STA $2000 ;

.segment "RESETVEC"
    .word $FF00 ; NMI vector location
    .word $8000 ; Reset vector location
    .word $FF00 ; BRK vector location
