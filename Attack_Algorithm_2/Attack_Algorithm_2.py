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
key_mask_14_mid_rounds = [52, 4, 49, 39, 17, 7, 50, 46, 26, 14, 59, 45, 27, 13]  # 49, 45, 17 - repeated keys

# reads the next pair of inputs and return the relevant bits from them as integers
# the relevant bits are the outputs bits of S1, S5


# returns a substring which contains bits from specific places in str
def get_sub_input(str_input, mask):
    sub_str = ''
    for i in range(len(mask)):
        sub_str += str_input[mask[i]]
    return sub_str


# TODO: update the function!! the masks are different!!
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

#6 bits binary num
def s_box_function(s_box, num):
	row = int(num[0] + num[5] ,2)

def s1_function(num):


def s5_function(num):


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
	plain_R = combination[:4] ^ s1_function(int(input_to_s1,2))

	# c_out_s5 ^ P(S5(k16^c_in_s5)) = c_out_s5 ^ S5(input_to_s5)
	cipher_R = combination[14:18] ^ s5_function(int(input_to_s5,2))


def attack_algorithm_2 (input file):

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


	#matrix [key][plain][cipher], key - guessed key for 12 key bits of the first and last rounds
	matrix = [[[0 for in in range (pow(2,8))] for i in range (pow(2,8))] for i in range(pow(2,12))]

	for key in pow(2,12):
		for combination in pow(2,28):
			plain, cipher = calculate_P_C_from_key_combination (key, combination)
			matrix[key][plain][cipher] += counter[combination]
			# calculate plain, cipher from key, combination
			# matrix[key][plain][cipher] += counter [combination]



	for i in range(pow(2, 12)):
	    for j in range(pow(2, 14)):
	        # calculate distance between the probability matrices of 14 levels to the matrices using (first key, last_key)
	        # update the counter if the distance is smaller than the real distance
