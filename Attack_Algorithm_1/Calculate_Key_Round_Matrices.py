import math

matrix_size = 256
num_of_rounds = 4

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


# returning the correct dictionary according to the inserted s-box value and plaintext
def prob_sbox_bit(plain, s_box, key):
    if s_box == 1:
        bit_index = 7
        dictionary_index = S1_probability
    else:
        bit_index = 5
        dictionary_index = S5_probability
    return dictionary_index[int(plain[bit_index]) ^ key]


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
    left = (cipher << 4) % 256
    return left | right


def fill_partial_m1(sbox, key):
    temp_matrix = [[0 for i in range(matrix_size)] for j in range(matrix_size)]
    for i in range(matrix_size):
        for j in range(matrix_size):
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


def calculate_2levels_matrix():
    m2 = [[] for k in range(4)]
    m2[0] = matrix_product(m1[0], m1[2])  # key = 00
    m2[1] = matrix_product(m1[0], m1[3])  # key = 01
    m2[2] = matrix_product(m1[1], m1[2])  # key = 10
    m2[3] = matrix_product(m1[1], m1[3])  # key = 11
    return m2


# [S5-0, S5-1, S1-0, S1-1, S1-0-no_swap, S1-1-no_swap]
m1 = [[] for k in range(4)]
m1[0] = fill_partial_m1(5, 0)
m1[1] = fill_partial_m1(5, 1)
m1[2] = fill_partial_m1(1, 0)
m1[3] = fill_partial_m1(1, 1)


# m1[4] = fill_partial_m1(1, 0, True)
# m1[5] = fill_partial_m1(1, 1, True)


# mat[levels][key][plaintext][ciphertext]
def result():
    mat = [[] for k in range(num_of_rounds + 1)]
    for level in range(num_of_rounds + 1):
        if level == 0 or level % 2 == 1:
            continue
        mat[level] = [[] for k in range(pow(2, level))]
        for key in range(pow(2, level)):
            subkey_left = math.floor(key / 4)
            subkey_right = key % 4
            if level == 2:
                mat[2] = calculate_2levels_matrix()
            else:
                mat[level][key] = matrix_product(mat[level - 2][subkey_left], mat[2][subkey_right])
    return mat
