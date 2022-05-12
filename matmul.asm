.m 0x10
.l 0x10
.n 0x10

lw r0 r1 m
lw r0 r2 l
lw r0 r3 n
# go straight to mult
beq r0 r0 inner_most

outer:
# reset counter of inner loop
addi r0 r5 0x0
# reset counter of inner-most loop
addi r0 r6 0x0
# increment counter of outermost loop
addi r4 r4 0x1
# reset accumulator
addi r0 r9 0x0
# branch to end counter complete
bgte r4 r1 end
# branch to inner most
beq r0 r0 inner_most

inner:
#add one to counter for current loop
addi r5 r5 0x1 
# branch to outer loop when inner loop is complete
bgte r5 r3 outer
#reset counter for inner most loop
addi r0 r6 0x0
# reset accumulator
addi r0 r9 0x0

inner_most:
# index calculation for A[m][k] => i = (r4 x number of rows in A) + r6
mult r7 r4 r1
add r7 r7 r6
# index calculation for B[k][r] => i = (r6 x number of comon columns in A & B) + r5
mult r8 r6 r2
add r8 r8 r5
# main operation
lw r7 r7 a
lw r8 r8 b
mult r8 r8 r7
add r9 r9 r8
# increment counter for inner most loop
addi r6 r6 0x1
# branch to inner loop if inner most loop is complete
bgte r6 r2 store
# branch to inner most loop again if loop isn't complete
beq r0 r0 inner_most

store:
sw r10 r9 c
addi r10 r10 0x1
beq r0 r0 inner
halt

end:
halt

.data
.a 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1
.b 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1
.c 0