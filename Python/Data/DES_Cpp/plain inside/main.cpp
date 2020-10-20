#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <algorithm>
#include <bitset>
#include <iostream>
#include <fstream>
#include "DES_Encrypt.h"
#include <ctime>
#include <random>
//#include <filesystem>
#include <sys/stat.h>
#include <conio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/*
#include <random>
#include <filesystem>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <functional>
#include <cstdio>
*/


//Tested
unsigned char power(int base, int exponent);


void convert_unsigned_to_file_in_binary(std::vector<unsigned char>& input_text, std::string& output_text);

// input: rounds, pairs_num, key, path_to_output_file
//input example "6 5 0000000000000000000000000000000000000000000000000000000000000000 C:\E.R\Programming\Git\GitHub\Technion_CS_236349_Computer_Security_Project\Data\Plain_Cipher_Pairs\6_rounds\5_pairs\1.txt"
int main(int argc, char* argv[]){

    int rounds = 6;
    int pairs_num = 10;
    std::string binary_key = "0000000000000000000000000000000000000000000000000000000000000000";

    std::vector<std::vector<unsigned char>> plains {std::vector<unsigned char>({172, 220, 184, 40,  163, 27,  14,  167}),
                                                    std::vector<unsigned char>({191, 147, 168, 252, 229, 85, 85, 155}),
                                                    std::vector<unsigned char>({200, 240, 37,  103, 244, 76,  246, 55}),
                                                    std::vector<unsigned char>({123, 95,  191, 208, 236, 225, 51,  68}),
                                                    std::vector<unsigned char>({252, 116, 19,  206, 81,  2,   202, 121}),
                                                    std::vector<unsigned char>({87,  223, 195, 125, 103, 244, 177, 109}),
                                                    std::vector<unsigned char>({104, 206, 133, 200, 211, 18,  215, 131}),
                                                    std::vector<unsigned char>({232, 84,  81,  85,  119, 131, 141, 151}),
                                                    std::vector<unsigned char>({224, 37,  28,  67,  251, 30,  29,  129}),
                                                    std::vector<unsigned char>({142, 138, 219, 144, 183, 114, 36,  0})};

    FILE* fp;
    fp = fopen(
            "C:\\E.R\\Programming\\Git\\GitHub\\Technion_CS_236349_Computer_Security_Project\\Stav_algorithm_prints_plain_n_cipher\\our_results.txt",
            "w+");


    for(int i = 0; i < pairs_num; ++i){
        std::string plaintext;
        std::string ciphertext;
        convert_unsigned_to_file_in_binary(plains[i], plaintext);


        DES_Encrypt(plaintext, binary_key, ciphertext, rounds);


        fprintf(fp, plaintext.c_str());
        fprintf(fp, " ");
        fprintf(fp, ciphertext.c_str());
        fprintf(fp, "\n");

    }
    fclose(fp);
    return 0;
}


std::string decimal2bin(int decimal_num){
    auto binary_num = std::bitset<8>(decimal_num).to_string();
    return binary_num;
}


void convert_unsigned_to_file_in_binary(std::vector<unsigned char>& input_text, std::string& output_text){
    for(int j = 0; j < 8; j++){
        int temp = input_text[j];
        for(int t = 7; t >= 0; t--){
            if(temp >= pow(2, t)){
                output_text.push_back('1');
//                output_text[(7 - t) + j*8] = ;
                temp -= (int) pow(2, t);
            }else{
//                output_text[(7 - t) + j*8] = 0;
                output_text.push_back('0');
            }
        }
    }
}