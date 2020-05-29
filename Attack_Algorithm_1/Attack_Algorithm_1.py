from Calculate_Key_Round_Matrices import mat
matrix_size = 256
num_of_rounds = 16

file_name = ""

# out s5 after permutation - 3, 8, 14, 25
# out s1 after permutation - 9, 17, 23, 31
# mask for plaintext - out s5 || out s1 - 3, 8, 14, 25, 41, 49, 55, 63

# mask for both plaintext and ciphertext, assuming swap at the last round
text_mask = [2, 7, 13, 24, 40, 48, 54, 62]


# returns a substring which contains bits from specific places in str
def get_sub_input(str_input, mask):
    sub_str = ''
    for i in range(len(mask)):
        sub_str += str_input[mask[i]]
    return sub_str


# reads the next pair of inputs and return the relevant bits from them as integers
# the relevant bits are the outputs bits of S1, S5
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


# return the distance between the matrix of key and the matrix we calculated
def calculate_distance(key):
    distance = 0
    for i in range(matrix_size):
        for j in range(matrix_size):
            part_1 = mat_probabilities[i][j] - num_of_inputs / pow(2, 16)
            part_2 = mat[num_of_rounds][key][i][j] * num_of_inputs / pow(2, 8) - num_of_inputs / pow(2, 16)
            distance += part_1 * part_2
    return abs(distance)


# mat_summing counts the sub_plaintest and sub_cipherrtext pairs
# sum_for_plaintext counts the number of sub_plaintext inputs
mat_summing = [[0 for j in range(matrix_size)] for i in range(matrix_size)]
sum_for_plaintext = [0 for i in range(matrix_size)]

file_object = open(file_name, "r")
first_line = file_object.readline().split(",")
real_rounds = first_line[0].split()[1]
real_key = first_line[1].split()[1]
num_of_inputs = 0

# preparing the data for calculating mat_probabilities
for plain, cipher in get_next_input_from_file(file_object):
    num_of_inputs += 1
    mat_summing[plain][cipher] += 1
    sum_for_plaintext[plain] += 1

file_object.close()

# mat_probabilities - conditional probability
mat_probabilities = [[0 for j in range(matrix_size)] for i in range(matrix_size)]
for plain in range(matrix_size):
    for cipher in range(matrix_size):
        mat_probabilities[plain][cipher] = mat_summing[plain][cipher] / sum_for_plaintext[plain]

# key[i] - the key bit we use in level i+1
key_mask = [28, 52, 4, 49, 39, 17, 7, 50, 46, 26, 14, 59, 45, 27, 13, 3]

sub_real_key = get_sub_input(real_key, key_mask)
sub_real_key_by_rounds = sub_real_key[:num_of_rounds]
real_distance = calculate_distance(sub_real_key_by_rounds)

real_location = 0
# max_key - the candidate for the right sub_key
for key in range(pow(2, num_of_rounds)):
    if key == sub_real_key_by_rounds:
        continue
    curr_dist = calculate_distance(key)
    if curr_dist < real_distance:
        real_location += 1

print(real_location)
#######################################################
#                        Tests
#######################################################

a = [[3, 4, 5], [6, 7, 8], [1, 2, 3]]

for line in range(6):
    for val in range(6):
        print("line : ", line, " val: ", val)
        print(mat[14][0][line][val])
    print('\n')

all_ones = [0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0]
all_zeroes = [1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
              1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1]


def get_sub_input_test_1():
    print(get_sub_input(all_ones))
    print(get_sub_input(all_zeroes))


get_sub_input_test_1()
