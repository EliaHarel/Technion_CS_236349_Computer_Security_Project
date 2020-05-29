import random
from pathlib import Path

num_of_plaintexts = 2000
num_of_files = 20
for file_index in range(num_of_files):
	new_file_name = str(num_of_plaintexts)+"_plains_"+str(file_index)+".txt"
	
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







