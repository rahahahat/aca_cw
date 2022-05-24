import os
import subprocess

img_size = 200

print("Images\nLena - 1\nBuilding - 2\nMonkey - 3\nChoose Image (Default is Lena) :")
inp = input()
image = "lena.pgm"
if inp == "2":
    image = "building.pgm"
if inp == "3":
    image = "monkey.pgm"

print("Image Kernels\nEmboss - 1\nEdge (Bottom Sobel) - 2\nSharpen- 3\nChoose Kernel (Default is Edge) :")
kernel = "edge"
inp = input()
if inp == "1":
    kernel = "emboss"
if inp == "3":
    kernel = "sharpen"
    
kernel_dict = {
    "emboss":  [-2,-1,0,-1,1,1,0,1,2],
    "edge":   [-1,-2,-1,0,0,0,1,2,1],
    "sharpen":[0,-1,0,-1,5,-1,0,-1,0],
}
kernel = kernel_dict[kernel]

var_pos = [".tl", ".tm", ".tr", ".ml", ".md", ".mr", ".bl", ".bm", ".br"]

inp_img = open(image, "r")

# Remove Metadata 
inp_img.readline()
inp_img.readline()
inp_img.readline()

img_str = ""
for line in inp_img:
    img_str += line[:-1]

    
conv_template = open("conv.template", "r")
conv_file = open("_conv.asm", "w")

for i in range(9):
    entry = "%s %s\n" % (var_pos[i], hex(kernel[i]))
    conv_file.write(entry)

conv_file.write("\n")
conv_file.write(".m %s\n" % hex(img_size))
conv_file.write(".n %s\n" % hex(img_size))
conv_file.write("\n")

for line in conv_template:
    conv_file.write(line)

conv_file.write("\n\n")
conv_file.write(".data\n")
conv_file.write(".img ")
conv_file.write(img_str)
conv_file.write("\n")
conv_file.write(".d 0")

conv_template.close()
conv_file.close()
inp_img.close()


def output():

    fname = "conv_res.txt"
    dim = 200

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
        
subprocess.call(["./build/out"])
output()