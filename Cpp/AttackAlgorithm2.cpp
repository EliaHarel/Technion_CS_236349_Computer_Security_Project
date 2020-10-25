//
// Created by EliaHarel on 23/10/2020.
//

#include "AttackAlgorithm2.h"
#include <vector>
#include <string>
#include <cmath>
#include "DataUtils.h"

namespace masks {
    vi plain_L_mask{8, 16, 22, 30};
    vi plain_R_mask{7, 13, 24, 2, 31, 0, 1, 2, 3, 4};
    vi cipher_L_mask{7, 13, 24, 2};
    vi cipher_R_mask{8, 16, 22, 30, 15, 16, 17, 18, 19, 20};

// key_mask[i] - the key bit we use in level i+1
    vi key_mask{27, 51, 3, 48, 38, 16, 6, 49, 45, 25, 13, 58, 44, 26, 12, 2};  // starts at 0

    vi k1_s1_mask{10, 51, 34, 60, 49, 17};
    vi k16_s5_mask{30, 5, 47, 62, 45, 12};
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
}
using namespace masks;

std::string getSubInput(std::string input, const vi& mask){
    std::string sub_str;
    for(int i : mask){
        sub_str += input[i];
    }
    return sub_str;
}

int calcIndex(int num_of_rounds, std::string binary_used_key){
    std::pair<std::string, std::string> data = getPlainCipherPair(num_of_rounds, binary_used_key);
    std::string bin_plain_r = data.first.substr(0, 32);
    std::string bin_plain_l = data.first.substr(32, 32);
    std::string bin_cipher_r = data.second.substr(0, 32);
    std::string bin_cipher_l = data.second.substr(32, 32);

    std::string sub_plain_r = getSubInput(bin_plain_r, plain_R_mask);
    std::string sub_plain_l = getSubInput(bin_plain_l, plain_L_mask);
    std::string sub_cipher_r = getSubInput(bin_cipher_r, cipher_R_mask);
    std::string sub_cipher_l = getSubInput(bin_cipher_l, cipher_L_mask);

    std::string sub_plain = sub_plain_l + sub_plain_r;
    std::string sub_cipher = sub_cipher_l + sub_cipher_r;
    std::string sub_input = sub_plain + sub_cipher;

    return binaryStrToInt(sub_input);
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
    std::string binary_key = intToBinStr(key, 12);
    std::string k1 = binary_key.substr(0, 6);
    std::string k16 = binary_key.substr(6, 6);

    // combination - p out s1 (4)|| p out s5 (4) || p in s1 (6)|| c out s5 (4)|| c out s1 (4)|| c in s5 (6)
    // combination -     0-3     ||     4-7      ||    8-13    ||    14-17    ||    18-21    ||   22-27
    std::string binary_combination = intToBinStr(combination, 28);

    // plaintext - (out S5)_2 || (out S1)_2 = (out S5)_1 || ((out S1)_1 xor S1(in S1, K1))
    std::string plain_left = binary_combination.substr(4, 4);  // (out S5)_1
    std::string binary_input1 = binary_combination.substr(8, 6);  // in S1
    std::string input_to_s1 = intToBinStr(binaryStrToInt(binary_input1)^binaryStrToInt(k1), 6);
    std::string out_s1_1 = binary_combination.substr(0, 4);
    std::string plain_right = intToBinStr(binaryStrToInt(out_s1_1)^sboxFunction(1, input_to_s1), 4);
    int plaintext = binaryStrToInt(plain_left + plain_right);

    // ciphertext - (out S1)_2 || (out S5)_2 = (out S1)_1 || ((out S1)_1 xor S5(in S5, K16))
    std::string cipher_left = binary_combination.substr(18, 4);
    std::string binary_input16 = binary_combination.substr(22, 6);
    std::string input_to_s5 = intToBinStr(binaryStrToInt(binary_input16)^binaryStrToInt(k16), 6);
    std::string out_s5_1 = binary_combination.substr(14, 4);
    std::string cipher_right = intToBinStr(binaryStrToInt(out_s5_1)^sboxFunction(5, input_to_s5), 4);
    int ciphertext = binaryStrToInt(cipher_left + cipher_right);

    return {plaintext, ciphertext};
}

// num_of_rounds is the number of the rounds characteristics
double calculateDistance(int middle_key, int num_of_rounds, int num_of_inputs,
                         vvi& input_matrix, vvvvd& pre_calculated_mat){
    double distance = 0;
    for(int i = 0; i < matrix_size; i ++){
        for(int j = 0; j < matrix_size; j ++){
            double part_1 = input_matrix[i][j] - (double) num_of_inputs/(matrix_size*matrix_size);
            double part_2 = pre_calculated_mat[num_of_rounds][middle_key][i][j] - 1.0/((double) matrix_size);
            distance += part_1*part_2;
        }
    }
    return distance;
}

// num_of_rounds includes the first and last round
// int attackAlgorithm2(int num_of_rounds, int num_of_inputs, std::string& binary_used_key,
//                      vvvvd& pre_calculated_mat){
int attackAlgorithm2(int num_of_rounds, int num_of_inputs, vvvvd& pre_calculated_mat){
    std::string binary_used_key;
    createBinText(binary_used_key);
    int char_rounds = num_of_rounds - 2;

    /*
     * plain - 14 bits - out s1 || out s5 || in s1
     * cipher - 14 bits -  out s5 || out s1 || in s5
     * index (int)- plain || cipher
     */
    vi counter(pow(2, 28), 0);
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
    vvvi input_matrix{static_cast<size_t>(pow(2, 12)), vvi{matrix_size, vi(matrix_size, 0)}};

    for(int key = 0; key < pow(2, 12); key ++){
        for(int combination = 0; combination < pow(2, 28); combination ++){
            std::pair<int, int> plain_cipher_pair = calculate_P_C_from_key_combination(key, combination);
            // counter[combination] holds the value of the "confirmed values"(C array) for the current combination
            input_matrix[key][plain_cipher_pair.first][plain_cipher_pair.second] += counter[combination];
        }
    }

    std::string str_curr_first_last_key = getSubInput(binary_used_key, first_last_mask);
    std::string str_curr_middle_key = getSubInput(binary_used_key, key_mask_14_mid_rounds);
    std::string str_curr_middle_key_by_rounds = str_curr_middle_key.substr(0, num_of_rounds);

    int curr_first_last_key = binaryStrToInt(str_curr_first_last_key);
    int curr_middle_key_by_rounds = binaryStrToInt(str_curr_middle_key_by_rounds);


    int location = 1;
    double used_key_distance = calculateDistance(curr_middle_key_by_rounds, char_rounds,
                                                 num_of_inputs, input_matrix[curr_first_last_key],
                                                 pre_calculated_mat);

    for(int first_last_key = 0; first_last_key < pow(2, 12); first_last_key ++){
        for(int middle_key = 0; middle_key < pow(2, char_rounds); middle_key ++){
            if( first_last_key == curr_first_last_key and middle_key == curr_middle_key_by_rounds ) continue;
            double curr_dist = calculateDistance(middle_key, char_rounds, num_of_inputs,
                                                 input_matrix[first_last_key], pre_calculated_mat);
            if( curr_dist > used_key_distance ){
                location ++;
            }
        }
    }

    return location;
}


