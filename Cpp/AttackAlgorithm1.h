#ifndef TECHNION_CS_236349_COMPUTER_SECURITY_PROJECT_CPP_ATTACKALGORITHM1_H
#define TECHNION_CS_236349_COMPUTER_SECURITY_PROJECT_CPP_ATTACKALGORITHM1_H

#include <string>
#include <vector>
#include "Tables.h"

// int attackAlgorithm1(int num_of_rounds, std::string& binary_used_key, int num_of_inputs);
void attack1(int rounds, int plain_cipher_pairs, int iterations,
             std::string& binary_key, std::fstream& output_file, vvvvd& pre_calculated_mat);

#endif //TECHNION_CS_236349_COMPUTER_SECURITY_PROJECT_CPP_ATTACKALGORITHM1_H
