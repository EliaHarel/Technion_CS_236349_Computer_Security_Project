//
// Created by EliaHarel on 25/06/2020.
//

#ifndef DES_CPP_DES_ENCRYPT_H
#define DES_CPP_DES_ENCRYPT_H

#include <string>

void DES_Encrypt(const std::string& binary_plaintext, const std::string& binary_key,
                 std::string& binary_ciphertext, int rounds);

std::string bin2hex(std::string s);
std::string hex2bin(std::string s);

#endif //DES_CPP_DES_ENCRYPT_H

