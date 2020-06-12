import random
import subprocess
import os


file_prefix = "20000_plains_"
num_of_files = 20
rounds = "6"
Mode = "1"  # 0 - Decrypt, 1 - Encrypt
# the number of plain texts and cipher texts is determined by the input file

def create_key():
	key = ""
	for j in range(8):
		x = random.randint(0,256)
		x = format(x, '08b')
		key+=str(x)
	return key

key = create_key()

file_path_prefix = "./../Plain Texts/" + file_prefix
output_path_prefix = "Data_" + file_prefix

c_code_file = os.pardir + "\DES_C\cmake-build-debug\DES_C.exe"

for files_index in range(20,100):
	file_path = file_path_prefix + str(files_index) + ".txt"
	output_path = output_path_prefix + str(files_index) + ".txt"
	input_file = open(file_path, "r")
	output_file = open(output_path, "w")
	output_file.write("rounds: " + rounds + " key: " + key + "\n")


	for plaintext in input_file.readlines():
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
		output_file.write(plain_cipher_line)
	
	input_file.close()
	output_file.close()
