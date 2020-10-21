//
// Created by EliaHarel on 20/10/2020.
//

#ifndef TECHNION_CS_236349_COMPUTER_SECURITY_PROJECT_CPP_RUNNINGATTACKUTIL_H
#define TECHNION_CS_236349_COMPUTER_SECURITY_PROJECT_CPP_RUNNINGATTACKUTIL_H

void extractingParams(int argc, char* argv[], int* rounds, int* plain_cipher_pairs, int* iterations,
                      std::string& file_path, std::string& binary_key);


void initializeOpenOutputFile(std::fstream& output_file, int rounds, int plain_cipher_pairs,
                             std::string& file_path, std::string& binary_key);

#endif //TECHNION_CS_236349_COMPUTER_SECURITY_PROJECT_CPP_RUNNINGATTACKUTIL_H
