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


// void extractingParams(int argc, char* argv[], int* attack_num, int* rounds, int* plain_cipher_pairs,
//                       int* iterations, std::string& file_path, std::string& binary_key){
void extractingParams(char* argv[], int* attack_num, int* rounds, int* plain_cipher_pairs,
                      int* iterations, std::string& file_path){
    *attack_num = strtol(argv[1], nullptr, 10);
    *rounds = strtol(argv[2], nullptr, 10); // number of rounds
    *plain_cipher_pairs = strtol(argv[3], nullptr, 10); // number of plaintexts
    *iterations = strtol(argv[4], nullptr, 10);
    file_path = argv[5];

    // if( argc == 7 )
    //     binary_key = argv[6];
    // else
    //     createBinText(binary_key);
}


static inline bool fileExists(const std::string& name){
    struct stat buffer{};
    return (stat(name.c_str(), &buffer) == 0);
}

static std::string findOpenResultPath(int attack_num, int rounds, int plain_cipher_pairs, std::string& path){
    path += separator + "Attack_" + std::to_string(attack_num) + "_xKeyLocations_" + std::to_string(rounds) +
            "xRounds_" +
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
// void initializeOpenOutputFile(std::fstream& output_file, int attack_num, int rounds, int plain_cipher_pairs,
//                               std::string& file_path, std::string& binary_key){
void initializeOpenOutputFile(std::fstream& output_file, int attack_num, int rounds, int plain_cipher_pairs,
                              std::string& file_path){
    std::string output_file_path = findOpenResultPath(attack_num, rounds, plain_cipher_pairs, file_path);
    //creates a file if does not exist
    output_file.open(output_file_path.c_str(), std::fstream::in|std::fstream::out|std::fstream::app);

    // output_file << "Rounds: " << std::to_string(rounds) << ". Key: " << binary_key << std::endl;
    output_file << "Rounds: " << std::to_string(rounds) << "." << std::endl;
}

// void attack(int attackNumber, int rounds, int plain_cipher_pairs, int iterations,
//             std::string& binary_key, std::fstream& output_file, vvvvd& pre_calculated_mat){
void attack(int attackNumber, int rounds, int plain_cipher_pairs, int iterations,
            std::fstream& output_file, vvvvd& pre_calculated_mat){
    double location_sum = 0; //using double for later division
    for(int i = 1; i <= iterations; ++ i){
        int location;
/*        switch (attackNumber){
            case 1:
                location = attackAlgorithm1(rounds, plain_cipher_pairs, binary_key, pre_calculated_mat);
                break;
            case 2:
                location = attackAlgorithm2(rounds, plain_cipher_pairs, binary_key, pre_calculated_mat);
                break;
            case 3:
                location = attackAlgorithm2FewLevels(rounds, plain_cipher_pairs, binary_key,
                                                     pre_calculated_mat);
                break;
            default:
                return;
        }*/
        switch (attackNumber){
            case 1:
                location = attackAlgorithm1(rounds, plain_cipher_pairs, pre_calculated_mat);
                break;
            case 2:
                location = attackAlgorithm2(rounds, plain_cipher_pairs, pre_calculated_mat);
                break;
            case 3:
                location = attackAlgorithm2FewLevels(rounds, plain_cipher_pairs, pre_calculated_mat);
                break;
            default:
                return;
        }
        output_file << "Iteration number: " << i << ". Location is: " << location << std::endl;
        location_sum += location;
    }

    double avg_location = location_sum/iterations;
    output_file << std::endl << "Average Location: " << avg_location << std::endl
                << "Samples :" << iterations << std::endl;

    output_file.close();

}

// void attack1(int rounds, int plain_cipher_pairs, int iterations,
//              std::string& binary_key, std::fstream& output_file, vvvvd& pre_calculated_mat){
void attack1(int rounds, int plain_cipher_pairs, int iterations,
             std::fstream& output_file, vvvvd& pre_calculated_mat){
    output_file << "Attack Algorithm 1" <<
                std::endl;
    attack(1, rounds, plain_cipher_pairs, iterations, output_file, pre_calculated_mat);
    output_file << "Attack Algorithm 1" <<
                std::endl;
}

// void attack2(int rounds, int plain_cipher_pairs, int iterations, std::string& binary_key,
//              std::fstream& output_file, vvvvd& pre_calculated_mat){
void attack2(int rounds, int plain_cipher_pairs, int iterations,
             std::fstream& output_file, vvvvd& pre_calculated_mat){
    output_file << "Attack Algorithm 2" << std::endl;
    attack(2, rounds, plain_cipher_pairs, iterations, output_file, pre_calculated_mat);
    output_file << "Attack Algorithm 2" << std::endl;
}

// void attack2FewLevels(int rounds, int plain_cipher_pairs, int iterations,
//                       std::string& binary_key, std::fstream& output_file, vvvvd& pre_calculated_mat){
void attack2FewLevels(int rounds, int plain_cipher_pairs, int iterations,
                      std::fstream& output_file, vvvvd& pre_calculated_mat){
    output_file << "Attack Algorithm 2 Few Levels" << std::endl;
    attack(3, rounds, plain_cipher_pairs, iterations, output_file, pre_calculated_mat);
    output_file << "Attack Algorithm 2 Few Levels" << std::endl;
}