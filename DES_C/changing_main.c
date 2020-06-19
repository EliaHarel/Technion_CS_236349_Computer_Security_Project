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

/*                WAS NOT Tested!!!!!                */
//Printing the 8-numbered binary value of the inserted unsigned char
void decimal2bin2file(unsigned char val, FILE *fp){
    char result[8];
    for(int i = 1; i <= 8; ++i){
        unsigned char current_power = (unsigned char) power(2, 8 - i);
        if(val >= current_power){
            fprintf(fp, "1");
            val -= current_power;
        }else
            fprintf(fp, "0");
    }

}

// Argument order  - key, rounds, encrypt(1)/decrypt(0), text
//input example "0100100100100100100100001001001100001001001101001000101000100100 16 0 0100100100100100100100100100100100100100100100100100100100100100"
void Des(int argc, char* argv[], FILE *dst){
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
    // for(int i = 0; i < TEXT_LENGTH/8; ++i)
    //     decimal2bin(text_output[i]);
    for(int i = 0; i < TEXT_LENGTH/8; ++i)
        decimal2bin2file(text_output[i], dst);

}

// arguments order: 1-key, 2-rounds, 3-mode, 4-source path, 5-dest path
int main(int argc, char* argv[]){

    char* key_parameter = argv[1]; //Key
    int rounds = strtol(argv[2], NULL, 10); // Number of Rounds
    enum action Dec_or_Enc = strcmp(argv[3], "0") == 0 ? Decrypt : Encrypt; //Encrypt or Decrypt
    const char* src_path = argv[4];
    const char* dst_path = argv[5];

    for(int param_i = 0, unsigned_char_i = 0; param_i < TEXT_LENGTH/8; param_i++, unsigned_char_i += 8) 
        key[param_i] = bin2decimal(key_parameter, unsigned_char_i);

    FILE *plain_cipher_input =  fopen(src_path, "r");
    if (plain_cipher_input == NULL) 
        return 1;

    FILE *plain_cipher_output =  fopen(src_path, "w");
    if (plain_cipher_output == NULL){
        fclose(plain_cipher_input);
        return 1;
    } 

    char plain[64];
    int eof;
    while (fgets(plain,sizeof(plain), plain_cipher_input)) {
        int param_i = 0, unsigned_char_i = 0;
        for(; param_i < TEXT_LENGTH/8; param_i++, unsigned_char_i += 8){
            text[param_i] = bin2decimal(text_parameter, unsigned_char_i);
        }
        Des(); //TODO: fill this with the arguments

        if((eof = fgetc(plain_cipher_input) != EOF);
    }
    
    fclose(plain_cipher_input);
    fclose(plain_cipher_output);
    return 0;

}
