//
// Created by DanielYacoby on 23/10/2020.
//

#include "AttackAlgorithm2FewLevels.h"
#include "AttackAlgorithm2.h"
#include "DataUtils.h"


#include <vector>
#include <string>
#include <map>
#include <cmath>

const int s1_out_shift_mask[] = {23, 15, 9, 1}; //{8, 16, 22, 30};
const int s5_out_shift_mask[] = {24, 18, 7, 29}; //{7, 13, 24, 2};
const int NUM_OF_FIRST_LAST_KEYS = 4096;


vi first_mask = {9, 50, 33, 59, 48, 16}; //starts at 0
vvi last_mask = {{},
                 {},
                 {},
                 {},
                 {},
                 {12, 22, 29, 44, 62, 61}, // 6 rounds
                 {},
                 {11, 53, 60, 12, 30, 29}, // 8 rounds
                 {},
                 {54, 29, 36, 19, 6,  5}, // 10 rounds
                 {},
                 {22, 60, 4,  54, 37, 36}, // 12 rounds
                 {},
                 {53, 28, 3,  22, 5,  4}, // 14 rounds
                 {},
                 {29, 4,  46, 61, 44, 11}}; // 16 rounds
vi key_mask_14_mid_rounds{19, 35, 54, 32, 22, 0, 55, 41, 29, 9, 60, 42, 28,
                          10}; //starts at 0, repeated keys: 9,

vvi s1{{14, 4,  13, 1, 2,  15, 11, 8,  3,  10, 6,  12, 5,  9,  0, 7},
       {0,  15, 7,  4, 14, 2,  13, 1,  10, 6,  12, 11, 9,  5,  3, 8},
       {4,  1,  14, 8, 13, 6,  2,  11, 15, 12, 9,  7,  3,  10, 5, 0},
       {15, 12, 8,  2, 4,  9,  1,  7,  5,  11, 3,  14, 10, 0,  6, 13}};

vvi s5{{2,  12, 4,  1,  7,  10, 11, 6,  8,  5,  3,  15, 13, 0, 14, 9},
       {14, 11, 2,  12, 4,  7,  13, 1,  5,  0,  15, 10, 3,  9, 8,  6},
       {4,  2,  1,  11, 10, 13, 7,  8,  15, 9,  12, 5,  6,  3, 0,  14},
       {11, 8,  12, 7,  1,  14, 2,  13, 6,  15, 0,  9,  10, 4, 5,  3}};

/*
Starting the count from 0
first round mask  - {9, 50, 33, 59, 48, 16};
last round mask -
                 {12, 22, 29, 44, 62, 61}, // 6 rounds
                 {11, 53, 60, 12, 30, 29}, // 8 rounds
                 {54, 29, 36, 19, 6,  5}, // 10 rounds
                 {22, 60, 4,  54, 37, 36}, // 12 rounds
                 {53, 28, 3,  22, 5,  4}, // 14 rounds
                 {29, 4,  46, 61, 44, 11}}; // 16 rounds
key_mask_14_mid_rounds-
{19, 35, 54, 32, 22, 0, 55, 41, 29, 9, 60, 42, 28, 10};

common bits:
    6_rounds - none
    8_rounds - none
    10_rounds - 19, 54
    12_rounds - 9, 22, 54
    14_rounds - 9, 22
    16_rounds - 9, 29
*/


std::map<int, std::pair<vi, vi>> common_bits = {
        //for each bit, the numbers represent thr number of bit to it's right
        // in each mask - last key, middle key
        //common bits of first key mask are treated differently
        {10, std::pair<vi, vi>{vi{2, 5}, vi{7, 5}}},
        {12, std::pair<vi, vi>{vi{5, 2}, vi{5, 7}}},
        {14, std::pair<vi, vi>{vi{2}, vi{7}}},
        {16, std::pair<vi, vi>{vi{5}, vi{5}}},
};

bool sameBits(int first_last_key, int middle_key, int num_of_rounds){
    // Assumption - num_of_rounds in {6,8,10,12,14,16}
    if(num_of_rounds == 6 || num_of_rounds == 8) return true; // different key's bits
    if(num_of_rounds >= 12){
        int first_key_bit = first_last_key >> 11;
        int middle_bit = (middle_key >> (num_of_rounds - 12))&1; //the key for round number 11
        if(first_key_bit != middle_bit) return false;
    }

    int last_key = first_last_key&63;
    int num_of_bits = common_bits[num_of_rounds].first.size();
    for(int i = 0; i < num_of_bits; i++){
        int last_bit = (last_key >> common_bits[num_of_rounds].first[i])&1;
        int middle_bit = (middle_key >> common_bits[num_of_rounds].second[i])&1;
        if(last_bit != middle_bit) return false;
    }

    return true;
}

int attackAlgorithm2FewLevels(int num_of_rounds, int num_of_inputs, vvvvd& pre_calculated_mat){
    std::string binary_used_key;
    createBinText(binary_used_key);
    vvvi input_matrix{static_cast<size_t>(pow(2, 12)), vvi{matrix_size, vi(matrix_size, 0)}};
//    int input_matrix[NUM_OF_FIRST_LAST_KEYS][matrix_size][matrix_size] = {0};
    for(int i = 0; i < num_of_inputs; i++){
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

        int s1_in = ((p_right&1) << 5) + (p_right >> 27); // bits 32,1,2,3,4,5 of p_right;
        int s5_in = (c_right >> 11)&63; // bits 16,17,18,19,20, 21 of c_right;

        int s1_out_first = calcOutSbox(p_left, s1_out_shift_mask);
        int s5_out_first = calcOutSbox(p_right, s5_out_shift_mask); // char_plain_left
        int s1_out_last = calcOutSbox(c_right, s1_out_shift_mask); // char_cipher_right (swap!!!)
        int s5_out_last = calcOutSbox(c_left, s5_out_shift_mask);

        for(int first_last_key = 0; first_last_key < NUM_OF_FIRST_LAST_KEYS; ++first_last_key){
            //std::pair<int, int> char_plain_cipher_pair = calcCharPlainCipher(first_round, last_round, first_last_key);

            int first_key = (first_last_key&4032) >> 6; // Bin - 111111000000
            int last_key = first_last_key&63; // Bin- 000000111111

            int input_s1 = first_key^s1_in;
            int input_s5 = last_key^s5_in;

            int char_plain_right = s1_out_first^sboxFunction(1, input_s1);
            int char_cipher_left = s5_out_last^sboxFunction(5, input_s5);

            int char_plain = (s5_out_first << 4) + char_plain_right;
            int char_cipher = (char_cipher_left << 4) + s1_out_last;

            input_matrix[first_last_key][char_plain][char_cipher]++;
        }
    }

    int char_rounds = num_of_rounds - 2;
    std::string str_curr_first_key = getSubInput(binary_used_key, first_mask);
    std::string str_curr_last_key = getSubInput(binary_used_key, last_mask[num_of_rounds - 1]);
    std::string str_curr_first_last_key = str_curr_first_key + str_curr_last_key;
    std::string str_curr_middle_key = getSubInput(binary_used_key, key_mask_14_mid_rounds);
    std::string str_curr_middle_key_by_rounds = str_curr_middle_key.substr(0, char_rounds);

    int curr_first_last_key = stoi(str_curr_first_last_key, nullptr, 2);
    int curr_middle_key_by_rounds = stoi(str_curr_middle_key_by_rounds, nullptr, 2);


    int location = 1;
    double used_key_distance = calculateDistance(curr_middle_key_by_rounds, char_rounds,
                                                 num_of_inputs, input_matrix[curr_first_last_key],
                                                 pre_calculated_mat);

    for(int first_last_key = 0; first_last_key < NUM_OF_FIRST_LAST_KEYS; first_last_key++){
        for(int middle_key = 0; middle_key < pow(2, char_rounds); middle_key++){
            if(first_last_key == curr_first_last_key && middle_key == curr_middle_key_by_rounds) continue;
            if(!sameBits(first_last_key, middle_key, num_of_rounds)) continue;
            double curr_dist = calculateDistance(middle_key, char_rounds, num_of_inputs,
                                                 input_matrix[first_last_key], pre_calculated_mat);
            if(curr_dist > used_key_distance){
                location++;
            }
        }
    }
    return location;
}