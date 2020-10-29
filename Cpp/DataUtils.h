//
// Created by EliaHarel on 18/10/2020.
//

#ifndef TECHNION_CS_236349_COMPUTER_SECURITY_PROJECT_CPP_DATAUTILS_H
#define TECHNION_CS_236349_COMPUTER_SECURITY_PROJECT_CPP_DATAUTILS_H

#include <string>

#define TEXT_LENGTH 64

void createBinText(std::string& text);

std::string decimalToBin(int decimal_num);

std::pair<std::string, std::string> getPlainCipherPair(int rounds, std::string& key);

long long binaryStrToInt(std::string binary);

#endif //TECHNION_CS_236349_COMPUTER_SECURITY_PROJECT_CPP_DATAUTILS_H
