
//
// Created by Stav Perla.
// Modified by EliaHarel on 20/10/2020.
//



#include <cmath>
#include <iostream>
#include "AttackAlgorithm1.h"
#include "Tables.h"
#include "Data.h"
#include <fstream>


namespace masks {
    const std::vector<int> plain_mask{7, 13, 24, 2, 40, 48, 54, 62};
    const std::vector<int> cipher_mask{39, 45, 56, 34, 8, 16, 22, 30};
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

static double
calculateKeyDistance(int key, std::vector<std::vector<double>>& mat_summing, int num_of_inputs,
                     int num_of_rounds, vvvvd& pre_calculated_mat){
    double distance = 0;
    for(int i = 0; i < matrix_size; i++){
        for(int j = 0; j < matrix_size; j++){
            double part1 = pre_calculated_mat.at(num_of_rounds).at(key).at(i).at(j) - 1.0/((double) matrix_size);
            double part2 = mat_summing.at(i).at(j) - ((double) num_of_inputs/(matrix_size*matrix_size));
            distance += part1*part2;
        }
    }
    return distance;
}

static std::pair<int, int> createPair(int num_of_rounds, std::string& binary_used_key){
    std::pair<std::string, std::string> data;
    data = getPlainCipherPair(num_of_rounds, binary_used_key);
    auto str_sub_plain = getSubInput(data.first, plain_mask);
    auto str_sub_cipher = getSubInput(data.second, cipher_mask);
    return {binaryStrToInt(str_sub_plain), binaryStrToInt(str_sub_cipher)};
}

int attackAlgorithm1(int num_of_rounds, std::string& binary_used_key, int num_of_inputs,
                     vvvvd& pre_calculated_mat){

    std::vector<std::vector<double>> mat_summing(matrix_size, vd(matrix_size, 0));
    for(int i = 0; i < num_of_inputs; i++){
        std::pair<int, int> plain_cipher_pair = createPair(num_of_rounds, binary_used_key);
        mat_summing.at(plain_cipher_pair.first).at(plain_cipher_pair.second)++;
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


void attack1(int rounds, int plain_cipher_pairs, int iterations,
             std::string& binary_key, std::fstream& output_file, vvvvd& pre_calculated_mat){
    double location_sum = 0;
    for(int i = 0; i < iterations; ++i){
        int location = attackAlgorithm1(rounds, binary_key, plain_cipher_pairs, pre_calculated_mat);
        output_file << "Iteration number: " << i << ". Location is: " << location << std::endl;
        location_sum += location;
    }

    double avg_location = location_sum/iterations;
    output_file << std::endl << "Average Location: " << avg_location << std::endl
                << "Samples :" << iterations << std::endl;

    output_file.close();
}