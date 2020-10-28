//
// Created by EliaHarel on 23/10/2020.
//

#ifndef TECHNION_CS_236349_COMPUTER_SECURITY_PROJECT_CPP_ATTACKALGORITHM2_H
#define TECHNION_CS_236349_COMPUTER_SECURITY_PROJECT_CPP_ATTACKALGORITHM2_H

#include <string>
#include "Tables.h"

namespace types {
    typedef std::vector<int> vi;
    typedef std::vector<vi> vvi;
} // defined in Tables.h


int attackAlgorithm2(int num_of_rounds, int num_of_inputs, vvvvd& pre_calculated_mat);

std::string getSubInput(std::string input, const vi& mask);

std::string intToBinStr(int num, int len);

int sboxFunction(int s_box_num, std::string& binary_input);

int sboxFunction(int s_box_num, int input);

double calculateDistance(int middle_key, int num_of_rounds, int num_of_inputs,
                         int input_matrix[matrix_size][matrix_size], vvvvd& pre_calculated_mat);

#endif //TECHNION_CS_236349_COMPUTER_SECURITY_PROJECT_CPP_ATTACKALGORITHM2_H
