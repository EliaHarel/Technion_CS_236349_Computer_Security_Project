//
// Created by EliaHarel on 20/10/2020.
//

#ifndef TECHNION_CS_236349_COMPUTER_SECURITY_PROJECT_CPP_RUNNINGATTACKUTIL_H
#define TECHNION_CS_236349_COMPUTER_SECURITY_PROJECT_CPP_RUNNINGATTACKUTIL_H

#include "Tables.h"

void extractingParams(char* argv[], int* attack_num, int* rounds, int* plain_cipher_pairs,
                      int* iterations, std::string& file_path);
// void extractingParams(int argc, char* argv[], int* attack_num, int* rounds, int* plain_cipher_pairs,
//                       int* iterations, std::string& file_path, std::string& binary_key);



/*
void initializeOpenOutputFile(std::fstream& output_file,int attack_num, int rounds, int plain_cipher_pairs,
                              std::string& file_path, std::string& binary_key);
void attack1(int rounds, int plain_cipher_pairs, int iterations,
             std::string& binary_key, std::fstream& output_file, vvvvd& pre_calculated_mat);
void attack2(int rounds, int plain_cipher_pairs, int iterations,
             std::string& binary_key, std::fstream& output_file, vvvvd& pre_calculated_mat);
void attack2FewLevels(int rounds, int plain_cipher_pairs, int iterations,
                      std::string& binary_key, std::fstream& output_file, vvvvd& pre_calculated_mat);
*/

void initializeOpenOutputFile(std::fstream& output_file, int attack_num, int rounds, int plain_cipher_pairs,
                              std::string& file_path);

void attack1(int rounds, int plain_cipher_pairs, int iterations, std::fstream& output_file,
             vvvvd& pre_calculated_mat);

void attack2(int rounds, int plain_cipher_pairs, int iterations, std::fstream& output_file,
             vvvvd& pre_calculated_mat);

void attack2FewLevels(int rounds, int plain_cipher_pairs, int iterations, std::fstream& output_file,
                      vvvvd& pre_calculated_mat);

#endif //TECHNION_CS_236349_COMPUTER_SECURITY_PROJECT_CPP_RUNNINGATTACKUTIL_H
