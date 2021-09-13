#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include "cereal/types/vector.hpp"
#include "cereal/archives/binary.hpp"
#include "cereal/types/memory.hpp"
#include "cereal/archives/xml.hpp"

#ifdef __linux__
extern std::string separator;
std::string separator = "/";

#include <unistd.h>

#define GetCurrentDir getcwd
#elif _WIN32
extern std::string separator;
std::string separator = "\\";

#include <direct.h>
#include <fstream>

#define GetCurrentDir _wgetcwd
#else
#endif

#include "Tables.h"
#include "RunningAttackUtil.h"

namespace types {} // defined in Tables.h
using namespace types;

std::string data_source_prefix = "cereal_data_", data_source_suffix = "_rounds.bin";

void tableCreation(char* argv[]);

// input: attack number, number of rounds, number of pairs, number of iterations, absolute path to output file directory
// input examples
// For Attack - "1 6 100 5  \Technion_CS_236349_Computer_Security_Project\Attack_Algorithm_1\Results"
// For Table Creation - "tables 6" when 6 is the number of wanted rounds rounds
int main(int argc, char* argv[]){
    if(argc == 3 && (std::string) argv[1] == "tables"){
        tableCreation(argv);
        return 0;
    }

    srand(time(nullptr));
    int rounds, plain_cipher_pairs, iterations, attack_num;
    std::string file_path;
    extractingParams(argv, &attack_num, &rounds, &plain_cipher_pairs, &iterations, file_path);

    std::string rounds_from_data(std::to_string(rounds));
    if(attack_num == 2 || attack_num == 3){
        rounds_from_data = std::to_string(rounds - 2);
    }
    vvvvd pre_calculated_mat;
    std::ifstream data_source;

    data_source.open(".." + separator + data_source_prefix + rounds_from_data + data_source_suffix,
                     std::ios::binary);
    {
        cereal::BinaryInputArchive iarchive(data_source); // Create an input archive
        iarchive(pre_calculated_mat); // Read the data from the archive
    }

    std::fstream output_file;
    initializeOpenOutputFile(output_file, attack_num, rounds, plain_cipher_pairs, file_path);

    switch (attack_num){
        case 1:
            attack1(rounds, plain_cipher_pairs, iterations, output_file, pre_calculated_mat);
            break;
        case 2:
            attack2(rounds, plain_cipher_pairs, iterations, output_file, pre_calculated_mat);
            break;
        case 3:
            attack2FewLevels(rounds, plain_cipher_pairs, iterations, output_file, pre_calculated_mat);
            break;
        default:
            return -1;
    }
    std::cout << std::endl;
    return 0;
}

// tables must be built by order 2 --> 4 --> 6 ...
void tableCreation(char* argv[]){
    int rounds = strtol(argv[2], nullptr, 10); // number of rounds

    vvvvd pre_calculated_mat;
        pre_calculated_mat.emplace_back(vvvd());
        pre_calculated_mat.emplace_back(vvvd(4, vvd(matrix_size, vd(matrix_size, 0))));
        pre_calculated_mat.emplace_back(vvvd(4, vvd(matrix_size, vd(matrix_size, 0))));


    for(int i = pre_calculated_mat.size() + 1; i <= rounds; i += 2){
        pre_calculated_mat.push_back(vvvd());
        pre_calculated_mat.emplace_back(vvvd(pow(2, ROUNDS_OF_SHORTEN_TABLE), vvd(matrix_size, vd(matrix_size, 0))));
    }


    Calc16Tables(rounds, pre_calculated_mat);

}