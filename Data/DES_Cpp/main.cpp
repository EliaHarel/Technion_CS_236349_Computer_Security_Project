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

#define TEXT_LENGTH 64
#define DEBUG false


//Tested
unsigned char power(int base, int exponent);

//Tested
void create_bin_text(std::string& text);

std::string decimal2bin(int decimal_num);


// input: rounds, pairs_num, key, path_to_output_file
//input example "6 5 0000000000000000000000000000000000000000000000000000000000000000 C:\E.R\Programming\Git\GitHub\Technion_CS_236349_Computer_Security_Project\Data\Plain_Cipher_Pairs\6_rounds\5_pairs\1.txt"
int main(int argc, char* argv[]){

    int rounds = strtol(argv[1], nullptr, 10); // number of rounds
    int pairs_num = strtol(argv[2], nullptr, 10); // number of plaintexts
    std::string binary_key = argv[3];
    std::string path_to_output_file = argv[4];


    std::fstream output_file;
    //creates a file if does not exists
    output_file.open(path_to_output_file.c_str(), std::fstream::in|std::fstream::out|std::fstream::app);
    std::string first_line = "rounds: " + std::to_string(rounds) + " key: " + binary_key + "\n";
    output_file << first_line.c_str();


    std::srand(time(nullptr));
    for(int i = 0; i < pairs_num; ++i){
        std::string plaintext;
        std::string ciphertext;
        create_bin_text(plaintext);


        if(DEBUG){
            std::cout << "\nplaintext is bin:\n" << plaintext;
            std::cout << "\nplaintext is hex:\n" << bin2hex(plaintext);
        }

        DES_Encrypt(plaintext, binary_key, ciphertext, rounds);

        if(DEBUG){
            std::cout << "\ncipher is bin:\n" << ciphertext;
            std::cout << "\ncipher is hex:\n" << bin2hex(ciphertext);
        }
        std::cout.flush();
        output_file << plaintext << " " << ciphertext << std::endl;
    }

    output_file.close();
    return 0;
}

void create_bin_text(std::string& text){
    text.clear();
    std::string byte;
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> dis(0.0, 256.0);
    for(int i = 0; i < TEXT_LENGTH/8; ++i){
//        byte = std::to_string(dis(gen));
        text += decimal2bin(rand()%256);
    }
}

std::string decimal2bin(int decimal_num){
    auto binary_num = std::bitset<8>(decimal_num).to_string();
    return binary_num;
}