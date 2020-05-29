# guess the first and the last round
# use algorithm 1 with 14 rounds
# calculate the matrices

# out s5 after permutation - 3, 8, 14, 25
# out s1 after permutation - 9, 17, 23, 31


plaintext = ""
ciphertext = ""
k1_s1_mask = [10, 51, 34, 60, 49, 17]
k16_s5_mask = [30, 5, 47, 62, 45, 12]
key_mask_14_mid_rounds = [52, 4, 49, 39, 17, 7, 50, 46, 26, 14, 59, 45, 27, 13]  # 49, 45, 17 - repeated keys

for plaintext, ciphertext in file:
    for first_key in range(pow(2, 6)):
        binary_first_key = format(first_key, '06b')
        temp_first_key = " "  # first_key in the bits of S1 in K1
        temp_cipher = " "  # DES - ENC, round =1, temp_first_key, plaintext
        left_side = " "  # cutting plaintext by out-S5 bits (right side)
        right_side = " "  # cutting temp _cipher by out-S1 bits , check about swapping
        sub_plain_after_first_round = left_side + right_side

        for last_key in range(pow(2, 6)):
            binary_last_key = format(first_key, '06b')
            temp_last_key = ""  # first_key in the bits of S5 in K16
            temp_plain = " "  # DES - DEC, round =1, temp_last_key, ciphertext
            left_side = " "  # cutting temp_plain by out-S1 bits, check about swapping
            right_side = " "  # cutting ciphertext by out-S5 bits (right side)
            sub_cipher_before_last_round = left_side + right_side

            #  update for each (first_key, last_key) the summing matrix according to 14 mid-rounds-key options -

# calculate probability matrix for each (first_key, last_key) pair
# calculate the real combination of first_key_subkey and 14-levels-key
# calculate the distance between the relevant matrices of the above combination - real distance

for i in range(pow(2, 12)):
    for j in range(pow(2, 14)):
        # calculate distance between the probability matrices of 14 levels to the matrices using (first key, last_key)
        # update the counter if the distance is smaller than the real distance
