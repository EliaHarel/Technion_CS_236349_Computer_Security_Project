//
// Created by EliaHarel on 18/10/2020.
//

#include "DES_Encrypt.h"
#include "Data.h"
#include <random>
#include <iostream>
#include <cmath>
#include <bitset>
#include <string>


std::string decimalToBin(int decimal_num){
    auto binary_num = std::bitset<8>(decimal_num).to_string();
    return binary_num;
}

/*void createBinText(std::string& text){
    text.clear();
    std::string byte;
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> dis(0.0, 256.0);
    for(int i = 0; i < TEXT_LENGTH/8; ++i){
        text += decimalToBin(int(round(dis(gen)))%256);
    }
}*/
void createBinText(std::string& text){
    text.clear();
    // std::string byte;
    // std::random_device rd;  //Will be used to obtain a seed for the random number engine
    // std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    // std::uniform_real_distribution<> dis(0.0, 256.0);
    for(int i = 0; i < TEXT_LENGTH/8; ++i){
       // byte = std::to_string(dis(gen));
        text += decimalToBin(rand()%256);
    }
}

std::pair<std::string, std::string> getPlainCipherPair(int rounds, std::string& key){
    std::string plain;
    createBinText(plain);

    std::string cipher;
    encryptWrapper(plain, key, cipher, rounds);

    return {plain, cipher};
}

int binaryStrToInt(std::string binary){
    int len = binary.size();
    int res = 0;
    for(int i = len - 1; i >= 0; i--){
        int exp = len - 1 - i;
        if(binary[i] == '1')
            res += pow(2, exp);
    }
    return res;
}

void eliaPrintBinaryAsUnsignedChars(std::string A, std::string& sep){
    std::string ln;
    for(int i = 0; i < A.size(); i += 8){
        int sum = 0;
        for(int j = 0; j < 8; ++j){
            if(A[i + j] == '1')
                sum += int(pow(2, 8 - j - 1));
        }
        std::cout << sum << ((i + 8 < A.size()) ? sep : "\n");
    }

}