# addi r0 r1 0x1
# addi r1 r1 0xa
# beq r0 r0 addr
# end:
# halt
# addr:
# add r1 r1 r1
# halt

.n 0x10
lw r0 r1 n
add r2 r1 r1
halt


# 1|2|3|4|5|6|7|
# F|D|E|M|W| | |
#  |F|D|E|W|W| |
#  | |F|D|E|W|W|