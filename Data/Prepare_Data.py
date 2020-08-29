import random
import subprocess
import os
import pathlib
from pathlib import Path

num_of_plain_cipher_pairs = 10
num_of_files = 5
rounds = "6"

# the number of plain texts and cipher texts is determined by the input file
plaintexts_dir_name = "Plain_Texts" + os.sep
plain_cipher_pairs_dir_name = "Plain_Cipher_Pairs"
os.chdir(plaintexts_dir_name)

def create_key():
	key = ""
	for j in range(8):
		x = random.randint(0,256)
		x = format(x, '08b')
		key+=str(x)
	return key

def create_new_directory(dir_name):
	f_name_count = 0
	new_dir_name = dir_name + "__v" + str(f_name_count)
	while os.path.isdir(new_dir_name):
		f_name_count += 1
		new_dir_name = dir_name + "__v" + str(f_name_count)
	os.mkdir(new_dir_name)
	return new_dir_name

def create_new_plaintexts_files(new_file_name, num_of_plaintexts):
	Path(new_file_name).touch()
	file = open(new_file_name, "w") 
	
	for i in range(num_of_plaintexts):
		plain = ""
		for j in range(8):
			x = random.randint(0,256)
			x = format(x, '08b')
			plain+=str(x)
		file.write(plain)
		file.write('\n')
	file.close()


# create directory for plaintexts
directory_name = str(num_of_plain_cipher_pairs) + "_plaintexts__" + str(num_of_files) + "_files"
new_dir_name = create_new_directory(directory_name)
os.chdir(new_dir_name)
new_plaintexts_dir_path = pathlib.Path().absolute()

# create plaintexts files
for file_index in range(num_of_files):
	new_file_name = str(num_of_plain_cipher_pairs) + "_plains_" + str(file_index) + ".txt"
	create_new_plaintexts_files(new_file_name ,num_of_plain_cipher_pairs)

# creating folders for the output files, moving into the destination dir
os.chdir(os.pardir + os.sep + os.pardir + os.sep + plain_cipher_pairs_dir_name)

rounds_dir_name = str(rounds) + "_rounds"
if not os.path.isdir(rounds_dir_name):
	os.mkdir(rounds_dir_name)
os.chdir(rounds_dir_name)

pairs_dir_name = str(num_of_plain_cipher_pairs) + "_pairs"
if not os.path.isdir(pairs_dir_name):
	os.mkdir(pairs_dir_name)
os.chdir(pairs_dir_name)


# file_path_prefix = "." + os.sep + ".." + os.sep + "Plain Texts" + file_prefix
# output_path_prefix = "Data_" + file_prefix

c_code_file = os.pardir + "\DES_C\cmake-build-debug\DES_C.exe"

key = create_key()

counter = 0
for files_index in os.scandir(new_plaintexts_dir_path):
	# file_path = file_path_prefix + str(files_index) + ".txt"

	output_path = str(counter) + ".txt"
	counter += 1

	input_file = open(files_index.path, "r")
	output_file = open(output_path, "w")
	output_file.write("rounds: " + rounds + " key: " + key + "\n")


	for plaintext in input_file.readlines():
		str_key = str(key)
		str_rounds = str(rounds)
		str_plain = str(plaintext)[:-1]
		command = [c_code_file, str_key, str_rounds, '1', str_plain] # '1' is for encryption, '0' is for decryption. but decryption mode does not fit this script
		ciphertext = subprocess.check_output(command ,shell=True)
		plain_cipher_line = str(plaintext[:-1]) + " " + str(ciphertext)[2:-1] + '\n'
		output_file.write(plain_cipher_line)
	
	input_file.close()
	output_file.close()



