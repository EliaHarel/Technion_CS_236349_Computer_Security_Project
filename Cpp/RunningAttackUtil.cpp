//
// Created by EliaHarel on 20/10/2020.
//

#include <cstdlib>
#include <string>
#include <fstream>
#include "RunningAttackUtil.h"
#include "DataUtils.h"
#include "Tables.h"
#include "AttackAlgorithm1.h"
#include "AttackAlgorithm2.h"
#include <sys/stat.h> //do not remove

#include "AttackAlgorithm2FewLevels.h"

using namespace types;

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

    if( argc == 6 )
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
    std::string temp_path = path + std::to_string(counter ++) + ".txt";
    while(fileExists(temp_path)){
        temp_path = path + std::to_string(counter ++) + ".txt";
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

void attack(double attackNumber, int rounds, int plain_cipher_pairs, int iterations,
            std::string& binary_key, std::fstream& output_file, vvvvd& pre_calculated_mat){
    double location_sum = 0;
    for(int i = 0; i < iterations; ++ i){
        int location;
        if (attackNumber == 1)
            location = attackAlgorithm1(rounds, binary_key, plain_cipher_pairs, pre_calculated_mat);
        else if(attackNumber == 2)
            location = attackAlgorithm2(rounds, binary_key, plain_cipher_pairs, pre_calculated_mat);
        else if(attackNumber == 2.5)
        output_file << "Iteration number: " << i << ". Location is: " << location << std::endl;
        location_sum += location;
    }

    double avg_location = location_sum/iterations;
    output_file << std::endl << "Average Location: " << avg_location << std::endl
                << "Samples :" << iterations << std::endl;

    output_file.close();

}

// void attack2
void attack1(int rounds, int plain_cipher_pairs, int iterations,
             std::string& binary_key, std::fstream& output_file, vvvvd& pre_calculated_mat){


}