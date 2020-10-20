//
// Created by EliaHarel on 20/10/2020.
//

#include "Tables.h"
#include <vector>

std::vector<std::vector<std::vector<std::vector<double>>>> pre_calculated_mat;


static char s1[64] = {

        0XE, 0, 0X4, 0XF, 0XD, 0X7, 0X1, 0X4,
        0X2, 0XE, 0XF, 0X2, 0XB, 0XD, 0X8, 0X1,
        0X3, 0XA, 0XA, 0X6, 0X6, 0XC, 0XC, 0XB,
        0X5, 0X9, 0X9, 0X5, 0, 0X3, 0X7, 0X8,
        0X4, 0XF, 0X1, 0XC, 0XE, 0X8, 0X8, 0X2,
        0XD, 0X4, 0X6, 0X9, 0X2, 0X1, 0XB, 0X7,
        0XF, 0X5, 0XC, 0XB, 0X9, 0X3, 0X7, 0XE,
        0X3, 0XA, 0XA, 0, 0X5, 0X6, 0, 0XD
};

static char s5[64] = {
        0X2, 0XE, 0XC, 0XB, 0X4, 0X2, 0X1, 0XC,
        0X7, 0X4, 0XA, 0X7, 0XB, 0XD, 0X6, 0X1,
        0X8, 0X5, 0X5, 0, 0X3, 0XF, 0XF, 0XA,
        0XD, 0X3, 0, 0X9, 0XE, 0X8, 0X9, 0X6,
        0X4, 0XB, 0X2, 0X8, 0X1, 0XC, 0XB, 0X7,
        0XA, 0X1, 0XD, 0XE, 0X7, 0X2, 0X8, 0XD,
        0XF, 0X6, 0X9, 0XF, 0XC, 0, 0X5, 0X9,
        0X6, 0XA, 0X3, 0X4, 0, 0X5, 0XE, 0X3};


//Mi[2^i][256][256]
//i הוא מספר השלבים של התכונה, 2^i הוא מספר המטריצות האפשריות.
//256=2^8 – כל הערכים האפשריים עבור 8 ביטי P (בצד ימין אלו הביטים במיקומים של הפלט של S1, בצד ימין אלו הביטים במיקומים של הפלט של S5).
//256=2^8 – כל הערכים האפשריים עבור 8 ביטי C (בצד ימין אלו הביטים במיקומים של הפלט של S5, בצד ימין אלו הביטים במיקומים של הפלט של S1).



static double M1[4][256][256]; //S5-0, S5-1, S1-0, S1-1
static double M2[4][256][256];
static double M4[16][256][256];
static double M6[64][256][256];

//תכל'ס הכוונה ב-Pin, Cin ל-4 הביטים בצד הימני, ו-Pout, Cout בצד השמאלי.
int Pin = 0;
int Pout = 0;
int Cin = 0;
int Cout = 0;
int Fin = 0;
int Fout = 0;

//void matrixMul(int r1, int c1, int c2, std::vector<std::vector<double>> A1[r1][c1], double A2[c1][c2], double A3[r1][c2]){
void matrixMul(std::vector<std::vector<double>> A1, std::vector<std::vector<double>> A2, std::vector<std::vector<double>> A3){
    int r1 = A1.size();
    int c2 = A2[0].size();
    int c1 = A2.size();
    for(int j = 0; j < c2; j++){
        for(int k = 0; k < c1; k++){
            for(int i = 0; i < r1; i++){
                A3[i][j] = A3[i][j] + A1[i][k]*A2[k][j];
            }
        }
    }
}

int main(){
//    הטבלה הראשונה שחישבנו ביחד באקסל מה שנקרא "S1 HELP"

    // in-4out
    double S1[32] = {0.0625, 0.03125, 0.0625, 0.0625, 0.09375, 0.09375, 0.0625, 0, 0, 0.09375, 0.125, 0.03125,
                     0.0625, 0.03125, 0.0625, 0.125, 0.0625, 0.09375, 0.0625, 0.0625, 0.03125, 0.03125,
                     0.0625, 0.125, 0.125, 0.03125, 0, 0.09375, 0.0625, 0.09375, 0.0625, 0};
    double S5[32] = {0, 0.125, 0.125, 0, 0.09375, 0, 0.03125, 0.125, 0.0625, 0, 0.0625, 0.125, 0.09375,
                     0.09375, 0.0625, 0, 0.125, 0, 0, 0.125, 0.03125, 0.125, 0.09375, 0, 0.0625, 0.125,
                     0.0625, 0, 0.03125, 0.03125, 0.0625, 0.125};

//    חישוב המטריצה עבור שלב בודד המבוסס על S5 וביט המפתח הוא 0.
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
//    חישוב המטריצה עבור שלב בודד המבוסס על S5 וביט המפתח הוא 1.
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
//    חישוב המטריצה עבור שלב בודד המבוסס על S1 וביט המפתח הוא 0.
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
//    חישוב המטריצה עבור שלב בודד המבוסס על S1 וביט המפתח הוא 1.
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
//    חישוב 4 המטריצות האפשריות עבור 2 שלבים.
    matrixMul(256, 256, 256, M1[0], M1[2], M2[0]);
    matrixMul(256, 256, 256, M1[0], M1[3], M2[1]);
    matrixMul(256, 256, 256, M1[1], M1[2], M2[2]);
    matrixMul(256, 256, 256, M1[1], M1[3], M2[3]);

//    חישוב 16 המטריצות עבור 4 שלבים:
    for(int i = 0; i < 16; i++){
        int first = ((i&12) >> 2);
        int last = (i&3);
        matrixMul(256, 256, 256, M2[first], M2[last], M4[i]);
    }
//    חישוב 64 המטריצות עבור 6 שלבים:
    for(int i = 0; i < 64; i++){
        int first = ((i&60) >> 2);
        int last = (i&3);
        matrixMul(256, 256, 256, M4[first], M2[last], M6[i]);
    }

    static unsigned char key[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    for(int times = 0; times < 5; times++){

        static unsigned char plain[8] = {00, 00, 00, 00, 00, 00, 00, 00};
        unsigned char cipher[8] = {00, 00, 00, 00, 00, 00, 00, 00};

