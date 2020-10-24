//
// Created by EliaHarel on 20/10/2020.
//

#ifndef TECHNION_CS_236349_COMPUTER_SECURITY_PROJECT_CPP_RUNNINGATTACKUTIL_H
#define TECHNION_CS_236349_COMPUTER_SECURITY_PROJECT_CPP_RUNNINGATTACKUTIL_H

#include "Tables.h"

void extractingParams(int argc, char* argv[], int* rounds, int* plain_cipher_pairs, int* iterations,
                      std::string& file_path, std::string& binary_key);


void initializeOpenOutputFile(std::fstream& output_file, int rounds, int plain_cipher_pairs,
                             std::string& file_path, std::string& binary_key);

void attack1(int rounds, int plain_cipher_pairs, int iterations,
             std::string& binary_key, std::fstream& output_file, vvvvd& pre_calculated_mat);

void attack2(int rounds, int plain_cipher_pairs, int iterations,
             std::string& binary_key, std::fstream& output_file, vvvvd& pre_calculated_mat);

void attack2FewLevels(int rounds, int plain_cipher_pairs, int iterations,
                     std::string& binary_key, std::fstream& output_file, vvvvd& pre_calculated_mat);

#endif //TECHNION_CS_236349_COMPUTER_SECURITY_PROJECT_CPP_RUNNINGATTACKUTIL_H
