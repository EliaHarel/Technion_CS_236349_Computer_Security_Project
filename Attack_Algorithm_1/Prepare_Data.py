import subprocess
import os

plain_file = './../Plain Texts/100_plains.txt'
rounds = "16"
key = "0100100100100100100100001001001100001001001101001000101000100100"
Mode = "1"  # 0 - Decrypt, 1 - Encrypt
# the number of plain texts and cipher texts is determined by the input file

output_file_name = "data.txt"
# c_code_file = "./../DES_C/cmake-build-debug/DES_C.exe"
c_code_file = os.pardir + "\DES_C\cmake-build-debug\DES_C.exe"


plain_object = open(plain_file, "r")
data_object = open(output_file_name, "w")
data_object.write("rounds: " + rounds + " key: " + key + "\n")


for plaintext in plain_object.readlines():
	key = str(key)
	# print(key)
	rounds = str(rounds)
	# print(rounds)
	mode = str(Mode)
	# print(mode)
	plain =str(plaintext)
	# print(plain)
	command = [c_code_file, key, rounds, mode, plain]
	ciphertext = subprocess.check_output(command ,shell=True)
	plain_cipher_line = str(plaintext[:-1]) + " " + str(ciphertext)[2:-1] + '\n'
	data_object.write(plain_cipher_line)

plain_object.close()
data_object.close()

# 0100010010000111001100001110100101101100001010011011110101010111
# 0000101001110011001110000111011110011001100010101011111011010110


# >>> from subprocess import check_output
# >>> foo = check_output('./th', shell=True)
# >>> foo
# '17.9 51.0'