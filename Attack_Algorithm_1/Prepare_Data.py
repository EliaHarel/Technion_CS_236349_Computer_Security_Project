import subprocess
# from subprocess import call
from subprocess import Popen, PIPE

plain_file = './../Plain Texts/10_plains.txt'
c_code_file = "./../DES_C/cmake-build-debug/DES_C.exe"
data_file = "data.txt"
rounds = "16"
key = "0100100100100100100100001001001100001001001101001000101000100100"
Mode = "1"  # 0 - Decrypt, 1 - Encrypt

plain_object = open(plain_file, "r")
data_object = open(data_file, "w")
data_object.write("rounds: " + rounds + " key: " + key + "\n")

# command = ['./serialize', binary, str(width), str(height)]
# stdout, stderr = subprocess.check_output(command)
# print stdout, stderr

for plaintext in plain_object.readlines():

    command = [c_code_file, str(key), str(rounds), str(Mode), str(plaintext[0])]
    # ciphertext = Popen([c_code_file, key, rounds, Mode, plaintext], stdin=PIPE, stdout=PIPE, stderr=PIPE)
    ciphertext = subprocess.check_output(command)
    # print(ciphertext)
    d = str(plaintext[:-1]) + " " + str(ciphertext)[2:-1]
    data_object.write(d)

plain_object.close()
data_object.close()

# print("ffdsfhbsf {} hvj".format(rounds))
