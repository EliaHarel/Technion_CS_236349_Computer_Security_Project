import sys
import random
import subprocess
import os
import pathlib
from pathlib import Path
import platform

rounds = "6"
num_of_files = 100
num_of_plain_cipher_pairs = 500

arguments_count = len(sys.argv)
if arguments_count != 1 and arguments_count != 3:
    exit()

if arguments_count == 3:
    rounds = sys.argv[1]
    num_of_files = sys.argv[2]
    num_of_plain_cipher_pairs = sys.argv[3]

# the number of plain texts and cipher texts is determined by the input file
plain_cipher_pairs_dir_name = "Plain_Cipher_Pairs"
c_file_path = pathlib.Path().absolute().__str__() + os.sep
windows_c_file_path = c_file_path + "DES_Cpp" + os.sep + "cmake-build-debug" + os.sep + "DES_Cpp.exe"
linux_c_file_path = c_file_path + "DES_Cpp/DES_CPP"


# cur_dir_path = cur_dir_path.resolve()
# c_file_path = pathlib.Path().absolute() + os.sep + "DES_Cpp" + os.sep + "cmake-build-debug" + os.sep + "DES_C.exe"

def create_key():
    key = ""
    for j in range(8):
        x = random.randint(0, 256)
        x = format(x, '08b')
        key += str(x)
    return key


def create_new_directory(dir_name):
    f_name_count = 0
    new_dir_name = dir_name + "__v" + str(f_name_count)
    while os.path.isdir(new_dir_name):
        f_name_count += 1
        new_dir_name = dir_name + "__v" + str(f_name_count)
    os.mkdir(new_dir_name)
    return new_dir_name


# creating folders for the output files, moving into the destination dir
os.chdir(plain_cipher_pairs_dir_name)

rounds_dir_name = str(rounds) + "_rounds"
if not os.path.isdir(rounds_dir_name):
    os.mkdir(rounds_dir_name)
os.chdir(rounds_dir_name)

pairs_dir_name = str(num_of_plain_cipher_pairs) + "_pairs"
pairs_dir_name = create_new_directory(pairs_dir_name)
os.chdir(pairs_dir_name)

key = create_key()

counter = 0
for i in range(num_of_files):
    output_file_path = str(counter) + ".txt"
    counter += 1

    str_key = str(key)
    str_rounds = str(rounds)
    str_pairs_num = str(num_of_plain_cipher_pairs)

    cur_OS = platform.system()
    if (cur_OS == "Windows"):
        command = [windows_c_file_path, str_rounds, str_pairs_num, str_key, output_file_path]
    elif (cur_OS == "Linux"):
        command = linux_c_file_path + ' ' + str_rounds + ' ' + str_pairs_num + ' ' + str_key + ' ' + output_file_path
    ciphertext = subprocess.check_output(command, shell=True)



