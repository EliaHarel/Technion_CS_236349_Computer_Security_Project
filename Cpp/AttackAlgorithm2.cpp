//
// Created by DanielYacoby on 23/10/2020.
//

#include "AttackAlgorithm2.h"
#include <vector>
#include <string>
#include <cmath>
#include "DataUtils.h"

namespace masks {

    const int NUM_OF_FIRST_LAST_KEYS = 4096;
    const int NUM_OF_COMBINATIONS = 1 << 28;

    const int s1_out_shift_mask[] = {23, 15, 9, 1}; //{8, 16, 22, 30};
    const int s5_out_shift_mask[] = {24, 18, 7, 29}; //{7, 13, 24, 2};
    vi plain_L_mask{8, 16, 22, 30};
    vi plain_R_mask{7, 13, 24, 2, 31, 0, 1, 2, 3, 4};
    vi cipher_L_mask{7, 13, 24, 2};
    vi cipher_R_mask{8, 16, 22, 30, 15, 16, 17, 18, 19, 20};

// key_mask[i] - the key bit we use in level i+1
    vi key_mask{27, 51, 3, 48, 38, 16, 6, 49, 45, 25, 13, 58, 44, 26, 12, 2};  // starts at 0

    // vi k1_s1_mask{10, 51, 34, 60, 49, 17};
    // vi k16_s5_mask{30, 5, 47, 62, 45, 12};
    // vi first_last_mask = {10, 51, 34, 60, 49, 17, 30, 5, 47, 62, 45, 12};
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
}
using namespace masks;

std::string getSubInput(std::string input, const vi& mask){
    std::string sub_str;
    for(int i : mask){
        sub_str += input[i];
    }
    return sub_str;
}

int sboxFunction(int s_box_num, int input){
    int row = ((input&32) >> 4) + (input&1); // 100001
    int col = (input&30) >> 1; // 011110
    std::vector<std::vector<int>> s_box = s_box_num == 1 ? s1 : s5;
    return s_box[row][col];
}

int calcOutSbox(long long text, const int* mask){
    int c1 = ((text >> mask[0])&1) << 3;
    int c2 = ((text >> mask[1])&1) << 2;
    int c3 = ((text >> mask[2])&1) << 1;
    int c4 = (text >> mask[3])&1;

    return c1 + c2 + c3 + c4;
}


int calcIndex(int num_of_rounds, std::string binary_used_key){
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

    // combination - p out s1 (4)|| p out s5 (4) || p in s1 (6)|| c out s5 (4)|| c out s1 (4)|| c in s5 (6)

    return (s1_out_first << 24) + (s5_out_first << 20) + (s1_in << 14) + (s5_out_last << 10) +
           (s1_out_last << 6) +
           s5_in;
}

std::string reverse(std::string str){
    std::string rev_str;
    for(int i = str.size() - 1; i >= 0; i --){
        rev_str += str[i];
    }
    return rev_str;
}

std::string intToBinStr(int num, int len){
    std::string bin_str;
    int new_num = num;
    while(new_num > 0){
        int res = new_num%2;
        res == 0 ? bin_str += '0' : bin_str += '1';
        new_num /= 2;
    }

    while(bin_str.size() < len){
        bin_str += '0';
    }

    return reverse(bin_str);
}

// returns the output of the s_box for the given s_box_num and binary_input
int sboxFunction(int s_box_num, std::string& binary_input){
    int row = binaryStrToInt(binary_input.substr(0, 1) + binary_input.substr(5, 1));
    int col = binaryStrToInt(binary_input.substr(1, 4));
    std::vector<std::vector<int>> s_box = s_box_num == 1 ? s1 : s5;
    return s_box[row][col];
}

std::pair<int, int> calculate_P_C_from_key_combination(int key, int combination){
    int k1 = (key&4032) >> 6; // Bin - 111111000000
    int k16 = key&63; // Bin- 000000111111

    // combination - p out s1 (4)|| p out s5 (4) || p in s1 (6)|| c out s5 (4)|| c out s1 (4)|| c in s5 (6)
    // combination -     0-3     ||     4-7      ||    8-13    ||    14-17    ||    18-21    ||   22-27

    // plaintext - (out S5)_2 || (out S1)_2 = (out S5)_1 || ((out S1)_1 xor S1(in S1, K1))
    int comb_plain = combination >> 14;
    int plain_left = (comb_plain >> 6)&15;  // (out S5)_1
    int plain_input = comb_plain&63;  // in S1
    int input_to_s1 = plain_input^k1;
    int out_s1_1 = comb_plain >> 10;
    int plain_right = out_s1_1^sboxFunction(1, input_to_s1);
    int plaintext = (plain_left << 4) + plain_right;

    // ciphertext - (out S1)_2 || (out S5)_2 = (out S1)_1 || ((out S1)_1 xor S5(in S5, K16))
    int comb_cipher = combination&16383; // 2^14-1
    int cipher_right = (comb_cipher >> 6)&15;
    int cipher_input = comb_cipher&63;
    int input_to_s5 = cipher_input^k16;
    int out_s5_1 = comb_cipher >> 10;
    int cipher_left = out_s5_1^sboxFunction(5, input_to_s5);
    int ciphertext = (cipher_left << 4) + cipher_right;

    return {plaintext, ciphertext};
}

// num_of_rounds is the number of the rounds characteristics
double calculateDistance(int middle_key, int num_of_rounds, int num_of_inputs,
                         int input_matrix[matrix_size][matrix_size], vvvvd& pre_calculated_mat){
    double distance = 0;
    const auto num_of_cells = matrix_size*matrix_size;
    for(int i = 0; i < matrix_size; i ++){
        for(int j = 0; j < matrix_size; j ++){
            double part_1 = input_matrix[i][j] - ((double) num_of_inputs)/num_of_cells;
            double part_2 = pre_calculated_mat[num_of_rounds][middle_key][i][j] - 1.0/matrix_size;
            distance += part_1*part_2;
        }
    }
    return distance;
}

// num_of_rounds includes the first and last round
int attackAlgorithm2(int num_of_rounds, int num_of_inputs, vvvvd& pre_calculated_mat){

    std::string binary_used_key;
    createBinText(binary_used_key);
    /*
     * plain - 14 bits - out s1 || out s5 || in s1
     * cipher - 14 bits -  out s5 || out s1 || in s5
     * index (int)- plain || cipher
     */

    // vi counter(pow(2, 28), 0);
    static int counter[NUM_OF_COMBINATIONS] = {0};
    for(int i = 0; i < num_of_inputs; i ++){
        int index = calcIndex(num_of_rounds, binary_used_key);
        counter[index] ++;
    }

/*
     * matrix [key][plain][cipher], key - guessed key for 12 key bits of the first and last rounds
     * the M matrix 12 key bits = 6 key bits of s1 | 6 key bits of s5
     * 8 plain bits = 4 output bits of s5 | 4 output bits of s1 (in round 2 of DES (round 1 in our characteristic)
     * 8 cipher bits = 4 output bits of s1 | 4 output bits of s5
     */

    // vvvi input_matrix{static_cast<size_t>(pow(2, 12)), vvi{matrix_size, vi(matrix_size, 0)}};
    static int input_matrix[NUM_OF_FIRST_LAST_KEYS][matrix_size][matrix_size] = {0};

    for(int key = 0; key < pow(2, 12); key ++){
        for(int combination = 0; combination < pow(2, 28); combination ++){
//TODO:  // for(int key = 0; key < NUM_OF_FIRST_LAST_KEYS; key ++){
            //     for(int combination = 0; combination < NUM_OF_COMBINATIONS; combination ++){
            std::pair<int, int> plain_cipher_pair = calculate_P_C_from_key_combination(key, combination);
            // counter[combination] holds the value of the "confirmed values"(C array) for the current combination
            input_matrix[key][plain_cipher_pair.first][plain_cipher_pair.second] += counter[combination];
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
    double used_key_distance = calculateDistance(curr_middle_key_by_rounds, char_rounds, num_of_inputs,
                                                 input_matrix[curr_first_last_key], pre_calculated_mat);

    // for(int first_last_key = 0; first_last_key < pow(2, 12); first_last_key ++){
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



