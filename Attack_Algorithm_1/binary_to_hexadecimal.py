
# Binary to 8-Decimal
x = '0100100100100100100100001001001100001001001101001000101000100100'
y = [x[8*i:8*i+8] for i in range(8)]
z = [int(num, 2) for num in y]

# print(z)

# 8-Hexadecimal to Binary
t = ''
x = 'CF 2F D2 ED 86 DC 9D A4'
y = [x[3*i:3*i+2] for i in range(8)]
for i in range(8):
    t += format(int(y[i], 16), '08b')

print(t)



[58, 52, 34, 49, 2, 17, 35, 50, 11, 26, 44, 59, 41, 27, 9, 3]

