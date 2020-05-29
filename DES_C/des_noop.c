#include "des_noop.h"

typedef unsigned char permTable[];


/*static permTable ip = {
        58, 50, 42, 34, 26, 18, 10, 2,
        60, 52, 44, 36, 28, 20, 12, 4,
        62, 54, 46, 38, 30, 22, 14, 6,
        64, 56, 48, 40, 32, 24, 16, 8,
        57, 49, 41, 33, 25, 17, 9, 1,
        59, 51, 43, 35, 27, 19, 11, 3,
        61, 53, 45, 37, 29, 21, 13, 5,
        63, 55, 47, 39, 31, 23, 15, 7}; 

static permTable fp = {
        40, 8, 48, 16, 56, 24, 64, 32,
        39, 7, 47, 15, 55, 23, 63, 31,
        38, 6, 46, 14, 54, 22, 62, 30,
        37, 5, 45, 13, 53, 21, 61, 29,
        36, 4, 44, 12, 52, 20, 60, 28,
        35, 3, 43, 11, 51, 19, 59, 27,
        34, 2, 42, 10, 50, 18, 58, 26,
        33, 1, 41, 9, 49, 17, 57, 25};*/


static permTable ip = {
    1,   2,  3,  4,  5,  6,  7,  8, 
    9,  10, 11, 12, 13, 14, 15, 16, 
    17, 18, 19, 20, 21, 22, 23, 24, 
    25, 26, 27, 28, 29, 30, 31, 32, 
    33, 34, 35, 36, 37, 38, 39, 40, 
    41, 42, 43, 44, 45, 46, 47, 48, 
    49, 50, 51, 52, 53, 54, 55, 56, 
    57, 58, 59, 60, 61, 62, 63, 64};

static permTable fp = {
    1,   2,  3,  4,  5,  6,  7,  8, 
    9,  10, 11, 12, 13, 14, 15, 16, 
    17, 18, 19, 20, 21, 22, 23, 24, 
    25, 26, 27, 28, 29, 30, 31, 32, 
    33, 34, 35, 36, 37, 38, 39, 40, 
    41, 42, 43, 44, 45, 46, 47, 48, 
    49, 50, 51, 52, 53, 54, 55, 56, 
    57, 58, 59, 60, 61, 62, 63, 64};

static permTable p = {
        16, 7, 20, 21,
        29, 12, 28, 17,
        1, 15, 23, 26,
        5, 18, 31, 10,
        2, 8, 24, 14,
        32, 27, 3, 9,
        19, 13, 30, 6,
        22, 11, 4, 25};

static char s[8][64] = {
        {
                0XE, 0,   0X4, 0XF, 0XD, 0X7, 0X1, 0X4,
                0X2, 0XE, 0XF, 0X2, 0XB, 0XD, 0X8, 0X1,
                0X3, 0XA, 0XA, 0X6, 0X6, 0XC, 0XC, 0XB,
                0X5, 0X9, 0X9, 0X5, 0,   0X3, 0X7, 0X8,
                0X4, 0XF, 0X1, 0XC, 0XE, 0X8, 0X8, 0X2,
                0XD, 0X4, 0X6, 0X9, 0X2, 0X1, 0XB, 0X7,
                0XF, 0X5, 0XC, 0XB, 0X9, 0X3, 0X7, 0XE,
                0X3, 0XA, 0XA, 0,   0X5, 0X6, 0,   0XD},
        {
                0XF, 0X3, 0X1, 0XD, 0X8, 0X4, 0XE, 0X7,
                0X6, 0XF, 0XB, 0X2, 0X3, 0X8, 0X4, 0XE,
                0X9, 0XC, 0X7, 0,   0X2, 0X1, 0XD, 0XA,
                0XC, 0X6, 0,   0X9, 0X5, 0XB, 0XA, 0X5,
                0,   0XD, 0XE, 0X8, 0X7, 0XA, 0XB, 0X1,
                0XA, 0X3, 0X4, 0XF, 0XD, 0X4, 0X1, 0X2,
                0X5, 0XB, 0X8, 0X6, 0XC, 0X7, 0X6, 0XC,
                0X9, 0,   0X3, 0X5, 0X2, 0XE, 0XF, 0X9},
        {
                0XA, 0XD, 0,   0X7, 0X9, 0,   0XE, 0X9,
                0X6, 0X3, 0X3, 0X4, 0XF, 0X6, 0X5, 0XA,
                0X1, 0X2, 0XD, 0X8, 0XC, 0X5, 0X7, 0XE,
                0XB, 0XC, 0X4, 0XB, 0X2, 0XF, 0X8, 0X1,
                0XD, 0X1, 0X6, 0XA, 0X4, 0XD, 0X9, 0,
                0X8, 0X6, 0XF, 0X9, 0X3, 0X8, 0,   0X7,
                0XB, 0X4, 0X1, 0XF, 0X2, 0XE, 0XC, 0X3,
                0X5, 0XB, 0XA, 0X5, 0XE, 0X2, 0X7, 0XC},
        {
                0X7, 0XD, 0XD, 0X8, 0XE, 0XB, 0X3, 0X5,
                0,   0X6, 0X6, 0XF, 0X9, 0,   0XA, 0X3,
                0X1, 0X4, 0X2, 0X7, 0X8, 0X2, 0X5, 0XC,
                0XB, 0X1, 0XC, 0XA, 0X4, 0XE, 0XF, 0X9,
                0XA, 0X3, 0X6, 0XF, 0X9, 0,   0,   0X6,
                0XC, 0XA, 0XB, 0X1, 0X7, 0XD, 0XD, 0X8,
                0XF, 0X9, 0X1, 0X4, 0X3, 0X5, 0XE, 0XB,
                0X5, 0XC, 0X2, 0X7, 0X8, 0X2, 0X4, 0XE},
        {
                0X2, 0XE, 0XC, 0XB, 0X4, 0X2, 0X1, 0XC,
                0X7, 0X4, 0XA, 0X7, 0XB, 0XD, 0X6, 0X1,
                0X8, 0X5, 0X5, 0,   0X3, 0XF, 0XF, 0XA,
                0XD, 0X3, 0,   0X9, 0XE, 0X8, 0X9, 0X6,
                0X4, 0XB, 0X2, 0X8, 0X1, 0XC, 0XB, 0X7,
                0XA, 0X1, 0XD, 0XE, 0X7, 0X2, 0X8, 0XD,
                0XF, 0X6, 0X9, 0XF, 0XC, 0,   0X5, 0X9,
                0X6, 0XA, 0X3, 0X4, 0,   0X5, 0XE, 0X3},
        {
                0XC, 0XA, 0X1, 0XF, 0XA, 0X4, 0XF, 0X2,
                0X9, 0X7, 0X2, 0XC, 0X6, 0X9, 0X8, 0X5,
                0,   0X6, 0XD, 0X1, 0X3, 0XD, 0X4, 0XE,
                0XE, 0,   0X7, 0XB, 0X5, 0X3, 0XB, 0X8,
                0X9, 0X4, 0XE, 0X3, 0XF, 0X2, 0X5, 0XC,
                0X2, 0X9, 0X8, 0X5, 0XC, 0XF, 0X3, 0XA,
                0X7, 0XB, 0,   0XE, 0X4, 0X1, 0XA, 0X7,
                0X1, 0X6, 0XD, 0,   0XB, 0X8, 0X6, 0XD},
        {
                0X4, 0XD, 0XB, 0,   0X2, 0XB, 0XE, 0X7,
                0XF, 0X4, 0,   0X9, 0X8, 0X1, 0XD, 0XA,
                0X3, 0XE, 0XC, 0X3, 0X9, 0X5, 0X7, 0XC,
                0X5, 0X2, 0XA, 0XF, 0X6, 0X8, 0X1, 0X6,
                0X1, 0X6, 0X4, 0XB, 0XB, 0XD, 0XD, 0X8,
                0XC, 0X1, 0X3, 0X4, 0X7, 0XA, 0XE, 0X7,
                0XA, 0X9, 0XF, 0X5, 0X6, 0,   0X8, 0XF,
                0,   0XE, 0X5, 0X2, 0X9, 0X3, 0X2, 0XC},
        {
                0XD, 0X1, 0X2, 0XF, 0X8, 0XD, 0X4, 0X8,
                0X6, 0XA, 0XF, 0X3, 0XB, 0X7, 0X1, 0X4,
                0XA, 0XC, 0X9, 0X5, 0X3, 0X6, 0XE, 0XB,
                0X5, 0,   0,   0XE, 0XC, 0X9, 0X7, 0X2,
                0X7, 0X2, 0XB, 0X1, 0X4, 0XE, 0X1, 0X7,
                0X9, 0X4, 0XC, 0XA, 0XE, 0X8, 0X2, 0XD,
                0,   0XF, 0X6, 0XC, 0XA, 0X9, 0XD, 0,
                0XF, 0X3, 0X3, 0X5, 0X5, 0X6, 0X8, 0XB}};

static permTable pc1 = {
        57, 49, 41, 33, 25, 17, 9,
        1, 58, 50, 42, 34, 26, 18,
        10, 2, 59, 51, 43, 35, 27,
        19, 11, 3, 60, 52, 44, 36,
        63, 55, 47, 39, 31, 23, 15,
        7, 62, 54, 46, 38, 30, 22,
        14, 6, 61, 53, 45, 37, 29,
        21, 13, 5, 28, 20, 12, 4};

static int rotations[] = {1, 2, 4, 6, 8, 10, 12, 14, 15, 17, 19, 21, 23, 25, 27, 28};

static permTable pc2 = {
        14, 17, 11, 24, 1, 5,
        3, 28, 15, 6, 21, 10,
        23, 19, 12, 4, 26, 8,
        16, 7, 27, 20, 13, 2,
        41, 52, 31, 37, 47, 55,
        30, 40, 51, 45, 33, 48,
        44, 49, 39, 56, 34, 53,
        46, 42, 50, 36, 29, 32};

static void permute();

static long f();

static void expand();

static void process_key();

void DES_encrypt(char plain[8], char key[8], char cipher[8], int rounds){
    long t[2];
    long t2[2];
    char processed_key[16][8];
    int i;

    process_key(key, processed_key);
    permute(plain, t, ip, 64);
    for(i = 0; i < rounds; i += 2){
        t[0] ^= f(t[1], processed_key[i]);
        t[1] ^= f(t[0], processed_key[i + 1]);
    }
    t2[0] = t[1];
    t2[1] = t[0];
    permute(t2, cipher, fp, 64);
}

void DES_decrypt(char cipher[8], char key[8], char plain[8], int rounds){
    long t[2];
    long t2[2];
    char processed_key[16][8];
    int i;

    process_key(key, processed_key);

    permute(cipher, t, ip, 64);



    for(i = 0; i < rounds; i += 2){
        t[0] ^= f(t[1], processed_key[15 - i]);
        t[1] ^= f(t[0], processed_key[15 - i - 1]);
    }



    t2[0] = t[1];
    t2[1] = t[0];
    permute(t2, plain, fp, 64);
}

static long f(long r, char subkey[8]){
    char er[8];
    char sres[4];
    long res;
    int i;

    expand(&r, er);
    for(i = 0; i < 8; i += 2){
        int inp = er[i]^subkey[i];
        int inp1 = er[i + 1]^subkey[i + 1];
        sres[i >> 1] = (s[i][inp] << 4)|s[i + 1][inp1];
    }
    permute(sres, &res, p, 32);
    return res;
}

static void permute(char* block, char* dest, unsigned char* permutation, int block_len){
    int i;
    int p;

    for(i = 0; i < block_len/8; i++)
        dest[i] = 0;

    for(i = 0; i < block_len; i++){
        p = permutation[i] - 1;
        dest[i >> 3] |= (((int) (block[p >> 3] << (p&7)))&0x80) >> (i&7);
    }
}

static void expand(inp, out)        /* E inplementation */
        register unsigned char inp[4], out[8]; /* At out, each byte contains 6 bits */
{
    out[0] = ((inp[0]&0370) >> 3)|((inp[3]&0001) << 5);
    out[1] = ((inp[0]&0037) << 1)|((inp[1]&0200) >> 7);
    out[2] = ((inp[1]&0370) >> 3)|((inp[0]&0001) << 5);
    out[3] = ((inp[1]&0037) << 1)|((inp[2]&0200) >> 7);
    out[4] = ((inp[2]&0370) >> 3)|((inp[1]&0001) << 5);
    out[5] = ((inp[2]&0037) << 1)|((inp[3]&0200) >> 7);
    out[6] = ((inp[3]&0370) >> 3)|((inp[2]&0001) << 5);
    out[7] = ((inp[3]&0037) << 1)|((inp[0]&0200) >> 7);
}

static void process_key(char key[8], char processed_key[16][8]){
    int i;

    for(i = 0; i < 16; i++){
        int rot = rotations[i];
        int j;

        for(j = 0; j < 8; j++)
            processed_key[i][j] = 0;

        for(j = 0; j < 48; j++){
            int src = pc1[(pc2[j] - 1 + rot)%28 + (pc2[j] > 28 ? 28 : 0)] - 1;
            processed_key[i][j/6] |= (((key[src >> 3] << (src&7)) >> 2)&0x20) >> (j%6);
        }
    }
}
