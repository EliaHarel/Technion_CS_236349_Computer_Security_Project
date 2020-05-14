import subprocess

plain_file = ""
c_code_file = "/file_for_testing.c"
data_file = "data.txt"
rounds = 4  #
key = '011'


plain = [111, 123, 235, 343]
# plain_object = open(plain_file, "r")
data_object = open(data_file, "w")
data_object.write("rounds: " + str(rounds) + " key: " + key + "\n")
# for plaintext in plain_object.readline():
for plaintext in plain:
    subprocess.call(["g++", c_code_file])
    # ciphertext = subprocess.call("./a.out", plaintext, key)
    ciphertext = subprocess.call("./a.out")
    data_object.write(plain_file + " " + str(ciphertext))

# plain_object.close()
data_object.close()

# print("ffdsfhbsf {} hvj".format(rounds))
