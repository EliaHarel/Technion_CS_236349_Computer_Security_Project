
mask = [2,5,8]

# returns a substring which contains bits from specific places in str
def get_sub_input(str_input):
    sub_str = ''
    for i in range(len(mask)):
        sub_str += str_input[mask[i]]
    return sub_str


# reads the next pair of inputs and return the relevant bits from them as integers
# the relevant bits are the outputs bits of S1, S5
def get_next_sub_input(index, plain, cipher):
    binary_plain = format(plain, '064b')
    binary_cipher = format(cipher, '064b')
    sub_plain = get_sub_input(binary_plain)
    sub_cipher = get_sub_input(binary_cipher)
    print(sub_plain)
    print(sub_cipher)
    return (int(sub_plain,2), int(sub_cipher,2))   # check binary to decimal conversion



num = 376934857454395734
print(num)
b_num = format(num, '064b')
print(b_num)

num2 = 4373955354354785649
print(num2)
b_num2 = format(num2, '064b')
print(b_num2)
print()

print(get_next_sub_input(0, num, num2))
