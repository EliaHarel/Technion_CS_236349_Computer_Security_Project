import os
import shutil
import time
from Attack_Algorithm_1 import attack_algorithm_1

# user inputs
num_of_rounds = 6
number_of_files = 100
number_of_samples_per_file = 20000
comments_to_file = "without_stav_correction"

start_time = time.time()

def time_end(block_name, start_time, comment = ""):
    print("block \"" + block_name + "\" runtime was " + str(time.time() - start_time) + "seconds." + comment)


output_file_name = "key_locations__" + str(num_of_rounds) + "_rounds__" + str(number_of_files) + "__files_" + str(number_of_samples_per_file) + "__inputs"
data_path = ".\Data\\" + str(number_of_samples_per_file) + "x" + str(number_of_files)

# numbering the output files
ext = ".txt"
f_name_count = 1
temp_name = output_file_name + "__" + str(f_name_count) + ext
path_to_file = ".\Results\\"
while os.path.isfile(path_to_file + temp_name):
    f_name_count += 1
    temp_name = output_file_name + "__" + str(f_name_count) + ext

output_file_name = temp_name
line_sep = "\n"

# opening the file
output_file = open(output_file_name, "w")
# adding comments if exist
if comments_to_file == "":
    output_file.write(comments_to_file + "\n")
    output_file.write(line_sep)

time_end("starting stuff", start_time)

locations_sum = 0
input_count = 0
for input_file in os.scandir(data_path):
    start_time = time.time()
    input_count += 1
    location = attack_algorithm_1(input_file.path)
    # print("Input file name:" + input_file.name + ", Key Location is: .\n")
    # alignment for the printed rows
    if input_file.name[-6] == "_":
        align = " "
    else:
        align = ""
    output_file.write("Input file name: " + input_file.name[:-4] + align + ", Key Location is: " + str(location) + '\n')
    locations_sum += location
    time_end(("input file" + str(input_count)), start_time)

start_time = time.time()
average = locations_sum / input_count
output_file.write(line_sep)
output_file.write("Average Location is " + str(average) + ", out of " + str(input_count) + " samples\n")
runnning_time = "--- Finished in " + str(time.time() - start_time) + "seconds ---"
output_file.write(runnning_time)
output_file.close()
time_end("writing to file", start_time)

shutil.move(".\\" + str(output_file_name), ".\Results\\" + str(output_file_name))

