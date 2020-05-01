import math

size = 256
tests = True
# number_of_rounds = 2

S1_probability = [{0: 0.0625, 1: 0.03125, 2: 0.0625, 3: 0.0625, 4: 0.09375, 5: 0.09375, 6: 0.0625, 7: 0,
                   8: 0, 9: 0.09375, 10: 0.125, 11: 0.03125, 12: 0.0625, 13: 0.03125, 14: 0.0625, 15: 0.125},
                  {0: 0.0625, 1: 0.09375, 2: 0.0625, 3: 0.0625, 4: 0.03125, 5: 0.03125, 6: 0.0625, 7: 0.125,
                   8: 0.125, 9: 0.03125, 10: 0, 11: 0.09375, 12: 0.0625, 13: 0.09375, 14: 0.0625, 15: 0}]

S5_probability = [{0: 0, 1: 0.125, 2: 0.125, 3: 0, 4: 0.09375, 5: 0, 6: 0.03125, 7: 0.125,
                   8: 0.0625, 9: 0, 10: 0.0625, 11: 0.125, 12: 0.09375, 13: 0.09375, 14: 0.0625, 15: 0},
                  {0: 0.125, 1: 0, 2: 0, 3: 0.125, 4: 0.03125, 5: 0.125, 6: 0.09375, 7: 0,
                   8: 0.0625, 9: 0.125, 10: 0.0625, 11: 0, 12: 0.03125, 13: 0.03125, 14: 0.0625, 15: 0.125}]



# comparing the 4 lsb of the 2 parameters
def equality_right_sides(plain, cipher):
    for i in range(4, 8):  # the RIGHT bits
        if plain[i] != cipher[i]:
            return False
    return True


# returing the currect dicionary according to the inserted s-box value and plaintext
def prob_sbox_bit(plain, s_box, key):
    if s_box == 1:
        bit_index = 7
        dictionary_index = S1_probability
    else:
        bit_index = 5
        dictionary_index = S5_probability
    return dictionary_index[int(plain[bit_index])^key]


# returns the xor of the two left parts of the input
def left_side_xor(plain, cipher):
    left_p = plain >> 4
    left_c = cipher >> 4
    return left_c ^ left_p


# this function 
def prob_calculation(plain, cipher, key, s_box):
    str_plain = format(plain, '08b')
    str_cipher = format(cipher, '08b')

    if not equality_right_sides(str_plain, str_cipher):
        return 0
    else:
        return prob_sbox_bit(str_plain, s_box, key)[left_side_xor(plain, cipher)]


def swap_cipher(cipher):
    right = cipher >> 4 
    left = (cipher << 4)%256
    return left|right


def fill_partial_m1(sbox, key):
    temp_matrix = [[0 for i in range(size)] for j in range(size)]
    for i in range(size):
        for j in range(size):
            cipher = swap_cipher(j)
            temp_matrix[i][j] = prob_calculation(i, cipher, key, sbox)

    return temp_matrix


def matrix_product(A, B):
    n = len(A)
    C = [[0 for i in range(n)] for j in range(n)]
    for i in range(n):
        for j in range(n):
            for k in range(n):
                C[i][j] += A[i][k] * B[k][j]
    return C


# [S5-0, S5-1, S1-0, S1-1, S1-0-no_swap, S1-1-no_swap]
m1 = [[] for k in range(4)]
m1[0] = fill_partial_m1(5, 0)
m1[1] = fill_partial_m1(5, 1)
m1[2] = fill_partial_m1(1, 0)
m1[3] = fill_partial_m1(1, 1)
# m1[4] = fill_partial_m1(1, 0, True)
# m1[5] = fill_partial_m1(1, 1, True)


def calculate_2levels_matrix():
    m2 = [[] for k in range(4)]
    m2[0] = matrix_product(m1[0],m1[2]) #key = 00
    m2[1] = matrix_product(m1[0],m1[3]) #key = 01
    m2[2] = matrix_product(m1[1],m1[2]) #key = 10
    m2[3] = matrix_product(m1[1],m1[3]) #key = 11
    return m2


# mat[levels][key][plaintext][ciphertext]
mat = [[] for k in range(15)]
for level in range(15):
    if level == 0 or level % 2 == 1:
        continue
    mat[level]=[[] for k in range(pow(2,level))]
    for key in range(pow(2,level)):
        subkey_left = math.floor(key / 4)
        subkey_right = key % 4
        if level == 2:
            mat[2]= calculate_2levels_matrix()
        else:
            mat[level][key]= matrix_product(mat[level-2][subkey_left], mat[2][subkey_right])


# out s5 after permutation - 3, 8, 14, 25
# out s1 after permutation - 9, 17, 23, 31
# mask for plaintext - out s5 || out s1 - 3, 8, 14, 25, 41, 49, 55, 63
# mask for ciphertext - out s1 || out s5 - 9, 17, 23, 31, 35, 40, 46, 57

mask_for_plain = [3, 8, 14, 25, 41, 49, 55, 63]
mask_for_cipher = [9, 17, 23, 31, 35, 40, 46, 57]


# returns a substring which contains bits from specific places in str
def get_sub_input(list, str):
    sub_str = []
    for i in range(list.len()):
        sub_str += str[list[i]]


# reads the next pair of inputs and return the relevant bits from them
# the relevant bits are the outputs bits of S1, S5
def get_next_sub_input(index):
    (plain, cipher) = get_next_input_from_file(index)
    binary_plain = format(plain, '64b')
    binary_cipher = format(cipher, '64b')
    sub_plain = get_sub_input(mask_for_plain, binary_plain)
    sub_cipher = get_sub_input(mask_for_cipher, binary_cipher)
    return (int(sub_plain), int(sub_cipher))


num_of_inputs  #  from the file

mat_summing = [[0 for j in range(size)]for i in range(size)]
sum_for_plaintext =[0 for i in range(size)]

for i in range(num_of_inputs):
    (plain, cipher) = get_next_sub_input(i)  # 8 bits of the i-th plaintext and ciphertext as decimal number
    mat_summing[plain][cipher] += 1
    sum_for_plaintext[plain] += 1

mat_probabilities = [[0 for j in range(size)]for i in range(size)]
for plain in range(size):
    for cipher in range(size):
        mat_probabilities[plain][cipher] = mat_summing / sum_for_plaintext[plain]  # conditional probability


#######################################################
#                        Tests
#######################################################

a = [[3,4,5],[6,7,8],[1,2,3]]

for line in range(6):
    for val in range(6):
        print ("line : ", line, " val: ", val)
        print(mat[14][0][line][val])
    print('\n')

