//
// Created by EliaHarel on 20/10/2020.
//

#include <cstdlib>
#include <string>
#include "RunningAttackUtil.h"
#include "Data.h"
#include <sys/stat.h> //do not remove


#ifdef __linux__
std::string separator = "/";
  #include <unistd.h>
  #define GetCurrentDir getcwd
#elif _WIN32
std::string separator = "\\";

  #include <direct.h>
#include <fstream>

  #define GetCurrentDir _wgetcwd
#else
#endif


void extractingParams(int argc, char* argv[], int* rounds, int* plain_cipher_pairs, int* iterations,
                      std::string& file_path, std::string& binary_key){
    *rounds = strtol(argv[1], nullptr, 10); // number of rounds
    *plain_cipher_pairs = strtol(argv[2], nullptr, 10); // number of plaintexts
    *iterations = strtol(argv[3], nullptr, 10);
    file_path = argv[4];

    if(argc == 6)
        binary_key = argv[5];
    else
        createBinText(binary_key);
}


static inline bool fileExists(const std::string& name){
    struct stat buffer{};
    return (stat(name.c_str(), &buffer) == 0);
}

static std::string findOpenResultPath(int rounds, int plain_cipher_pairs, std::string& path){
    path += separator + "KeyLocations_" + std::to_string(rounds) + "xRounds_" +
            std::to_string(plain_cipher_pairs) +
            "xPairs__v";

    int counter = 0;
    std::string temp_path = path + std::to_string(counter++) + ".txt";
    while(fileExists(temp_path)){
        temp_path = path + std::to_string(counter++) + ".txt";
    }
    return temp_path;
}


// This function returns an open file, caller's responsibility to close it
void initializeOpenOutputFile(std::fstream& output_file, int rounds, int plain_cipher_pairs,
                              std::string& file_path, std::string& binary_key){
    std::string output_file_path = findOpenResultPath(rounds, plain_cipher_pairs, file_path);
    //creates a file if does not exist
    output_file.open(output_file_path.c_str(), std::fstream::in|std::fstream::out|std::fstream::app);
    output_file << "Rounds: " << std::to_string(rounds) << ". Key: " << binary_key << std::endl;
}