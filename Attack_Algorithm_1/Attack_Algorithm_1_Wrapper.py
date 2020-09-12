import os
import shutil
import time
from pathlib import Path
from Attack_Algorithm_1 import attack_algorithm_1

###############################
#         user inputs         #
###############################
num_of_rounds = 6
num_of_pairs = 5000
v_num = 0
comments_to_file = ""

##############################


start_time = time.time()

def time_end(block_name, start_time, comment=""):
    print("block \"" + block_name + "\" runtime was " + str(time.time() - start_time) + "seconds." + comment)



output_file_name = "KeyLocations_" + str(num_of_rounds) + "xRounds_" + str(num_of_pairs) + "xFiles__v" + str(v_num)
data_path = os.pardir + os.sep + "Data" + os.sep + "Plain_Cipher_Pairs" + os.sep + str(num_of_rounds) + "_rounds" + \
            os.sep + str(num_of_pairs) + "_pairs__v"+ str(v_num)

# numbering the output files
f_name_count = 1
temp_name = output_file_name + "_" + str(f_name_count) + ".txt"
path_to_file = os.curdir + os.sep + "Results"

while os.path.isfile(path_to_file + os.sep + temp_name):
    f_name_count += 1
    temp_name = output_file_name + "_" + str(f_name_count) + ".txt"

output_file_name = output_file_name + "_" + str(f_name_count) + ".txt"

# opening the file
output_file = open(output_file_name, "w")
# adding comments if exist
if comments_to_file == "":
    output_file.write(comments_to_file + "\n")

time_end("starting stuff", start_time)

locations_sum = 0
input_count = 0
for input_file in os.scandir(data_path):
    start_time = time.time()
    input_count += 1
    location = attack_algorithm_1(input_file.path, num_of_rounds)
    # print("Input file name:" + input_file.name + ", Key Location is: .\n")
    # alignment for the printed rows
    align = ""
    # for i in range(num_of_pairs-) == "_":
    #     align = " "
    # else:
    #     align = ""
    # output_file.write("Input file name: " + input_file.name[:-4] + align + ", Key Location is: " + str(location) + '\n')
    output_file.write("Input file name: " + input_file.name[:-4] + ", Key Location is: " + str(location) + '\n')
    locations_sum += location
    time_end(("input file" + str(input_count)), start_time)

start_time = time.time()
average = locations_sum / input_count
output_file.write('\n')
output_file.write("Average Location is " + str(average) + ", out of " + str(input_count) + " samples\n")
runnning_time = "--- Finished in " + str(time.time() - start_time) + "seconds ---"
output_file.write(runnning_time)
output_file.close()
time_end("writing to file", start_time)

shutil.move(".\\" + str(output_file_name), ".\Results\\" + str(output_file_name))





