import os
from Attack_Algorithm_1 import attack_algorithm_1

num_of_rounds = 6
output_file_name = "key_locations_20000x100.txt"
data_path = '.\Data\\20000x100'

# directory_in_str = "C:\E.R\Programming\Git\GitHub\Technion_CS_236349_Computer_Security_Project\Attack_Algorithm_1\Data"
output_file = open(output_file_name, "w")

for input_file in os.scandir(data_path):
    location = attack_algorithm_1(input_file.path)
    # print("Input file name:" + input_file.name + ", Key Location is: .\n")
    output_file.write("Input file name:" + input_file.name + ", Key Location is: " + str(location) + '\n')

output_file.close()





