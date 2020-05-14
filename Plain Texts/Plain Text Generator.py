import random
from pathlib import Path

num_of_plaintexts = 3


new_file_name = str(num_of_plaintexts)+" plains.txt"
Path(new_file_name).touch()

file = open(new_file_name, "w") 


for i in range(num_of_plaintexts):
	plain = ""
	# plain = "{"
	for j in range(8):
		x = random.randint(0,256)
		plain+=str(x)+' '
		# plain+=str(x)+', '
	plain = plain[:-2]
	# plain +="}"
	file.write(plain)
	file.write('\n')

file.close()


