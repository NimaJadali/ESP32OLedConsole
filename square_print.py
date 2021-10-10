a = "0b11111111, "
size = (16, 16) #w x h
b = ""
for i in range(int(size[0]/8) * size[1]):
    b = b + a
print(b)