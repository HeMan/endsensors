
.nolist
.include "tn45def.inc"
.list

main:
    ldi R16, 0b00011111
    out DDRB, R16
loop:
    out PINB, R16 ; toggle outputs
    rcall delay
    rjmp main
    
delay:
    clr R24
    clr R25
    clr R26
del1:
    SUBI R26, 1
    BRNE del1
    SUBI R25, 1
    BRNE del1
    SUBI R24, 1
    BRNE del1
    RET
