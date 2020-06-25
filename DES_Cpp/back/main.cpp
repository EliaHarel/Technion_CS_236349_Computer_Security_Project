#include <iostream>
#include <string>
//#include <cmath>
//#include <ctime>
//#include <sys/stat.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <unistd.h>


#define TEXT_LENGTH 64

//Tested
unsigned char power(int base, int exponent);

//Tested
unsigned char bin2decimal(const char* bin_number, int index);

bool file_exists(const char* fname);

char* create_binary_key();

unsigned char* create_text(unsigned char* text);

void decimal2bin(unsigned char* binary, FILE* fp);

char* append_strings(char* chars[], int num);

char* append_2_strings(char* a, char* b);

char* append_3_strings(char* a, char* b, char* c);

char* append_4_strings(char* a, char* b, char* c, char* d);

char* append_5_strings(char* a, char* b, char* c, char* d, char* e);

char* find_available_file_name(char* prefix_path);


int main(int argc, char* argv[]){

    int rounds = strtol(argv[1], nullptr, 10); // number of rounds
    int num_of_plaintexts = strtol(argv[2], nullptr, 10); // number of plaintexts
    char* path_to_data_directory = argv[3];

    char* binary_key; // two options for key, because it is an "optional" parameter
    unsigned char key[TEXT_LENGTH/8];
    unsigned char plain_text[TEXT_LENGTH/8];

    unsigned char ciphertext_output[8];

    srand(time(nullptr));
    char temp_char[64];
    std::string slash = "/";
    struct stat st = {0};

    //binary_key is a string of a binary number
    if(argc == 4)
        binary_key = create_binary_key();
    else
        binary_key = argv[4];


    for(int param_i = 0, unsigned_char_i = 0; param_i < TEXT_LENGTH/8; param_i++, unsigned_char_i += 8)
        key[param_i] = bin2decimal(binary_key, unsigned_char_i);


    /*
        path block
    */
    sprintf(temp_char, "%d", rounds);
    std::string path_to_rounds = append_4_strings(path_to_data_directory, slash, temp_char, "_rounds");

    if(stat(path_to_rounds, &st) == -1)// create directory if does not exist
        mkdir(path_to_rounds, 0700);


    char* path_to_key = append_4_strings(path_to_rounds, slash, binary_key, "_key");
    if(stat(path_to_key, &st) == -1){
        mkdir(path_to_key, 0700);
    }


    sprintf(temp_char, "%d", num_of_plaintexts);
    char* path_to_num_of_plaintexts = append_4_strings(path_to_key, slash, temp_char, "_plaintexts");

    if(stat(path_to_num_of_plaintexts, &st) == -1) //create directory if does not exist
        mkdir(path_to_num_of_plaintexts, 0700);


    /*
         block
    */
    char* prefix_path = append_2_strings(path_to_num_of_plaintexts, slash);
    char* final_path = find_available_file_name(prefix_path);


    FILE* fp;
    fp = fopen(final_path, "w+");
    sprintf(temp_char, "%d", rounds);
    char* first_line = append_5_strings("rounds: ", temp_char, " key: ", binary_key, "\n");
    fprintf(fp, first_line);


    for(int i = 0; i < num_of_plaintexts; ++i){
//        create_text(plain_text);
//decimal2bin(plain_text, stdout);
        printf("plain before is:\n");
        for(int j = 0; j < 7; ++j){
            printf("%u, ", plain_text[j]);
        }
        printf("%u\n", plain_text[7]);

        DES_encrypt(plain_text, key, ciphertext_output);
        decimal2bin(plain_text, fp);
        decimal2bin(plain_text, stdout);
//printf("\nplain is: %s\n", plain_text);
        printf("\ncipher is:\n", ciphertext_output);
        decimal2bin(ciphertext_output, stdout);
        fflush(stdout);
        fprintf(fp, " ");
        decimal2bin(ciphertext_output, fp);
        fprintf(fp, "\n");
    }

    if(argc == 4)
        free(binary_key);
    free(path_to_rounds);
    free(path_to_key);
    free(path_to_num_of_plaintexts);
    free(prefix_path);
    free(first_line);
    free(final_path);
    fclose(fp);

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
unsigned char bin2decimal(const char* bin_number, int index){
    int result = 0;
    for(int i = (TEXT_LENGTH/8) - 1; i >= 0; --i)
        if(bin_number[index++] == '1')
            result += power(2, i);
    return result;
}

bool file_exists(const char* fname){
    FILE* file;
    file = fopen(fname, "r");
    if(file){
        fclose(file);
        return true;
    }
    return false;
}

unsigned char* create_text(unsigned char* text){
//    unsigned char *text = malloc(sizeof(unsigned char)*8);
//
    for(int i = 0; i < 8; ++i){
//        text[i] = rand()%256;
        int x = rand()%256;
        text[i] = x;
    }
    return text;
}

char* create_binary_key(){
    char* key = new char[64];
    for(int i = 0; i < 64; ++i)
        key[i] = rand()%2 ? '1' : '0';
    return key;
}

void decimal2bin(unsigned char* binary, FILE* fp){
    for(int i = 0; i < 8; ++i){
        unsigned char val = binary[i];
        for(int j = 1; j <= 8; ++j){
            unsigned char current_power = (unsigned char) power(2, 8 - j);
            if(val >= current_power){
                fprintf(fp, "1");
                val -= current_power;
            }else
                fprintf(fp, "0");
        }
    }
}

char* append_strings(char* chars[], int num){
    int total_length = 0;
    for(int i = 0; i < num; ++i)
        total_length += strlen(chars[i]);

    char* new_str = new char[total_length + 1];
    total_length = 0;

    for(int i = 0; i < num; ++i){
        strcpy(new_str + total_length, chars[i]);
        total_length += strlen(chars[i]);
    }
    return new_str;

}

char* append_2_strings(char* a, char* b){
    char* chars[] = {a, b};
    return append_strings(chars, 2);
}

char* append_3_strings(char* a, char* b, char* c){
    char* chars[] = {a, b, c};
    return append_strings(chars, 3);
}

char* append_4_strings(char* a, char* b, char* c, char* d){
    char* chars[] = {a, b, c, d};
    return append_strings(chars, 4);
}

char* append_5_strings(char* a, char* b, char* c, char* d, char* e){
    char* chars[] = {a, b, c, d, e};
    return append_strings(chars, 5);
}

char* find_available_file_name(char* prefix_path){
    char temp_char[64];
    int counter = 0;
    char* temp_path = nullptr;

    do{
        free(temp_path);
        sprintf(temp_char, "%d", counter);
        counter++;
        temp_path = append_3_strings(prefix_path, temp_char, ".txt");
    }while(file_exists(temp_path));

    free(temp_path);
    return append_3_strings(prefix_path, temp_char, ".txt");
}

