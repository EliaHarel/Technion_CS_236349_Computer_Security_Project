

#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include "Attack_1.h"
#include "Data.h"
#include <sys/stat.h> //do not remove


#ifdef __linux__
std::string seperator = "/";
  #include <unistd.h>
  #define GetCurrentDir getcwd
#elif _WIN32
std::string seperator = "\\";

  #include <direct.h>

  #define GetCurrentDir _wgetcwd
#else
#endif

#define TEXT_LENGTH 64
#define DEBUG false


std::string result_path(int rounds, int plain_cipher_pairs, std::string& path);

inline bool file_exists(const std::string& name){
    struct stat buffer{};
    return (stat(name.c_str(), &buffer) == 0);
}

// input: rounds, pairs, iterations, path_to_output_file, key
//input example "6 100 5  \Technion_CS_236349_Computer_Security_Project\Attack_Algorithm_1\Results"
int main(int argc, char* argv[]){

    int rounds = strtol(argv[1], nullptr, 10); // number of rounds
    int plain_cipher_pairs = strtol(argv[2], nullptr, 10); // number of plaintexts
    int iterations = strtol(argv[3], nullptr, 10);
    std::string file_path = argv[4];


    std::string binary_key;
    if(argc == 6)
        binary_key = argv[5];
    else
        createBinText(binary_key);


    std::fstream output_file;
    std::string output_file_path = result_path(rounds, plain_cipher_pairs, file_path);
    //creates a file if does not exist
    output_file.open(output_file_path.c_str(), std::fstream::in|std::fstream::out|std::fstream::app);
    output_file << "Rounds: " << std::to_string(rounds) << ". Key: " << binary_key << std::endl;

    double location_sum = 0;
    for(int i = 0; i < iterations; ++i){
        int location = attackAlgorithm1(rounds, binary_key, plain_cipher_pairs);
        output_file << std::endl << "Iteration number: " << i << ". Location is: " << location << std::endl;
        location_sum += location;
    }

    double avg_location = location_sum/iterations;
    output_file << std::endl << "Average Location is: " << avg_location << "out of " << iterations
                << " samples." << std::endl;

    output_file.close();
    return 0;
}

std::string result_path(int rounds, int plain_cipher_pairs, std::string& path){
    path += seperator + "KeyLocations_" + std::to_string(rounds) + "xRounds_" +
            std::to_string(plain_cipher_pairs) +
            "xPairs__v";

    int counter = 0;
    std::string temp_path = path + std::to_string(counter++) + ".txt";
    while(file_exists(temp_path)){
        temp_path = path + std::to_string(counter++) + ".txt";
    }
    return temp_path;
}

