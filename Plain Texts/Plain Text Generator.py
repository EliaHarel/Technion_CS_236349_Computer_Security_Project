import random
from pathlib import Path

num_of_plaintexts = 10


new_file_name = str(num_of_plaintexts)+"_plains.txt"
Path(new_file_name).touch()

file = open(new_file_name, "w") 


for i in range(num_of_plaintexts):
	plain = ""
	for j in range(8):
		x = random.randint(0,256)
		x = format(x, '08b')
		plain+=str(x)
	# plain = plain[:-2]
	file.write(plain)
	file.write('\n')

file.close()







