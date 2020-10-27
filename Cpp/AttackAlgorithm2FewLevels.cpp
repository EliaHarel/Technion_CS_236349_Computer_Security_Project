//
// Created by EliaHarel on 23/10/2020.
//

#include "AttackAlgorithm2FewLevels.h"
#include "AttackAlgorithm2.h"
#include "DataUtils.h"


#include <vector>
#include <string>
#include <cmath>


const int s1_out_shift_mask[]={23, 15, 9, 1}; //{8, 16, 22, 30};
const int s5_out_shift_mask[]={24, 18, 7, 29}; //{7, 13, 24, 2};
const int NUM_OF_FIRST_LAST_KEYS = 4096;

/*
vi plain_L_mask{8, 16, 22, 30};
vi plain_R_mask{7, 13, 24, 2, 31, 0, 1, 2, 3, 4};
vi cipher_L_mask{7, 13, 24, 2};
vi cipher_R_mask{8, 16, 22, 30, 15, 16, 17, 18, 19, 20};

// key_mask[i] - the key bit we use in level i+1
vi key_mask{27, 51, 3, 48, 38, 16, 6, 49, 45, 25, 13, 58, 44, 26, 12, 2};  // starts at 0

vi k1_s1_mask{10, 51, 34, 60, 49, 17};
vi k16_s5_mask{30, 5, 47, 62, 45, 12};*/


vi first_last_mask = {10, 51, 34, 60, 49, 17, 30, 5, 47, 62, 45, 12};
vi key_mask_14_mid_rounds{51, 3, 48, 38, 16, 6, 49, 45, 25, 13, 58, 44, 26,
                          12}; //starts at 0, repeated keys: 51, 49, 45, 12

vvi s1{{14, 4,  13, 1, 2,  15, 11, 8,  3,  10, 6,  12, 5,  9,  0, 7},
       {0,  15, 7,  4, 14, 2,  13, 1,  10, 6,  12, 11, 9,  5,  3, 8},
       {4,  1,  14, 8, 13, 6,  2,  11, 15, 12, 9,  7,  3,  10, 5, 0},
       {15, 12, 8,  2, 4,  9,  1,  7,  5,  11, 3,  14, 10, 0,  6, 13}};

vvi s5{{2,  12, 4,  1,  7,  10, 11, 6,  8,  5,  3,  15, 13, 0, 14, 9},
       {14, 11, 2,  12, 4,  7,  13, 1,  5,  0,  15, 10, 3,  9, 8,  6},
       {4,  2,  1,  11, 10, 13, 7,  8,  15, 9,  12, 5,  6,  3, 0,  14},
       {11, 8,  12, 7,  1,  14, 2,  13, 6,  15, 0,  9,  10, 4, 5,  3}};

/*const int FIRST_LAST_KEY_LENGTH = 12;
const int HALF_PLAIN_CIPHER_LENGTH = 32;
const int IN_SBOX_LENGTH = 6;
const int OUT_SBOX_LENGTH =4;*/


int calcOutSbox (long long text, const int* mask){
    int c1= ((text>>mask[0])&1)<<3;
    int c2= ((text >> mask[1])&1)<<2;
    int c3= ((text >> mask[2])&1)<<1;
    int c4= (text >> mask[3])&1;

    return c1 + c2 + c3 + c4;
}

/*

std::pair<int, int> calcFirstRoundParams( std::string& plain) {

    std::string bin_plain_r = plain.substr(0, HALF_PLAIN_CIPHER_LENGTH);
    std::string bin_plain_l = plain.substr(32, HALF_PLAIN_CIPHER_LENGTH);

    std::string sub_plain_r = getSubInput(bin_plain_r, plain_R_mask); // s5_out + s1_in
    std::string plain_out_s5 = sub_plain_r.substr(0, OUT_SBOX_LENGTH);
    int plain_in_s1 = binaryStrToInt(sub_plain_r.substr(4, IN_SBOX_LENGTH));
    int plain_s1_out = binaryStrToInt(getSubInput(bin_plain_l, plain_L_mask));

    return {plain_in_s1, plain_s1_out};
}

std::pair<int, int> calcLastRoundParams( std::string& cipher) {

    std::string bin_cipher_r = cipher.substr(0, HALF_PLAIN_CIPHER_LENGTH);
    std::string bin_cipher_l = cipher.substr(32, HALF_PLAIN_CIPHER_LENGTH);

    std::string sub_cipher_r = getSubInput(bin_cipher_r, cipher_R_mask); //  s1_out + s5_in
    std::string cipher_out_s1 = sub_cipher_r.substr(0, 4);
    int cipher_in_s5 = binaryStrToInt(sub_cipher_r.substr(4, IN_SBOX_LENGTH));
    int cipher_s5_out = binaryStrToInt(getSubInput(bin_cipher_l, cipher_L_mask));

    return {cipher_in_s5, cipher_s5_out};
}

std::pair<int, int> calcCharPlainCipher( std::pair<int, int> first_round, std::pair<int, int> last_round, int first_last_key ) {
    int first_key = first_last_key& 63; // Bin- 000000111111
    int last_key = first_last_key& 4032; // Bin - 111111000000

    std::string input_to_s1 = intToBinStr(first_round.first^first_key, IN_SBOX_LENGTH);
    int out_s1 = sboxFunction(1, input_to_s1);
    std::string out_s1_new = intToBinStr(out_s1 ^ first_round.second, OUT_SBOX_LENGTH);
    std::string plaintext = plain_out_s5 + out_s1_new;

    std::string input_to_s5 = intToBinStr(cipher_in_s5^last_key, IN_SBOX_LENGTH);
    int out_s5 = sboxFunction(5, input_to_s5);
    std::string out_s5_new = intToBinStr(out_s5^cipher_s5_out, OUT_SBOX_LENGTH);
    std::string ciphertext = cipher_out_s1 + out_s5_new;

    return {binaryStrToInt(plaintext), binaryStrToInt(ciphertext)};
}*/

// int attackAlgorithm2FewLevels(int num_of_rounds, int num_of_inputs, std::string& binary_used_key,
//                                  vvvvd& pre_calculated_mat){
int attackAlgorithm2FewLevels(int num_of_rounds, int num_of_inputs, vvvvd& pre_calculated_mat){
    std::string binary_used_key;
    //createBinText(binary_used_key);
    binary_used_key= "0000000000000000000000000000000000000000000000000000000000000000";
   // vvvi input_matrix(NUM_OF_FIRST_LAST_KEYS,vvi(matrix_size, vi(matrix_size,0)));
   int input_matrix [NUM_OF_FIRST_LAST_KEYS][matrix_size][matrix_size]={0};
    for(int i = 0; i < num_of_inputs; i ++){
        std::pair<std::string, std::string> plain_cipher_pair = getPlainCipherPair(num_of_rounds,
                                                                                   binary_used_key);
        std::string plain_left = plain_cipher_pair.first.substr(0, 32);
        std::string plain_right = plain_cipher_pair.first.substr(32, 32);
        std::string cipher_left = plain_cipher_pair.second.substr(0, 32);
        std::string cipher_right = plain_cipher_pair.second.substr(32, 32);

        long long p_left = stoll(plain_left, nullptr, 2);
        long long p_right = stoll(plain_right, nullptr, 2);
        long long c_left = stoll(cipher_left,nullptr,2);
        long long c_right = stoll(cipher_right,nullptr,2);

        int s1_in = ((p_right& 1)<<5) + (p_right>>27); // bits 32,1,2,3,4,5 of p_right;
        int s5_in = (c_right>>11) & 63 ; // bits 16,17,18,19,20, 21 of c_right;

        int s1_out_first = calcOutSbox(p_left, s1_out_shift_mask);
        int s5_out_first =calcOutSbox(p_right, s5_out_shift_mask); // char_plain_left
        int s1_out_last = calcOutSbox(c_right, s1_out_shift_mask); // char_cipher_right (swap!!!)
        int s5_out_last =calcOutSbox(c_left, s5_out_shift_mask);

        for(int first_last_key = 0; first_last_key < NUM_OF_FIRST_LAST_KEYS; ++first_last_key){
            //std::pair<int, int> char_plain_cipher_pair = calcCharPlainCipher(first_round, last_round, first_last_key);

            int first_key = (first_last_key& 4032)>>6; // Bin - 111111000000
            int last_key = first_last_key& 63; // Bin- 000000111111

            int input_s1 = first_key ^ s1_in;
            int input_s5 = last_key ^ s5_in;

            int char_plain_right = s1_out_first ^ sboxFunction(1, input_s1);
            int char_cipher_left = s5_out_last ^ sboxFunction(5, input_s5);

            int char_plain = (s5_out_first << 4) + char_plain_right;
            int char_cipher = (char_cipher_left << 4) + s1_out_last;

            input_matrix[first_last_key][char_plain][char_cipher] ++;
        }
    }

    int char_rounds = num_of_rounds - 2;
    std::string str_curr_first_last_key = getSubInput(binary_used_key, first_last_mask);
    std::string str_curr_middle_key = getSubInput(binary_used_key, key_mask_14_mid_rounds);
    std::string str_curr_middle_key_by_rounds = str_curr_middle_key.substr(0, char_rounds);

    int curr_first_last_key = binaryStrToInt(str_curr_first_last_key);
    int curr_middle_key_by_rounds = binaryStrToInt(str_curr_middle_key_by_rounds);


    int location = 1;
    double used_key_distance = calculateDistance(curr_middle_key_by_rounds, char_rounds,
                                                 num_of_inputs, input_matrix[curr_first_last_key],
                                                 pre_calculated_mat);

    for(int first_last_key = 0; first_last_key < NUM_OF_FIRST_LAST_KEYS; first_last_key ++){
        for(int middle_key = 0; middle_key < pow(2, char_rounds); middle_key ++){
            if( first_last_key == curr_first_last_key && middle_key == curr_middle_key_by_rounds ) continue;
            double curr_dist = calculateDistance(middle_key, char_rounds, num_of_inputs,
                                                 input_matrix[first_last_key], pre_calculated_mat);
            if( curr_dist > used_key_distance ){
                location ++;
            }
        }
    }
    return location;
}
