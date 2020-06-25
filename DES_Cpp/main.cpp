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
#include <filesystem>
#include <sys/stat.h>

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

#define TEXT_LENGTH 64
#define DEBUG false


//Tested
unsigned char power(int base, int exponent);

//Tested
unsigned char bin2decimal(const std::string& bin_number, int index);

void create_binary_key(std::string& key);

//void decimal2bin(unsigned char* binary, FILE* fp);

std::string find_available_file_name(const std::string& prefix_path);

void create_bin_text(std::string& text);

//unsigned char* create_text(unsigned char* text);
//bool file_exists(const char* fname);
//char* append_strings(char* chars[], int num);
//char* append_2_strings(char* a, char* b);
//char* append_3_strings(char* a, char* b, char* c);
//char* append_4_strings(char* a, char* b, char* c, char* d);
//char* append_5_strings(char* a, char* b, char* c, char* d, char* e);
//char* find_available_file_name(char* prefix_path);
int Daniel_is_annoying(int argc, char* argv[]);

// input: rounds, num_of_plaintexts, path_to_data_directory, key = optional
//input example "16 5000 /cygdrive/c/E.R/Programming/Git/GitHub/Technion_CS_236349_Computer_Security_Project/Data 0000000000000000000000000000000000000000000000000000000000000000"
int main(int argc, char* argv[]){
    for(int i = 0; i < 100; ++i){
        Daniel_is_annoying(argc, argv);

    }
}

int Daniel_is_annoying(int argc, char* argv[]){
    int rounds = strtol(argv[1], nullptr, 10); // number of rounds
    int num_of_plaintexts = strtol(argv[2], nullptr, 10); // number of plaintexts
    std::string path_to_data_directory = argv[3];

    std::string key;
    std::string plaintext;
    std::string ciphertext;
    std::string slash = "/";
    std::string binary_key;

    std::srand(std::time(nullptr));

    //binary_key is a string of a binary number
    if(argc == 4)
        create_binary_key(binary_key);
    else
        binary_key += argv[4];


    for(int param_i = 0, unsigned_char_i = 0; param_i < TEXT_LENGTH/8; param_i++, unsigned_char_i += 8)
        key[param_i] = bin2decimal(binary_key, unsigned_char_i);


    /*
        path block
    */
    std::string path_to_rounds = path_to_data_directory + slash + std::to_string(rounds) + "_rounds";
    struct stat st = {0};

    if(stat(path_to_rounds.c_str(), &st) == -1)// create directory if does not exist
        mkdir(path_to_rounds.c_str(), 0700);


//    std::string path_to_key = path_to_rounds + slash + binary_key + "_key";
//    if(stat(path_to_key.c_str(), &st) == -1){
//        mkdir(path_to_key.c_str(), 0700);
//    }


    std::string path_to_num_of_plaintexts = path_to_rounds + slash +
                                            std::to_string(num_of_plaintexts) + "_plaintexts";

    if(stat(path_to_num_of_plaintexts.c_str(), &st) == -1) //create directory if does not exist
        mkdir(path_to_num_of_plaintexts.c_str(), 0700);


    /*
         block
    */
    std::string prefix_path = path_to_num_of_plaintexts + slash;
    std::string final_path = find_available_file_name(prefix_path);

    std::fstream output_file;
    output_file.open(final_path.c_str(), std::fstream::in|std::fstream::out|std::fstream::app);
    std::string first_line = "rounds: " + std::to_string(rounds) + " key: " + binary_key + "\n";
    output_file << first_line.c_str();


    for(int i = 0; i < num_of_plaintexts; ++i){
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


//Tested
unsigned char power(int base, int exponent){
    if(exponent < 0) return 0; // checking argument
    if(!exponent) return 1;

    unsigned char result;
    int temp_result = base;
    while(--exponent) temp_result *= base;
    result = temp_result;
    return result;
}

//Tested
unsigned char bin2decimal(const std::string& bin_number, int index){
    int result = 0;
    for(int i = (TEXT_LENGTH/8) - 1; i >= 0; --i)
        if(bin_number[index++] == '1')
            result += power(2, i);
    return result;
}

bool is_file_exist(const char* fileName){
    std::ifstream infile(fileName);
    return infile.good();
}

void create_bin_text(std::string& text){
    text.clear();
    for(int i = 0; i < TEXT_LENGTH; ++i)
        text += std::to_string(std::rand()%2);
}

void create_binary_key(std::string& key){
    for(int i = 0; i < 64; ++i)
        key += std::rand()%2 ? '1' : '0';
}

std::string decimal2bin(std::string& decimal_num){
    std::string binary_num = std::bitset<64>(decimal_num).to_string();
    return binary_num;
}

std::string find_available_file_name(const std::string& prefix_path){
    int counter = 0;
    std::string temp_path(prefix_path);

    do{
        counter++;
        temp_path = prefix_path + std::to_string(counter) + ".txt";
    }while(is_file_exist(temp_path.c_str()));

    return (prefix_path + std::to_string(counter) + ".txt");
}
