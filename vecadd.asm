.a 0x19

# Load size of vector into memory
lw r0 r1 a
vec_add:
# load element of first vector in $r5
lw r2 r5 b
# load element of second vector in $r6
lw r3 r6 c 
add r7 r5 r6
# Store result computed in last step
sw r4 r7 d
# increment register values for each register
addi r4 r4 0x1
addi r2 r2 0x1
addi r3 r3 0x1
# branch if $r2 < $r1
bl r2 r1 vec_add
halt

.data
.b 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25
.c 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25 25
.d 0