.n 0xfffff

lw r0 r1 n
addi r0 r2 0x1
beq r0 r0 and_add
h_w:
lsr r1 r1 0x1
beq r0 r0 and_add
and_add:
and r3 r1 r2
add r4 r4 r3
bne r0 r1 h_w
halt
