
tests = True


S1_probability = [{ 0 : 0.0625,	 1 : 0.03125,   2 : 0.0625,   3 : 0.0625,     4 : 0.09375,	  5 : 0.09375,	  6 : 0.0625,	 7 : 0,	
                    8 : 0,		 9 : 0.09375,  10 : 0.125,   11 : 0.03125,   12 : 0.0625,	 13 : 0.03125,	 14 : 0.0625,	15 : 0.125 },
                  { 0 : 0.0625,	 1 : 0.09375,   2 : 0.0625,   3 : 0.0625,     4 : 0.03125,	  5 : 0.03125,	  6 : 0.0625,	 7 : 0.125,	
                    8 : 0.125,	 9 : 0.03125,  10 : 0,	     11 : 0.09375,   12 : 0.0625,	 13 : 0.09375,	 14 : 0.0625,	15 : 0 }]
S5_probability = [{ 0 : 0,		 1 : 0.125,	    2 : 0.125,    3 : 0,	      4 : 0.09375,	  5 : 0,		  6 : 0.03125,	 7 : 0.125,
                    8 : 0.0625,	 9 : 0,		   10 : 0.0625,  11 : 0.125,     12 : 0.09375,	 13 : 0.09375,	 14 : 0.0625,	15 : 0 },
                  { 0 : 0.125,	 1 : 0,		    2 : 0,	      3 : 0.125,      4 : 0.03125,	  5 : 0.125,	  6 : 0.09375,	 7 : 0,
                    8 : 0.0625,	 9 : 0.125,	   10 : 0.0625,  11 : 0,	     12 : 0.03125,	 13 : 0.03125,	 14 : 0.0625,	15 : 0.125 }]

size = 256


#  comparing the 4 lsb of the 2 parameters
def equality_right_sides(plain, cipher):
	for i in range(4,8): #the RIGHT bits
		if( plain[i]!=cipher[i]):
			return False
	return True


#  returning the current dictionary according to the inserted sbox value and plain-text
def prob_sbox_bit(plain, s_box):
	if s_box==1 :
		bit_index = 7  
		dictionary_index = S1_probability
	else :
		bit_index = 5
		dictionary_index = S5_probability
	return dictionary_index[int(plain[bit_index])]


#  returns the xor of the two left parts of the input
def left_side_xor(plain, cipher):
	left_P = plain >> 4
	left_C = cipher >> 4
	return left_C^left_P


#  this function 
def prob_calculation(plain, cipher, key, s_box):
	str_plain = format(plain, '08b')
	str_cipher = format(cipher, '08b')
	# print (str_cipher)
	# print (str_plain)

	if not equality_right_sides(str_plain, str_cipher):
		# print ("out")
		return 0
	else:
		return prob_sbox_bit(str_plain, s_box)[left_side_xor(plain, cipher)]


#  Tests
if tests: #all tests are getting into one of the dictionaries, meaning - equality_right_sides returns True. 
	# print(prob_calculation(0, 0, 0, 1))   #output of the F function should be 0000, input bit = 0
	# print(prob_calculation(0, 0, 0, 5))   #output of the F function should be 0000, input bit = 0
	# print(prob_calculation(4, 4, 0, 5))   #output of the F function should be 0000, input bit = 1
	# print(prob_calculation(244, 4, 0, 5)) #output of the F function should be 1111, input bit = 1
	# print(prob_calculation(180, 4, 0, 5)) #output of the F function should be 1011, input bit = 1
	# print(prob_calculation(176, 0, 0, 5)) #output of the F function should be 1011, input bit = 0