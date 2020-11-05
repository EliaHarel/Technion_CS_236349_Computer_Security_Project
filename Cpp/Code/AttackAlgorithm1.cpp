//
// Created by Stav Perla.
// Modified by EliaHarel on 20/10/2020.
//


#include <cmath>
#include <iostream>
#include "AttackAlgorithm1.h"
#include "Tables.h"
#include "DataUtils.h"
#include <fstream>


namespace masks {
    const int plain_left_shift_mask[] = {24, 18, 7, 29}; // {7, 13, 24, 2} 31-x
    const int plain_right_shift_mask[] = {23, 15, 9, 1}; // {40, 48, 54, 62} 63-x
    const int cipher_left_shift_mask[] = {23, 15, 9, 1}; //{8, 16, 22, 30} 31-x
    const int cipher_right_shift_mask[] = {24, 18, 7, 29}; //{39, 45, 56, 34} 63-x

    // key_mask[i] - the key bit we use in level i+1
    const std::vector<int> key_mask{27, 51, 3, 48, 38, 16, 6, 49, 45, 25, 13, 58, 44, 26, 12, 2};
}
using namespace masks;


static std::string getSubInput(std::string& input, const std::vector<int>& mask){
    std::string sub_str;
    for(int i : mask){
        sub_str += input.at(i);
    }
    return sub_str;
}

static int getSubInput(long long input, const int mask[4]){
    int res = 0;
    int size = 4;
    for(int i = 0; i < size; i++){
        int shift_left = size - 1 - i;
        res += ((input >> mask[i])&1) << shift_left;
    }
    return res;
}

static double calculateKeyDistance(int key, int mat_summing[matrix_size][matrix_size],
                                   int num_of_inputs, int num_of_rounds, vvvvd& pre_calculated_mat){
    double distance = 0;
    for(int i = 0; i < matrix_size; i++){
        for(int j = 0; j < matrix_size; j++){
            double part1 =
                    pre_calculated_mat.at(num_of_rounds).at(key).at(i).at(j) - 1.0/((double) matrix_size);
            double part2 = mat_summing[i][j] - ((double) num_of_inputs)/(matrix_size*matrix_size);
            distance += part1*part2;
        }
    }
    return distance;
}


static std::pair<int, int> createPair(int num_of_rounds, std::string& binary_used_key){
    std::pair<std::string, std::string> plain_cipher_pair = getPlainCipherPair(num_of_rounds,
                                                                               binary_used_key);
    std::string plain_left = plain_cipher_pair.first.substr(0, 32);
    std::string plain_right = plain_cipher_pair.first.substr(32, 32);
    std::string cipher_left = plain_cipher_pair.second.substr(0, 32);
    std::string cipher_right = plain_cipher_pair.second.substr(32, 32);

    long long p_left = stoll(plain_left, nullptr, 2);
    long long p_right = stoll(plain_right, nullptr, 2);
    long long c_left = stoll(cipher_left, nullptr, 2);
    long long c_right = stoll(cipher_right, nullptr, 2);

    int plaintext_left = getSubInput(p_left, plain_left_shift_mask);
    int plaintext_right = getSubInput(p_right, plain_right_shift_mask);
    int ciphertext_right = getSubInput(c_left, cipher_left_shift_mask); // swap
    int ciphertext_left = getSubInput(c_right, cipher_right_shift_mask); //swap


    int plain = (plaintext_left << 4) + plaintext_right;
    int cipher = (ciphertext_left << 4) + ciphertext_right;
    return {plain, cipher};
}

int attackAlgorithm1(int num_of_rounds, int num_of_inputs, vvvvd& pre_calculated_mat){

    std::string binary_used_key;
    createBinText(binary_used_key);
    static int mat_summing[matrix_size][matrix_size] = {0};
    for(int i = 0; i < num_of_inputs; i++){
        std::pair<int, int> plain_cipher_pair = createPair(num_of_rounds, binary_used_key);
        mat_summing[plain_cipher_pair.first][plain_cipher_pair.second]++;
    }

    std::string str_sub_used_key = getSubInput(binary_used_key, key_mask);
    std::string str_sub_used_key_by_rounds = str_sub_used_key.substr(0, num_of_rounds);
    int sub_used_key_by_rounds = binaryStrToInt(str_sub_used_key_by_rounds);
    double used_key_distance = calculateKeyDistance(sub_used_key_by_rounds, mat_summing, num_of_inputs,
                                                    num_of_rounds, pre_calculated_mat);

    int location = 1;
    for(int temp_key = 0; temp_key < pow(2, num_of_rounds); temp_key++){
        if(temp_key == sub_used_key_by_rounds) continue;
        double curr_distance = calculateKeyDistance(temp_key, mat_summing, num_of_inputs, num_of_rounds,
                                                    pre_calculated_mat);
        if(curr_distance > used_key_distance){
            location++;
        }
    }

    return location;

}


