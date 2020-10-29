//
// Created by EliaHarel on 25/06/2020.
//

#ifndef DES_CPP_DES_ENCRYPT_H
#define DES_CPP_DES_ENCRYPT_H

#include <string>


void encryptWrapper(const std::string& binary_plaintext, const std::string& binary_key,
                 std::string& binary_ciphertext, int rounds);

std::string bin2hex(std::string src);
std::string hex2bin(std::string src);

#endif //DES_CPP_DES_ENCRYPT_H

