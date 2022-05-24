print("Input Filename: ")
fname = input()

print("Dimensions: ")
dim = int(input())

print("Output Name (with extension): ")
otpn = input()

f = open(fname)

all = []
for line in f:
    strn = line.split(" ")[:-1]
    all += strn

ints = []
for i in range(len(all)):
    num = int(all[i])
    if num < 0:
        ints.append(0)
    elif num > 255:
        ints.append(255)
    else:
        ints.append(num)
        
print("Len: ", len(ints))

f = open(otpn, "w")
f.write("P2\n")
f.write("%d %d\n" % (dim, dim))
f.write("255\n")
for i in range(dim):
    strn = ""
    for j in range(dim):
        index = (i * dim) + j
        strn += str(ints[index])
        strn += " "
    strn += "\n"
    f.write(strn)
    
        
    
