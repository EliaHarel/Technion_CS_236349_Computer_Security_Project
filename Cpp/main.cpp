

#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include "cereal/types/vector.hpp"
#include "cereal/archives/binary.hpp"
#include "cereal/types/memory.hpp"
#include "cereal/archives/xml.hpp"
#include "Tables.h"
#include "RunningAttackUtil.h"

#define DEBUG false

namespace types {} // defined in Tables.h
using namespace types;

std::string data_source_prefix = "cereal_data_", data_source_suffix = "_rounds.bin";

void tableCreation(char* argv[]);

// input: rounds, pairs, iterations, path_to_output_file, key
//input examples
// For Attack - "6 100 5  \Technion_CS_236349_Computer_Security_Project\Attack_Algorithm_1\Results"
// For Table Creation - "tables 6" when 6 is the number of wanted rounds rounds
int main(int argc, char* argv[]){

    if( argc == 3 && (std::string) argv[1] == "tables" ){
        tableCreation(argv);
        return 0;
    }

    int rounds, plain_cipher_pairs, iterations;
    std::string file_path, binary_key;
    extractingParams(argc, argv, &rounds, &plain_cipher_pairs, &iterations,
                     file_path, binary_key);

    vvvvd pre_calculated_mat;
    std::ifstream data_source;
    data_source.open(data_source_prefix + std::to_string(rounds) + data_source_suffix, std::ios::binary);
    {
        cereal::BinaryInputArchive iarchive(data_source); // Create an input archive
        iarchive(pre_calculated_mat); // Read the data from the archive
    }

    std::fstream output_file;
    initializeOpenOutputFile(output_file, rounds, plain_cipher_pairs, file_path, binary_key);
    // attack1(rounds, plain_cipher_pairs, iterations, binary_key, output_file, pre_calculated_mat);
    // attack2(rounds, plain_cipher_pairs, iterations, binary_key, output_file, pre_calculated_mat);
    // attack2FewLevels(rounds, plain_cipher_pairs, iterations, binary_key, output_file, pre_calculated_mat);
    std::cout << std::endl;

    return 0;
}


// tables must be built by order 2 --> 4 --> 6 ...
void tableCreation(char* argv[]){
    int rounds = strtol(argv[2], nullptr, 10); // number of rounds

    // checking if the wanted data source already exists
    std::ifstream wanted_data_source(data_source_prefix + std::to_string(rounds) + data_source_suffix,
                                     std::ios::binary);
    if( wanted_data_source.is_open()){
        return;
    }

    int ready_rounds = 2;
    std::ifstream data_source(data_source_prefix + std::to_string(ready_rounds) + data_source_suffix,
                              std::ios::binary);
    vvvvd pre_calculated_mat;
    if( ! data_source.is_open()){
        ready_rounds = 0;
        pre_calculated_mat.emplace_back(vvvd());
        pre_calculated_mat.emplace_back(vvvd(4, vvd(matrix_size, vd(matrix_size, 0))));
        pre_calculated_mat.emplace_back(vvvd(4, vvd(matrix_size, vd(matrix_size, 0))));
    }else{
        bool exists = true;

        while(exists){ // checking what is the highest prepared table
            ready_rounds += 2;
            std::ifstream temp_data_source(
                    data_source_prefix + std::to_string(ready_rounds) + data_source_suffix,
                    std::ios::binary);
            exists = temp_data_source.is_open();
        };
        ready_rounds -= 2;
        std::ifstream temp_data_source(data_source_prefix + std::to_string(ready_rounds) + data_source_suffix,
                                       std::ios::binary);
        {
            cereal::BinaryInputArchive iarchive(temp_data_source); // Create an input archive
            iarchive(pre_calculated_mat); // Read the data from the archive
        }
    }

    for(int i = pre_calculated_mat.size() + 1; i <= rounds; i += 2){
        pre_calculated_mat.push_back(vvvd());
        pre_calculated_mat.emplace_back(vvvd(pow(2, i), vvd(matrix_size, vd(matrix_size, 0))));
    }


    CalcTables(rounds, pre_calculated_mat, ready_rounds);

    std::ofstream cereal_out(data_source_prefix + std::to_string(rounds) + data_source_suffix,
                             std::ios::binary);
    {
        cereal::BinaryOutputArchive oarchive(cereal_out); // Create an output archive

        oarchive(pre_calculated_mat); // Write the data to the archive
    } // archive goes out of scope, ensuring all contents are flushed

    vvvvd pre_calculated_mat_check;
    std::ifstream new_data_source(data_source_prefix + std::to_string(rounds) + data_source_suffix,
                                  std::ios::binary);
    {
        cereal::BinaryInputArchive iarchive(new_data_source); // Create an input archive
        iarchive(pre_calculated_mat_check); // Read the data from the archive
    }

}