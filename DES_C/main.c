#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "des_noop.h"

#define pp(x) printf("%10s = %02X %02X %02X %02X %02X %02X %02X %02X\n", \
    #x, x[0], x[1], x[2], x[3], x[4], x[5], x[6], x[7]);

#define TEXT_LENGTH 64

enum action {
    Decrypt, Encrypt
};

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
unsigned char bin2decimal(const char* bin_number, int index){
    int result = 0;
    for(int i = (TEXT_LENGTH/8) - 1; i >= 0; --i)
        if(bin_number[index++] == '1')
            result += power(2, i);
    return result;
}

//Tested
//Printing the 8-numbered binary value of the inserted unsigned char
void decimal2bin(unsigned char val){
    char result[8];
    for(int i = 1; i <= 8; ++i){
        unsigned char current_power = (unsigned char) power(2, 8 - i);
        if(val >= current_power){
            printf("1");
            val -= current_power;
        }else
            printf("0");
    }

}

// Argument order  - key, rounds, encrypt(1)/decrypt(0), text
//input example "0100100100100100100100001001001100001001001101001000101000100100 16 0 0100100100100100100100100100100100100100100100100100100100100100"
int main(int argc, char* argv[]){
    /* Argument assignment*/
    char* key_parameter = argv[1]; //Key
    int rounds = strtol(argv[2], NULL, 10); // Number of Rounds
    enum action Dec_or_Enc = strcmp(argv[3], "0") == 0 ? Decrypt : Encrypt; //Encrypt or Decrypt
    char* text_parameter = argv[4]; //Text

    unsigned char key[TEXT_LENGTH/8];
    unsigned char text[TEXT_LENGTH/8];
    unsigned char text_output[TEXT_LENGTH/8];

    int param_i = 0, unsigned_char_i = 0;
    for(; param_i < TEXT_LENGTH/8; param_i++, unsigned_char_i += 8){
        key[param_i] = bin2decimal(key_parameter, unsigned_char_i);
        text[param_i] = bin2decimal(text_parameter, unsigned_char_i);
    }

    if(Dec_or_Enc == Decrypt)
        DES_decrypt(text, key, text_output, rounds);
    else
        DES_encrypt(text, key, text_output, rounds);

    //printing the text output as a binary-based number
    for(int i = 0; i < TEXT_LENGTH/8; ++i)
        decimal2bin(text_output[i]);

//     printf("Simba!!!!");



/*    printf("\n\n");
    DES_encrypt(text, key, text_output, rounds);

    for(int i = 0; i < TEXT_LENGTH/8; ++i)
        decimal2bin(text_output[i]);

    printf("\n\nSimba!!!!\n\n");
    unsigned char text_output2[TEXT_LENGTH/8];
    DES_decrypt(text_output, key, text_output2, rounds);

    for(int i = 0; i < TEXT_LENGTH/8; ++i)
        decimal2bin(text_output2[i]);*/


    return 0;
}
