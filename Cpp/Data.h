//
// Created by EliaHarel on 18/10/2020.
//

#ifndef TECHNION_CS_236349_COMPUTER_SECURITY_PROJECT_CPP_DATA_H
#define TECHNION_CS_236349_COMPUTER_SECURITY_PROJECT_CPP_DATA_H

#include <string>

#define TEXT_LENGTH 64

//Tested
void createBinText(std::string& text);

std::string decimalToBin(int decimal_num);

void eliaPrintBinaryAsUnsignedChars(std::string A, std::string sep = ",");

std::pair<std::string, std::string> getPlainCipherPair(int rounds, std::string& key);

int binaryStrToInt(std::string binary);

#endif //TECHNION_CS_236349_COMPUTER_SECURITY_PROJECT_CPP_DATA_H
