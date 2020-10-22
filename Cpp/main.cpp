

#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <sstream>
#include "Tables.h"
#include "RunningAttackUtil.h"
#include "AttackAlgorithm1.h"
#include "cereal\types\vector.hpp"
#include "cereal\archives\binary.hpp"
#include "cereal/types/memory.hpp"
#include "cereal/archives/xml.hpp"



#define DEBUG false

namespace types {}
using namespace types;

// input: rounds, pairs, iterations, path_to_output_file, key
//input example "6 100 5  \Technion_CS_236349_Computer_Security_Project\Attack_Algorithm_1\Results"
int main(int argc, char* argv[]){

    int rounds, plain_cipher_pairs, iterations;
    std::string file_path, binary_key;
    extractingParams(argc, argv, &rounds, &plain_cipher_pairs, &iterations,
                     file_path, binary_key);


    vvvvd pre_calculated_mat;
    // pre_calculated_mat.emplace_back(vvvd());
    // pre_calculated_mat.emplace_back(vvvd(4, vvd(matrix_size, vd(matrix_size, 0))));
    // for(int i = 2; i <= rounds; i += 2){
    //     pre_calculated_mat.emplace_back(vvvd(pow(2, i), vvd(matrix_size, vd(matrix_size, 0))));
    //     if(i != rounds)
    //         pre_calculated_mat.push_back(vvvd());
    // }
    //
    // CalcTables(rounds, pre_calculated_mat);
    //
    //
    // std::ofstream cereal_out("cereal_data_10_rounds.bin",std::ios::binary); // any stream can be used
    // {
    //     cereal::BinaryOutputArchive oarchive(cereal_out); // Create an output archive
    //
    //     oarchive(pre_calculated_mat); // Write the data to the archive
    // } // archive goes out of scope, ensuring all contents are flushed


    std::ifstream ss;
    ss.open("cereal_data_10_rounds.bin", std::ios::binary);
    {
        cereal::BinaryInputArchive iarchive(ss); // Create an input archive
        iarchive(pre_calculated_mat); // Read the data from the archive
    }


    std::fstream output_file;
    // opens output_file
    initializeOpenOutputFile(output_file, rounds, plain_cipher_pairs, file_path, binary_key);
    attack1(rounds, plain_cipher_pairs, iterations, binary_key, output_file, pre_calculated_mat);


    return 0;
}

