.n 0xa

addi r0 r1 0x1
addi r0 r2 0x1
lw r0 r3 n
factorial:
mult r2 r2 r3
sub r3 r3 r1
bne r3 r1 factorial
halt

