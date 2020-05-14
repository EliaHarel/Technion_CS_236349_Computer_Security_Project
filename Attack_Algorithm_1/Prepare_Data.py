import subprocess

plain_file = ""
c_code_file = "/file_for_testing.c"
data_file = "data.txt"
rounds = 4  #
key = '011'
########
from subprocess import call
#######

plain = [111, 123, 235, 343]
# plain_object = open(plain_file, "r")
data_object = open(data_file, "w")
data_object.write("rounds: " + str(rounds) + " key: " + key + "\n")
# for plaintext in plain_object.readline():
for plaintext in plain:
    # subprocess.call(["gcc", "./file_for_testing.c"])
    # ciphertext = subprocess.call("./a.out", plaintext, key)
    # ciphertext = subprocess.call("./a.out")

    key = "0100100100100100100100001001001100001001001101001000101000100100"
    rounds = "16"
    decrypt_or_encrypt = "1"
    text = "0100100100100100100100100100100100100100100100100100100100100100"
    call(["./../DES_C/cmake-build-debug/DES_C.exe", key, rounds, decrypt_or_encrypt, text])

    # data_object.write(plain_file + " " + str(ciphertext))

# plain_object.close()
data_object.close()

# print("ffdsfhbsf {} hvj".format(rounds))
