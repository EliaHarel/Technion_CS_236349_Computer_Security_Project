
#include <cmath>
#include "Tables.h"


namespace preCalcTables {
    double S1Help[32] = {0.0625, 0.03125, 0.0625, 0.0625, 0.09375, 0.09375, 0.0625, 0, 0, 0.09375, 0.125,
                         0.03125,
                         0.0625, 0.03125, 0.0625, 0.125, 0.0625, 0.09375, 0.0625, 0.0625, 0.03125, 0.03125,
                         0.0625, 0.125, 0.125, 0.03125, 0, 0.09375, 0.0625, 0.09375, 0.0625, 0};
    double S5Help[32] = {0, 0.125, 0.125, 0, 0.09375, 0, 0.03125, 0.125, 0.0625, 0, 0.0625, 0.125, 0.09375,
                         0.09375, 0.0625, 0, 0.125, 0, 0, 0.125, 0.03125, 0.125, 0.09375, 0, 0.0625, 0.125,
                         0.0625, 0, 0.03125, 0.03125, 0.0625, 0.125};

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

}
using namespace preCalcTables;


// pre_calculated_mat[16][4][matrix_size][matrix_size];
// technically it is --> Mi[2^i][256][256]
//      Note: pre_calculated_mat[1][4][matrix_size][matrix_size] === S5-0, S5-1, S1-0, S1-1
// vvvvd pre_calculated_mat;

//void matrixMul(int r1, int c1, int c2, vvd A[r1][c1], double B[c1][c2], double C[r1][c2]){
static void matrixMul(vvd& A, vvd& B, vvd& C){
    for(int i = 0; i < A.size(); i ++){
        for(int j = 0; j < B.at(0).size(); j ++){
            for(int k = 0; k < A.at(0).size(); k ++){
                C.at(i).at(j) = C.at(i).at(j) + A.at(i).at(k)*B.at(k).at(j);
            }
        }
    }
}


static void calcTableOneAndTwo(vvvvd& pre_calculated_mat){
    int Pin;
    int Pout;
    int Cin;
    int Cout;
    int Fin;
    int Fout;

    for(int i = 0; i < 256; i ++){
        for(int j = 0; j < 256; j ++){
            Pin = (i&15);
            Pout = ((i&240) >> 4);
            Cin = ((j&240) >> 4);
            Cout = (j&15);
            if( Cin != Pin )
                pre_calculated_mat.at(1).at(0).at(i).at(j) = 0;
            else{
                Fin = ((i&4) << 2);
                Fout = Cout^Pout;
                pre_calculated_mat.at(1).at(0).at(i).at(j) = S5Help[Fin + Fout];
            }
        }
    }
    for(int i = 0; i < 256; i ++){
        for(int j = 0; j < 256; j ++){
            Pin = (i&15);
            Pout = ((i&240) >> 4);
            Cin = ((j&240) >> 4);
            Cout = (j&15);
            if( Cin != Pin )
                pre_calculated_mat.at(1).at(1).at(i).at(j) = 0;
            else{
                Fin = ((i&4) << 2)^16;
                Fout = Cout^Pout;
                pre_calculated_mat.at(1).at(1).at(i).at(j) = S5Help[Fin + Fout];
            }
        }
    }
    for(int i = 0; i < 256; i ++){
        for(int j = 0; j < 256; j ++){
            Pin = (i&15);
            Pout = ((i&240) >> 4);
            Cin = ((j&240) >> 4);
            Cout = (j&15);
            if( Cin != Pin )
                pre_calculated_mat.at(1).at(2).at(i).at(j) = 0;
            else{
                Fin = ((i&1) << 4);
                Fout = Cout^Pout;
                pre_calculated_mat.at(1).at(2).at(i).at(j) = S1Help[Fin + Fout];
            }
        }
    }
    for(int i = 0; i < 256; i ++){
        for(int j = 0; j < 256; j ++){
            Pin = (i&15);
            Pout = ((i&240) >> 4);
            Cin = ((j&240) >> 4);
            Cout = (j&15);
            if( Cin != Pin )
                pre_calculated_mat.at(1).at(3).at(i).at(j) = 0;
            else{
                Fin = ((i&1) << 4)^16;
                Fout = Cout^Pout;
                pre_calculated_mat.at(1).at(3).at(i).at(j) = S1Help[Fin + Fout];
            }
        }
    }
    matrixMul(pre_calculated_mat.at(1).at(0), pre_calculated_mat.at(1).at(2),
              pre_calculated_mat.at(2).at(0));
    matrixMul(pre_calculated_mat.at(1).at(0), pre_calculated_mat.at(1).at(3),
              pre_calculated_mat.at(2).at(1));
    matrixMul(pre_calculated_mat.at(1).at(1), pre_calculated_mat.at(1).at(2),
              pre_calculated_mat.at(2).at(2));
    matrixMul(pre_calculated_mat.at(1).at(1), pre_calculated_mat.at(1).at(3),
              pre_calculated_mat.at(2).at(3));

}

void CalcTables(int rounds, vvvvd& pre_calculated_mat, int ready_rounds){
    if( ready_rounds == 0 ){
        calcTableOneAndTwo(pre_calculated_mat);
        ready_rounds = 2;
    }

    for(int i = ready_rounds + 2; i <= rounds; i += 2){
        for(int j = 0; j < pow(2, i); j ++){
            int first = ((j&(int) (pow(2, i) - 4)) >> 2);
            int last = (j&3);
            matrixMul(pre_calculated_mat.at(i - 2).at(first), pre_calculated_mat.at(2).at(last),
                      pre_calculated_mat.at(i).at(j));
        }
    }
}

