# from Calculate_Key_Round_Matrices import result
import pickle
from pathlib import Path

matrix_size = 256

#################
#     Masks     #
#################

# out s5 after permutation - 3, 8, 14, 25 (starting from 1)
# out s1 after permutation - 9, 17, 23, 31
# mask for plaintext - out s5 || out s1 - 3, 8, 14, 25, 41, 49, 55, 63
# mask for both plaintext and ciphertext, assuming swap at the last round
text_mask = [2, 7, 13, 24, 40, 48, 54, 62]  # starts from 0

# key[i] - the key bit we use in level i+1
key_mask = [27, 51, 3, 48, 38, 16, 6, 49, 45, 25, 13, 58, 44, 26, 12, 2]  # starts at 0



# T
# input: string, mask - which mask to truncates by
# output: returns a substring which contains bits from specific places in str
def get_sub_input(str_input, mask):
    sub_str = ''
    for i in range(len(mask)):
        sub_str += str_input[mask[i]]
    return sub_str

# # T
# # input: 
# # output: 
# def swap_int(num):
#     bin_num = format(num, '08b')
#     bin_num = bin_num[4:] + bin_num[:4]
#     return int(bin_num, 2)

# Tested
# input: file_object - the file object from which we read our input (File type) 
# output: reads the next pair of inputs and return the relevant bits from threadm as integers
#         the relevant bits are the outputs bits of S1, S5
def get_next_input_from_file(file_object):
    data_line = file_object.readline()
    while data_line:
        data = data_line.split()
        binary_plain = data[0]
        binary_cipher = data[1]
        sub_plain = get_sub_input(binary_plain, text_mask)
        sub_cipher = get_sub_input(binary_cipher, text_mask)
        yield int(sub_plain, 2), int(sub_cipher, 2)
        data_line = file_object.readline()

# Checked
# output: the "matrix_distance" between the probability-matrix matching the current key 
#         to the probability matrix matching the wanted-key 
def calculate_key_distance(key, mat_summing, num_of_inputs, mat, num_of_rounds):
    distance = 0
    for i in range(matrix_size):
        for j in range(matrix_size):
            part_1 = (mat[num_of_rounds][key][i][j]  - (1 / matrix_size))
            part_2 = mat_summing[i][j] - (num_of_inputs / pow(matrix_size, 2))
            # part_1 = mat_summing[i][j] - (num_of_inputs / matrix_size)
            # part_2 = (mat[num_of_rounds][key][i][j] * num_of_inputs / pow(2, num_of_rounds)) - (num_of_inputs / matrix_size)
            distance += (part_1 * part_2)
    return distance


def attack_algorithm_1(file_name, probabilities_matrix_file_number):
    probabilities_matrix_file = str(probabilities_matrix_file_number) + "_Round_Expected_Probabilities_Matrix.txt"
    
    # TODO: finish the validity check
    # probabilities_matrix_file_path = Path("/path/to/file")
    # if (os.path.isfile(probabilities_matrix_file_path) == False):
    #     print("Probabilities Matrix File Does Not Exists")
        # exit()


    # mat parameter is constructed as mat[rounds][key_(2^rounds)][plaintext_256][ciphertext_256]
    #                                 mat[0 || i%2 == 1] = empty
    with open(probabilities_matrix_file, "rb") as results:  # Unpickling
        mat = pickle.load(results)
        
    # receiving the `number of rounds` and the `key` from the first line of the file
    file_object = open(file_name, "r")
    first_line = file_object.readline().split(" ")
    # first line format : "num of rounds: X key: X"
    num_of_rounds = int(first_line[1])
    actual_key = first_line[3]
    num_of_inputs = 0

    # mat_summing counts the sub_plaintexts and sub_ciphertexts pairs
    # plaintext_counter counts the number of sub_plaintext inputs
    mat_summing = [[0 for j in range(matrix_size)] for i in range(matrix_size)]
    plaintext_counter = [0 for i in range(matrix_size)]

    # preparing the data for calculating mat_probabilities
    # sub_plaintext and sub_ciphertext are the 8bits in the plain/cipher which we are checking
    for plain, cipher in get_next_input_from_file(file_object):
        mat_summing[plain][cipher] += 1 #counts the amount of pairs of sub_plain-sub_cipher
        # plaintext_counter[plain] += 1 #counts the amount of appearances of each sub_plaintext
        num_of_inputs += 1

    file_object.close()

    # # mat_probabilities - conditional probability
    # mat_probabilities = [[0 for j in range(matrix_size)] for i in range(matrix_size)]
    # for plain in range(matrix_size):
    #     for cipher in range(matrix_size):
    #         if plaintext_counter[plain] == 0:
    #             mat_probabilities[plain][cipher] = 0
    #         else:
    #             mat_probabilities[plain][cipher] = mat_summing[plain][cipher] / plaintext_counter[plain]

    # key[i] - the key bit we use in level i+1
    # key_mask = [27, 51, 3, 48, 38, 16, 6, 49, 45, 25, 13, 58, 44, 26, 12, 2]  # starts at 0
    sub_actual_key = get_sub_input(actual_key, key_mask)
    sub_actual_key_by_rounds = sub_actual_key[:num_of_rounds]
    real_distance = calculate_key_distance(int(sub_actual_key_by_rounds, 2), mat_summing, num_of_inputs, mat, num_of_rounds)

    real_location = 1
    # max_key - the candidate for the right sub_key
    for temp_key in range(pow(2, num_of_rounds)): #iterating over all of the possible keys
        if temp_key == sub_actual_key_by_rounds:
            continue
        curr_dist = calculate_key_distance(temp_key, mat_summing, num_of_inputs, mat, num_of_rounds)
        if curr_dist > real_distance:
            real_location += 1

    return real_location

