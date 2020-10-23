//
// Created by EliaHarel on 23/10/2020.
//

#include "AttackAlgorithm2FewLevels.h"
#include "AttackAlgorithm2.h"
#include "DataUtils.h"


#include <vector>
#include <string>
#include <cmath>


namespace types {
    typedef std::vector<int> vi;
    typedef std::vector<vi> vvi;
} // defined in Tables.h
using namespace types;


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

// string getSubInput(string input, const vi& mask){
//     string sub_str;
//     for(int i : mask){
//         sub_str += input[i];
//     }
//     return sub_str;
// }

// // num_of_rounds is the number of the rounds characteristics
// double calculateDistance(int middle_key, int num_of_rounds, int num_of_inputs,
//                          vector<vector<double>> input_matrix, vvvvd& pre_calculated_mat){
//     double distance = 0;
//     for(int i = 0; i < matrix_size; i ++){
//         for(int j = 0; j < matrix_size; j ++){
//             double part_1 = input_matrix[i][j] - (double) num_of_inputs/(matrix_size*matrix_size);
//             double part_2 = pre_calculated_mat[num_of_rounds][middle_key][i][j] - 1.0/((double) matrix_size);
//             distance += part_1*part_2;
//         }
//     }
//     return distance;
// }

// int binaryStrToInt(string binary){
//     int len = binary.size();
//     int res = 0;
//     for(int i = len - 1; i >= 0; i --){
//         int exp = len - 1 - i;
//         if( binary[i] == '1' )
//             res += pow(2, exp);
//     }
//     return res;
// }

// // returns the output of the s_box for the given s_box_num and binary_input
// int sboxFunction(int s_box_num, string& binary_input){
//     int row = binaryStrToInt(binary_input.substr(0, 1) + binary_input.substr(5, 1));
//     int col = binaryStrToInt(binary_input.substr(1, 4));
//     vector<vector<int>> s_box = s_box_num == 1 ? s1 : s5;
//     return s_box[row][col];
// }

// std::string reverse(std::string str){
//     std::string rev_str;
//     for(int i = str.size() - 1; i >= 0; i --){
//         rev_str += str[i];
//     }
//     return rev_str;
// }
//
// std::string intToBinStr(int num, int len){
//     std::string bin_str;
//     int new_num = num;
//     while(new_num > 0){
//         int res = new_num%2;
//         res == 0 ? bin_str += '0' : bin_str += '1';
//         new_num /= 2;
//     }
//
//     while(bin_str.size() < len){
//         bin_str += '0';
//     }
//
//     return reverse(bin_str);
// }

std::pair<int, int> calcPair(int first_last_key, std::string& plain, std::string& cipher){
    std::string str_first_last_key = intToBinStr(first_last_key, 12);
    std::string str_first_key = str_first_last_key.substr(0, 6);
    std::string str_last_key = str_first_last_key.substr(6, 6);
    int first_key = binaryStrToInt(str_first_key);
    int last_key = binaryStrToInt(str_last_key);

    std::string bin_plain_r = plain.substr(0, 32);
    std::string bin_plain_l = plain.substr(32, 32);
    std::string bin_cipher_r = cipher.substr(0, 32);
    std::string bin_cipher_l = cipher.substr(32, 32);

    std::string sub_plain_r = getSubInput(bin_plain_r, plain_R_mask); // s5_out + s1_in
    std::string plain_out_s5 = sub_plain_r.substr(0, 4);
    int plain_in_s1 = binaryStrToInt(sub_plain_r.substr(4, 6));
    int plain_s1_out = binaryStrToInt(getSubInput(bin_plain_l, plain_L_mask));

    std::string input_to_s1 = intToBinStr(plain_in_s1^first_key, 6);
    int out_s1 = sboxFunction(1, input_to_s1);
    std::string out_s1_new = intToBinStr(out_s1^plain_s1_out, 4);
    std::string plaintext = plain_out_s5 + out_s1_new;

    std::string sub_cipher_r = getSubInput(bin_cipher_r, cipher_R_mask); //  s1_out + s5_in
    std::string cipher_out_s1 = sub_cipher_r.substr(0, 4);
    int cipher_in_s5 = binaryStrToInt(sub_cipher_r.substr(4, 6));
    int cipher_s5_out = binaryStrToInt(getSubInput(bin_cipher_l, cipher_L_mask));

    std::string input_to_s5 = intToBinStr(cipher_in_s5^last_key, 6);
    int out_s5 = sboxFunction(5, input_to_s5);
    std::string out_s5_new = intToBinStr(out_s5^cipher_s5_out, 4);
    std::string ciphertext = cipher_out_s1 + out_s5_new;

    return {binaryStrToInt(plaintext), binaryStrToInt(ciphertext)};
}

double AttackAlgorithm2FewLevels(int num_of_rounds, int num_of_inputs, std::string& binary_used_key,
                                 vvvvd& pre_calculated_mat){

    vvvd input_matrix(pow(2, 12), vvd (matrix_size,vd(matrix_size, 0)));
    for(int i = 0; i < num_of_inputs; i ++){
        std::pair<std::string, std::string> plain_cipher_pair = getPlainCipherPair(num_of_rounds, binary_used_key);
        for(int first_last_key = 0; first_last_key < pow(2, 12); first_last_key ++){
            std::pair<int, int> char_plain_cipher_pair = calcPair(first_last_key, plain_cipher_pair.first,
                                                             plain_cipher_pair.second);
            input_matrix[first_last_key][char_plain_cipher_pair.first][char_plain_cipher_pair.second] ++;
        }
    }

    std::string str_curr_first_last_key = getSubInput(binary_used_key, first_last_mask);
    std::string str_curr_middle_key = getSubInput(binary_used_key, key_mask_14_mid_rounds);
    std::string str_curr_middle_key_by_rounds = str_curr_middle_key.substr(0, num_of_rounds);

    int curr_first_last_key = binaryStrToInt(str_curr_first_last_key);
    int curr_middle_key_by_rounds = binaryStrToInt(str_curr_middle_key_by_rounds);


    int location = 1;
    int char_rounds = num_of_rounds - 2;
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


// def get_next_input_from_file(file_object):