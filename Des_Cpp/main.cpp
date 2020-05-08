#include "des_noop.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

static char s1[64] = {
        0XE, 0, 0X4, 0XF, 0XD, 0X7, 0X1, 0X4,
        0X2, 0XE, 0XF, 0X2, 0XB, 0XD, 0X8, 0X1,
        0X3, 0XA, 0XA, 0X6, 0X6, 0XC, 0XC, 0XB,
        0X5, 0X9, 0X9, 0X5, 0, 0X3, 0X7, 0X8,
        0X4, 0XF, 0X1, 0XC, 0XE, 0X8, 0X8, 0X2,
        0XD, 0X4, 0X6, 0X9, 0X2, 0X1, 0XB, 0X7,
        0XF, 0X5, 0XC, 0XB, 0X9, 0X3, 0X7, 0XE,
        0X3, 0XA, 0XA, 0, 0X5, 0X6, 0, 0XD};

static char s5[64] = {
        0X2, 0XE, 0XC, 0XB, 0X4, 0X2, 0X1, 0XC,
        0X7, 0X4, 0XA, 0X7, 0XB, 0XD, 0X6, 0X1,
        0X8, 0X5, 0X5, 0, 0X3, 0XF, 0XF, 0XA,
        0XD, 0X3, 0, 0X9, 0XE, 0X8, 0X9, 0X6,
        0X4, 0XB, 0X2, 0X8, 0X1, 0XC, 0XB, 0X7,
        0XA, 0X1, 0XD, 0XE, 0X7, 0X2, 0X8, 0XD,
        0XF, 0X6, 0X9, 0XF, 0XC, 0, 0X5, 0X9,
        0X6, 0XA, 0X3, 0X4, 0, 0X5, 0XE, 0X3};


Mi[2^i][256][256];


static double M1[4][256][256]; //S5-0, S5-1, S1-0, S1-1
static double M2[4][256][256];
static double M4[16][256][256];
static double M6[64][256][256];

int Pin = 0;
int Pout = 0;
int Cin = 0;
int Cout = 0;
int Fin = 0;
int Fout = 0;

void matrixMul(int r1, int c1, int c2, double A1[r1][c1], double A2[c1][c2], double A3[r1][c2]){
    for(int i = 0; i < r1; i++){
        for(int j = 0; j < c2; j++){
            for(int k = 0; k < c1; k++){
                A3[i][j] = A3[i][j] + A1[i][k]*A2[k][j];
            }
        }
    }
}

int main(){

    // in-4out
    double S1[32] = {0.0625, 0.03125, 0.0625, 0.0625, 0.09375, 0.09375, 0.0625, 0, 0, 0.09375, 0.125, 0.03125,
                     0.0625, 0.03125, 0.0625, 0.125, 0.0625, 0.09375, 0.0625, 0.0625, 0.03125, 0.03125,
                     0.0625, 0.125, 0.125, 0.03125, 0, 0.09375, 0.0625, 0.09375, 0.0625, 0};
    double S5[32] = {0, 0.125, 0.125, 0, 0.09375, 0, 0.03125, 0.125, 0.0625, 0, 0.0625, 0.125, 0.09375,
                     0.09375, 0.0625, 0, 0.125, 0, 0, 0.125, 0.03125, 0.125, 0.09375, 0, 0.0625, 0.125,
                     0.0625, 0, 0.03125, 0.03125, 0.0625, 0.125};

    for(int i = 0; i < 256; i++){
        for(int j = 0; j < 256; j++){
            Pin = (i&15);
            Pout = ((i&240) >> 4);
            Cin = ((j&240) >> 4);
            Cout = (j&15);
            if(Cin != Pin) M1[0][i][j] = 0;
            else{
                Fin = ((i&4) << 2);
                Fout = Cout^Pout;
                M1[0][i][j] = S5[Fin + Fout];
            }
        }
    }
    for(int i = 0; i < 256; i++){
        for(int j = 0; j < 256; j++){
            Pin = (i&15);
            Pout = ((i&240) >> 4);
            Cin = ((j&240) >> 4);
            Cout = (j&15);
            if(Cin != Pin) M1[1][i][j] = 0;
            else{
                Fin = ((i&4) << 2)^16;
                Fout = Cout^Pout;
                M1[1][i][j] = S5[Fin + Fout];
            }
        }
    }
    for(int i = 0; i < 256; i++){
        for(int j = 0; j < 256; j++){
            Pin = (i&15);
            Pout = ((i&240) >> 4);
            Cin = ((j&240) >> 4);
            Cout = (j&15);
            if(Cin != Pin) M1[2][i][j] = 0;
            else{
                Fin = ((i&1) << 4);
                Fout = Cout^Pout;
                M1[2][i][j] = S1[Fin + Fout];
            }
        }
    }
    for(int i = 0; i < 256; i++){
        for(int j = 0; j < 256; j++){
            Pin = (i&15);
            Pout = ((i&240) >> 4);
            Cin = ((j&240) >> 4);
            Cout = (j&15);
            if(Cin != Pin) M1[3][i][j] = 0;
            else{
                Fin = ((i&1) << 4)^16;
                Fout = Cout^Pout;
                M1[3][i][j] = S1[Fin + Fout];
            }
        }
    }
    matrixMul(256, 256, 256, M1[0], M1[2], M2[0]);
    matrixMul(256, 256, 256, M1[0], M1[3], M2[1]);
    matrixMul(256, 256, 256, M1[1], M1[2], M2[2]);
    matrixMul(256, 256, 256, M1[1], M1[3], M2[3]);

    for(int i = 0; i < 16; i++){
        int first = ((i&12) >> 2);
        int last = (i&3);
        matrixMul(256, 256, 256, M2[first], M2[last], M4[i]);
    }
    for(int i = 0; i < 64; i++){
        int first = ((i&60) >> 2);
        int last = (i&3);
        matrixMul(256, 256, 256, M4[first], M2[last], M6[i]);
    }

    static unsigned char key[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    for(int times = 0; times < 5; times++){

        static unsigned char plain[8] = {00, 00, 00, 00, 00, 00, 00, 00};
        unsigned char cipher[8] = {00, 00, 00, 00, 00, 00, 00, 00};

        static long keys[4096][256][256] = {0};

        int iL1 = 0;
        int iL2 = 0;
        int iL3 = 0;
        int iL4 = 0;
        int iR1 = 0;
        int iR2 = 0;
        int iR3 = 0;
        int iR4 = 0;
        int oL1 = 0;
        int oL2 = 0;
        int oL3 = 0;
        int oL4 = 0;
        int oR1 = 0;
        int oR2 = 0;
        int oR3 = 0;
        int oR4 = 0;

        int s1in = 0;
        int s1out = 0;

        int s5in = 0;
        int s5out = 0;

        int inInx = 0;
        int outInx = 0;

        /*
           17 -> 0 [7]
           18 -> 1 [5]
           19 -> 3 [0]
           20 -> 0 [2]

           1 -> 1 [0]
           2 -> 2 [0]
           3 -> 2 [6]
           4 -> 3 [6]
         */

        int texts = 2500;

        srand(time(NULL));
        for(int i = 0; i < texts; i++){
            for(int j = 0; j < 8; j++){
                plain[j] = ((plain[(j + 1)%8] + rand()%256)%256)^cipher[j];
            }

            DES_encrypt(plain, key, cipher);
            s1in = ((plain[7]&1) << 5) + ((plain[4]&248) >> 3);
            s5in = ((cipher[5]&1) << 5) + ((cipher[6]&248) >> 3);

            iL1 = (plain[4]&1) >> 0;
            iL2 = (plain[5]&4) >> 2;
            iL3 = (plain[7]&128) >> 7;
            iL4 = (plain[4]&32) >> 5;
            oR1 = (cipher[5]&128) >> 7;
            oR2 = (cipher[6]&128) >> 7;
            oR3 = (cipher[6]&2) >> 1;
            oR4 = (cipher[7]&2) >> 1;

            for(int k = 0; k < 4096; k++){

                s1out = s1[s1in^((k&4032) >> 6)];
                s5out = s5[s5in^((k&63))];

                iR1 = ((s1out&8) >> 3)^((plain[1]&128) >> 7);
                iR2 = ((s1out&4) >> 2)^((plain[2]&128) >> 7);
                iR3 = ((s1out&2) >> 1)^((plain[2]&2) >> 1);
                iR4 = ((s1out&1) >> 0)^((plain[3]&2) >> 1);

                oL1 = ((s5out&8) >> 3)^((cipher[0]&1) >> 0);
                oL2 = ((s5out&4) >> 2)^((cipher[1]&4) >> 2);
                oL3 = ((s5out&2) >> 1)^((cipher[3]&128) >> 7);
                oL4 = ((s5out&1) >> 0)^((cipher[0]&32) >> 5);

                inInx =
                        iL1*128 + iL2*64 + iL3*32 + iL4*16 + iR1*8 +
                        iR2*4 + iR3*2 + iR4*1;
                outInx =
                        oL1*128 + oL2*64 + oL3*32 + oL4*16 + oR1*8 +
                        oR2*4 + oR3*2 + oR4*1;
                keys[k][inInx][outInx]++;
            }
        }
    }
}