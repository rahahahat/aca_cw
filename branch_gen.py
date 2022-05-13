import random


    
template = open("branch.tem", "r")
output = open("branch.asm", "w")

print("Length of bitstring: ")
strLen = int(input())

output.write((".n %s" % str(hex(strLen))))
output.write("\n\n")
for line in template:
    output.write(line)
output.write("\n")

# print("Choose Pattern (Default: 1Bit) \n(0) Random\n(1) 1 Bit Alternate\n(2)Mixed Variable Sequence\n(3) Single Mixed Variable sequence\n(4) Single Fixed variable Sequence")
print("Choose Pattern (Default: 1Bit) \n(0) Random\n(1) 1 Bit Alternate\n(2) N-bit Alternate")
pattern = input()

def gen_random(strLen):
    bit_arr_1 = [random.randrange(2) for _ in range(strLen)]
    bit_arr_1 = [str(bit_arr_1[i]) for i in range(strLen)]
    bit_arr_1 = " ".join(bit_arr_1)

    bit_arr_2 = [random.randrange(2) for _ in range(strLen)]
    bit_arr_2 = [str(bit_arr_2[i]) for i in range(strLen)]
    bit_arr_2 = " ".join(bit_arr_2)

    output.write(".data\n")
    output.write((".a %s\n" % bit_arr_1))
    output.write((".b %s\n" % bit_arr_2))

def gen_1bitAlt(strLen):
    bit_arr_1 = [1 for _ in range(strLen)]
    bit_arr_1 = [str(bit_arr_1[i]) for i in range(strLen)]
    bit_arr_1 = " ".join(bit_arr_1)
    
    bit_arr_2 = [0 if (i % 2 == 0) else 1 for i in range(strLen)]
    bit_arr_2 = [str(bit_arr_2[i]) for i in range(strLen)]
    bit_arr_2 = " ".join(bit_arr_2)
    
    output.write(".data\n")
    output.write((".a %s\n" % bit_arr_1))
    output.write((".b %s\n" % bit_arr_2))
    
def gen_nbit_alternate(strLen):
    print("Bit Length: ")
    max_seq_len = int(input())
    if (max_seq_len > strLen):
        raise Exception("Bit length should be smaller than data length")
    
    bit_arr_1 = [1 for _ in range(strLen)]
    bit_arr_1 = [str(bit_arr_1[i]) for i in range(strLen)]
    bit_arr_1 = " ".join(bit_arr_1)
    
    bit_arr_2 = gen_fixed_seq(max_seq_len, strLen)
    
    output.write(".data\n")
    output.write((".a %s\n" % bit_arr_1))
    output.write((".b %s\n" % bit_arr_2))
    
def gen_fixed_seq(max_seq_len, max_len):
    arr = []
    curr_len = 0
    while curr_len < max_len:
        bit = "1"
        for _ in range(max_seq_len):
            if curr_len < max_len:
                arr.append(bit)
            curr_len += 1
        if curr_len < max_len:
            arr.append("0")
            curr_len += 1
    return " ".join(arr)

if pattern == "0":
    gen_random(strLen)
elif pattern == "2":
    gen_nbit_alternate(strLen)
else:
    gen_1bitAlt(strLen)
    

# def gen_variable_sequence(strLen):
#     print("Input Maximum Sequence Length: ")
#     max_seq_len = int(input())
#     if (max_seq_len > strLen):
#         raise Exception("Sequence length should be smaller than data length")
#     bit_arr_1 = gen_seq(max_seq_len, strLen)
#     bit_arr_2 = gen_seq(max_seq_len, strLen)
    
#     output.write(".data\n")
#     output.write((".a %s\n" % bit_arr_1))
#     output.write((".b %s\n" % bit_arr_2))
    
# def gen_single_variable_sequence(strLen):
#     print("Input Maximum Sequence Length: ")
#     max_seq_len = int(input())
#     if (max_seq_len > strLen):
#         raise Exception("Sequence length should be smaller than data length")
#     bit_arr_1 = [1 for _ in range(strLen)]
#     bit_arr_1 = [str(bit_arr_1[i]) for i in range(strLen)]
#     bit_arr_1 = " ".join(bit_arr_1)
#     bit_arr_2 = gen_seq(max_seq_len, strLen)
#     output.write(".data\n")
#     output.write((".a %s\n" % bit_arr_1))
#     output.write((".b %s\n" % bit_arr_2))
    
# def gen_single_fixed_sequence(strLen):
#     print("Input Maximum Sequence Length: ")
#     max_seq_len = int(input())
#     if (max_seq_len > strLen):
#         raise Exception("Sequence length should be smaller than data length")
#     bit_arr_1 = [1 for _ in range(strLen)]
#     bit_arr_1 = [str(bit_arr_1[i]) for i in range(strLen)]
#     bit_arr_1 = " ".join(bit_arr_1)
#     bit_arr_2 = gen_fixed_seq(max_seq_len, strLen)
#     output.write(".data\n")
#     output.write((".a %s\n" % bit_arr_1))
#     output.write((".b %s\n" % bit_arr_2))

# def gen_seq(max_seq_len, max_len):
#     arr = []
#     curr_len = 0
#     while curr_len < max_len:
#         bit = str(random.randrange(2))
#         seq_len = random.randrange(max_seq_len)
#         for i in range(seq_len):
#             if curr_len < max_len:
#                 arr.append(bit)
#             curr_len += 1
#     return " ".join(arr)