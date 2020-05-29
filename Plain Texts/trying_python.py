import os
directory_in_str = "C:\E.R\Programming\Git\GitHub\Technion_CS_236349_Computer_Security_Project\Attack_Algorithm_1\Data"
for entry in os.scandir(directory_in_str):
    print(entry.path)

# for filename in os.listdir(directory_in_str):
#     print(filename)
#     if filename.endswith(".txt") or filename.endswith(".py"):
#         print("hey")
#         # print(os.path.join(directory_in_str, filename))
        
#     else:
#         print("not hey")
