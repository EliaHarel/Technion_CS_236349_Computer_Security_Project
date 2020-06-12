# guess the first and the last round
# use algorithm 1 with 14 rounds
# calculate the matrices

# out s5 after permutation - 3, 8, 14, 25
# out s1 after permutation - 9, 17, 23, 31
# in s5 - 16, 17, 18, 19, 20, 21
# in s1 - 32, 1, 2, 3, 4, 5

# sub plain L - out s1
# sub plain R - out s5 || in s1
# sub cipher L - out s5
# sub cipher R - out s1 || in s5
# c - sub plain R || sub plain L || sub cipher R || sub cipher L

plain_L_mask = [ 8, 16, 22, 30]
plain_R_mask = [  2, 7, 13, 24 , 31, 0, 1, 2, 3, 4]
cipher_L_mask = [ 2, 7, 13, 24]
cipher_R_mask = [8, 16, 22, 30, 15, 16, 17, 18, 19, 20]

k1_s1_mask = [10, 51, 34, 60, 49, 17]
k16_s5_mask = [30, 5, 47, 62, 45, 12]
first_last_mask = k1_s1_mask + k16_s5_mask
key_mask_14_mid_rounds = [52, 4, 49, 39, 17, 7, 50, 46, 26, 14, 59, 45, 27, 13]  # 49, 45, 17 - repeated keys


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

        sub_plain_R = get_sub_input(binary_plain_R, plain_R_mask)
        sub_plain_L = get_sub_input(binary_plain_L, plain_L_mask)
        sub_cipher_R = get_sub_input(binary_cipher_R, cipher_R_mask)
        sub_cipher_L = get_sub_input(binary_cipher_L, cipher_L_mask)

        sub_plain = sub_plain_L + sub_plain_R
        sub_cipher = sub_cipher_L + sub_cipher_R
        sub_input = sub_plain + sub_cipher

        yield int(sub_input,2)
        data_line = file_object.readline()

# returns the output of the s_box for the given s_box_num and binary_input 
def s_box_function(s_box_num, binary_input):
	row = int(binary_input[0] + binary_input[5] ,2)
	col = int(binary[1:5],2)
	if s_box_num ==1:
		s_box = s1
	else:
		s_box = s5
	return format(s_box[row][col], '04b')

def calculate_P_C_from_key_combination (key, combination):
	binary_key = format(key, '012b')
	k1 = binary_key[:6]
	k16 = binary_key[6:]

	# combination - p out s1 (4)|| p out s5 (4) || p in s1 (6)|| c out s5 (4)|| c out s1 (4)|| c in s5 (6)
	# combunation -     0-3     ||     4-7      ||    8-13    ||    14-17    ||    18-21    ||   22-27
	binary_combination = format(combination, '028b')
	plain_L = combination[4:8]
	cipher_L = combination[18:22] 

	binary_input1 = combination[8:14]
	binary_input16 = combination[22:]

	input_to_s1 = binary_input1 ^ k1
	input_to_s5 = binary_input16 ^ k16

	# p_out_s1 ^ (S1(k1^p_in_s1) = p_out_s1 ^ S1(input_to_s1)
	plain_R = combination[:4] ^ s_box_function(1, input_to_s1)

	# c_out_s5 ^ P(S5(k16^c_in_s5)) = c_out_s5 ^ S5(input_to_s5)
	cipher_R = combination[14:18] ^ s_box_function(5, input_to_s5)

	plaintext = plain_L + plain_R
	ciphertext = cipher_L + cipher_R

	return plaintext, ciphertext

def calculate_distance(first_last_key, middle_key, num_of_rounds, num_of_inputs):
    distance = 0
    for i in range(matrix_size):
        for j in range(matrix_size):
            part_1 = matrix[first_last_key][i][j] - num_of_inputs / pow(2, 8)
            part_2 = mat[num_of_rounds-2][middle_key][i][j] * num_of_inputs - num_of_inputs / pow(2, 8)
            distance += part_1 * part_2
    return abs(distance)


# num_of_rounds includes the first and last round
def attack_algorithm_2 (input file, num_of_rounds):

	file_object = open(file_name, "r")
	first_line = file_object.readline().split()
	real_rounds = first_line[1]
	real_key = first_line[3]
	num_of_inputs = 0

	counter = [0 for i in range(pow(2, 28))]

	# plain - 14 bits - out s1 || out s5 || in s1
	# cipher - 14 bits -  out s5 || out s1 || in s5
	# index (int)- plain || cipher
	for index in get_next_input_from_file(file_object):
		counter[index] += 1
		num_of_inputs +=1

	#matrix [key][plain][cipher], key - guessed key for 12 key bits of the first and last rounds
	matrix = [[[0 for in in range (pow(2,8))] for i in range (pow(2,8))] for i in range(pow(2,12))]

	for key in pow(2,12):
		for combination in pow(2,28):
			plain, cipher = calculate_P_C_from_key_combination (key, combination)
			matrix[key][plain][cipher] += counter[combination]


	real location = 1
	curr_first_last_key = get_sub_input (real_key, first_last_mask)
	curr_middle_key = get_sub_input(real_key, key_mask_14_mid_rounds)
	curr_middle_key_by_rounds = curr_middle_key[num_of_rounds-2]
	
	# num of inputs ???

	# calculate real key distance
	real_distance = calculate_distance(curr_first_last_key, curr_middle_key_by_rounds, num_of_rounds, num_of_inputs)

	for first_last_key in range(pow(2, 12)):
	    for middle_key in range(pow(2, num_of_rounds-2)):
	    	if first_last_key == curr_first_last_key && middle_key==curr_middle_key_by_rounds:
	    		continue
    		curr_dist = calculate_distance(first_last_key, middle_key, num_of_rounds, num_of_inputs )
    		if curr_dist < real_distance:
        		real_location += 1

    return real_location

