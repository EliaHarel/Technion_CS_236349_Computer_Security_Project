# guess the first and the last round
# use algorithm 1 with 14 rounds
# calculate the matrices

# out s5 after permutation - 7, 13, 24, 2 (starting from 0)
# out s1 after permutation - 8, 16, 22, 30 (starting from 0)
import pickle

matrix_size = 256
s5_out_mask = [7, 13, 24, 2]  # starting from 0
s1_out_mask = [8, 16, 22, 30]  # starting from 0
s1_in_mask = [31, 0, 1, 2, 3, 4]  # starting from 0
s5_in_mask = [15, 16, 17, 18, 19, 20]  # starting from 0

# c - sub plain R || sub plain L || sub cipher R || sub cipher L
plain_L_mask = s1_out_mask  # [8, 16, 22, 30]
plain_R_mask = s5_out_mask + s1_in_mask  # [7, 13, 24, 2, 31, 0, 1, 2, 3, 4]
cipher_L_mask = s5_out_mask  # [7, 13, 24, 2]
cipher_R_mask = s1_out_mask + s5_in_mask  # [8, 16, 22, 30, 15, 16, 17, 18, 19, 20]

# key[i] - the key bit we use in level i+1
key_mask = [27, 51, 3, 48, 38, 16, 6, 49, 45, 25, 13, 58, 44, 26, 12, 2]  # starts at 0

k1_s1_mask = [10, 51, 34, 60, 49, 17]
k16_s5_mask = [30, 5, 47, 62, 45, 12]
first_last_mask = k1_s1_mask + k16_s5_mask
key_mask_14_mid_rounds = [51, 3, 48, 38, 16, 6, 49, 45, 25, 13, 58, 44, 26,
                          12]  # starts at 0, repeated keys: 51, 49, 45, 12

s1 = [[14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7],
      [0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8],
      [4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0],
      [15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13]]

s5 = [[2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9],
      [14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6],
      [4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14],
      [11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3]]


# reads the next pair of inputs and return the relevant bits from them as integers
# the relevant bits are the outputs bits of S1, S5


# returns a substring which contains bits from specific places in str
def get_sub_input(str_input, mask):
    sub_str = ''
    for i in range(len(mask)):
        sub_str += str_input[mask[i]]
    return sub_str


def get_next_input_from_file(file_object):
    data_line = file_object.readline()
    while data_line:
        data = data_line.split()
        binary_plain = data[0]
        binary_cipher = data[1]

        binary_plain_R = binary_plain[32:]
        binary_plain_L = binary_plain[:32]
        binary_cipher_R = binary_cipher[32:]
        binary_cipher_L = binary_cipher[:32]

        ### creating sub-strings containing the relevent bits for each text input
        sub_plain_R = get_sub_input(binary_plain_R, plain_R_mask)
        sub_plain_L = get_sub_input(binary_plain_L, plain_L_mask)
        sub_cipher_R = get_sub_input(binary_cipher_R, cipher_R_mask)
        sub_cipher_L = get_sub_input(binary_cipher_L, cipher_L_mask)

        ### todo: we can do this above
        sub_plain = sub_plain_L + sub_plain_R
        sub_cipher = sub_cipher_L + sub_cipher_R
        sub_input = sub_plain + sub_cipher

        yield int(sub_input, 2)
        data_line = file_object.readline()


# returns the output of the s_box for the given s_box_num and binary_input
def s_box_function(s_box_num, binary_input):
    # todo: check this syntax of appending binary values
    row = int(binary_input[0] + binary_input[5], 2)
    col = int(binary_input[1:5], 2)
    if s_box_num == 1:
        s_box = s1
    else:
        s_box = s5
    return s_box[row][col]


# TODO: add explanations to the indexed values

def list_to_str(input_list):
    output_str = ''
    for i in range(len(input_list)):
        output_str += input_list[i]
    return output_str


def calculate_P_C_from_key_combination(key, combination):
    binary_key = format(key, '012b')
    k1 = list_to_str(binary_key[:6])
    k16 = list_to_str(binary_key[6:])

    # combination - p out s1 (4)|| p out s5 (4) || p in s1 (6)|| c out s5 (4)|| c out s1 (4)|| c in s5 (6)
    # combination -     0-3     ||     4-7      ||    8-13    ||    14-17    ||    18-21    ||   22-27
    binary_combination = format(combination, '028b')

    # plaintext - (out S5)_2 || (out S1)_2 = (out S5)_1 || ((out S1)_1 xor S1(in S1, K1))
    plain_left = list_to_str(binary_combination[4:8])  # (out S5)_1
    binary_input1 = list_to_str(binary_combination[8:14])  # in S1
    input_to_s1 = format(int(binary_input1, 2) ^ int(k1, 2), '06b')
    out_s1_1 = list_to_str(binary_combination[:4])
    plain_right = str(int(out_s1_1, 2) ^ s_box_function(1, input_to_s1))
    plaintext = int(plain_left + plain_right, 2)

    # ciphertext - (out S1)_2 || (out S5)_2 = (out S1)_1 || ((out S1)_1 xor S5(in S5, K16))
    cipher_left = list_to_str(binary_combination[18:22])
    binary_input16 = str(binary_combination[22:])
    input_to_s5 = format(int(binary_input16, 2) ^ int(k16, 2), '06b')
    out_s5_1 = list_to_str(binary_combination[14:18])
    cipher_right = str(int(out_s5_1, 2) ^ s_box_function(5, input_to_s5))
    ciphertext = int(cipher_left + cipher_right, 2)

    return plaintext, ciphertext


# num_of_rounds is the number of the rounds characteristics
def calculate_distance(first_last_key, middle_key, num_of_rounds, num_of_inputs, compared_matrices, input_matrix):
    distance = 0
    for i in range(matrix_size):
        for j in range(matrix_size):
            part_1 = input_matrix[first_last_key][i][j] - num_of_inputs / matrix_size
            part_2 = compared_matrices[num_of_rounds][middle_key][i][j] * num_of_inputs - num_of_inputs / pow(matrix_size, 2)
            distance += part_1 * part_2
    return distance


# probabilities_matrix_file_number includes the first and last round
def attack_algorithm_2(input_file, probabilities_matrix_file_number):
    probabilities_matrix_file = str(probabilities_matrix_file_number-2) + "_Round_Expected_Probabilities_Matrix.txt"
    with open(probabilities_matrix_file, "rb") as results:  # Unpickling
        compared_matrices = pickle.load(results)

    # num_of_rounds = probabilities_matrix_file_number
    file_object = open(input_file, "r")
    first_line = file_object.readline().split()
    num_of_rounds = int(first_line[1])
    real_key = first_line[3]
    num_of_inputs = 0
    characteristic_rounds = num_of_rounds - 2

    ### counter is the C array, BUT IN DIFFERENT OREDER!!!!
    counter = [0 for i in range(pow(2, 28))]

    # plain - 14 bits - out s1 || out s5 || in s1
    # cipher - 14 bits -  out s5 || out s1 || in s5
    # index (int)- plain || cipher
    for index in get_next_input_from_file(file_object):
        counter[index] += 1
        num_of_inputs += 1

    # matrix [key][plain][cipher], key - guessed key for 12 key bits of the first and last rounds
    ### the M matrix 12 key bits = 6 key bits of s1 | 6 key bits of s5
    ###				 8 plain bits = 4 output bits of s5 | 4 output bits of s1 (in round 2 of DES (round 1 in our characteristic)
    ###				 8 cipher bits = 4 output bits of s1 | 4 output bits of s5
    input_matrix = [[[0 for k in range(pow(2, 8))] for j in range(pow(2, 8))] for i in range(pow(2, 12))]

    ### combination iterating over all of the possible values of the 14 bits from the plain text and the 14 bits from the cipher text
    for key in pow(2, 12):
        for combination in pow(2, 28):
            plain, cipher = calculate_P_C_from_key_combination(key, combination)
            # counter[combination] holds the value of the "confirmed values"(C array) for the current combination
            input_matrix[key][plain][cipher] += counter[combination]

    real_location = 1
    curr_first_last_key = get_sub_input(real_key, first_last_mask)  # 12 bits
    curr_middle_key = get_sub_input(real_key, key_mask_14_mid_rounds)
    curr_middle_key_by_rounds = curr_middle_key[characteristic_rounds]

    # calculate real key distance
    real_distance = calculate_distance(curr_first_last_key, curr_middle_key_by_rounds, num_of_rounds-2, num_of_inputs,
                                       compared_matrices, input_matrix)

    for first_last_key in range(pow(2, 12)):
        for middle_key in range(pow(2, characteristic_rounds)):
            if first_last_key == curr_first_last_key and middle_key == curr_middle_key_by_rounds:
                continue
            curr_dist = calculate_distance(first_last_key, middle_key, characteristic_rounds, num_of_inputs)
            if curr_dist > real_distance:
                real_location += 1
    return real_location
