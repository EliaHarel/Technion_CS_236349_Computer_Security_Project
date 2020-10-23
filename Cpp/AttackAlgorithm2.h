//
// Created by EliaHarel on 23/10/2020.
//

#ifndef TECHNION_CS_236349_COMPUTER_SECURITY_PROJECT_CPP_ATTACKALGORITHM2_H
#define TECHNION_CS_236349_COMPUTER_SECURITY_PROJECT_CPP_ATTACKALGORITHM2_H

#include <string>
#include "Tables.h"
// namespace types{}
// using namespace types;

namespace types {
    typedef std::vector<int> vi;
    typedef std::vector<vi> vvi;
} // defined in Tables.husing namespace types;



int attackAlgorithm2(int num_of_rounds, int num_of_inputs, std::string& binary_used_key,
                     vvvvd& pre_calculated_mat);
std::string getSubInput(std::string input, const vi& mask);
std::string intToBinStr(int num, int len);
int sboxFunction(int s_box_num, std::string& binary_input);
double calculateDistance(int middle_key, int num_of_rounds, int num_of_inputs,
                         vvd& input_matrix, vvvvd& pre_calculated_mat);

#endif //TECHNION_CS_236349_COMPUTER_SECURITY_PROJECT_CPP_ATTACKALGORITHM2_H
