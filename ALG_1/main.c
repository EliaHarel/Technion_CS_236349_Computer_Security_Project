#include "des_noop.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define pp(x) printf("%10s = %02X %02X %02X %02X %02X %02X %02X %02X\n", \
	  #x, x[0], x[1], x[2], x[3], x[4], x[5], x[6], x[7]);

// P * C
// S5 > S1
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

void matrixMul (int r1, int c1, int c2, double A1[r1][c1], double A2[c1][c2], double A3[r1][c2]) {
    for (int i=0; i < r1; i++) {
        for (int j = 0; j < c2; j++) {
            for (int k = 0; k < c1; k++) {
                A3[i][j] = A3[i][j] + A1[i][k] * A2[k][j];
            }
        }
    }
}

int main(){

	// in-4out
	double S1[32] = {0.0625, 0.03125, 0.0625, 0.0625, 0.09375, 0.09375,	0.0625,	0, 0, 0.09375, 0.125, 0.03125,	0.0625,	0.03125,	0.0625,	0.125,	0.0625,	0.09375, 0.0625, 0.0625, 0.03125, 0.03125, 0.0625, 0.125, 0.125, 0.03125, 0, 0.09375, 0.0625, 0.09375, 0.0625, 0};
	double S5[32] = {0,	0.125, 0.125, 0, 0.09375, 0, 0.03125, 0.125, 0.0625, 0,	0.0625,	0.125, 0.09375,	0.09375, 0.0625, 0,	0.125, 0, 0, 0.125,	0.03125, 0.125,	0.09375, 0,	0.0625,	0.125, 0.0625, 0, 0.03125, 0.03125,	0.0625,	0.125};

	for (int i=0; i<256; i++) {
		for (int j=0; j<256; j++) {
			Pin = (i&15);
			Pout = ((i&240)>>4);
			Cin = ((j&240)>>4);
			Cout = (j&15);
			if (Cin!=Pin) M1[0][i][j] = 0;
			else {
				Fout = Cout^Pout;
				Fin = ((i&4)<<2);
				M1[0][i][j] = S5[Fin+Fout];
				Fin = ((i&4)<<2)^16;
				M1[1][i][j] = S5[Fin+Fout];
				Fin = ((i&1)<<4);
				M1[2][i][j] = S1[Fin+Fout];
				Fin = ((i&1)<<4)^16;
				M1[3][i][j] = S1[Fin+Fout];
			}
		}
	}
	matrixMul(256, 256, 256, M1[0], M1[2], M2[0]);
	matrixMul(256, 256, 256, M1[0], M1[3], M2[1]);
	matrixMul(256, 256, 256, M1[1], M1[2], M2[2]);
	matrixMul(256, 256, 256, M1[1], M1[3], M2[3]);

	for(int i=0; i<16; i++) {
		int first = ((i&12)>>2);
	    int last = (i&3);
	    matrixMul(256, 256, 256, M2[first], M2[last], M4[i]);
	}

	for(int i=0; i<64; i++) {
		int first = ((i&48)>>4);
	    int last = (i&15);
	    matrixMul(256, 256, 256, M2[first], M4[last], M6[i]);
	}

	static unsigned char key[8] = {0, 0, 0, 0, 0, 0, 0, 0};

	for (int t=0; t<10; t++) {
		static unsigned char plain[8] = { 00, 00, 00, 00, 00, 00, 00, 00 };
		unsigned char cipher[8] = { 00, 00, 00, 00, 00, 00, 00, 00 };

		long long A[256][256] = { 0, };

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

		int inInx = 0;
		int outInx = 0;

		int texts = 50000;

		srand(time(NULL));
		for (int i = 0; i < texts; i++) {
			for (int j = 0; j < 8; j++) {
				plain[j] = ((plain[(j + 1) % 8] + rand() % 256) % 256) ^ cipher[j];
			}

			DES_encrypt(plain, key, cipher);

			iL1 = (plain[0] & 1) >> 0;
			iL2 = (plain[1] & 4) >> 2;
			iL3 = (plain[3] & 128) >> 7;
			iL4 = (plain[0] & 32) >> 5;

			iR1 = (plain[5] & 128) >> 7;
			iR2 = (plain[6] & 128) >> 7;
			iR3 = (plain[6] & 2) >> 1;
			iR4 = (plain[7] & 2) >> 1;

			oL1 = (cipher[4] & 1) >> 0;
			oL2 = (cipher[5] & 4) >> 2;
			oL3 = (cipher[7] & 128) >> 7;
			oL4 = (cipher[4] & 32) >> 5;

			oR1 = (cipher[1] & 128) >> 7;
			oR2 = (cipher[2] & 128) >> 7;
			oR3 = (cipher[2] & 2) >> 1;
			oR4 = (cipher[3] & 2) >> 1;

			inInx =
			    iL1 * 128 + iL2 * 64 + iL3 * 32 + iL4 * 16 + iR1 * 8 +
			    iR2 * 4 + iR3 * 2 + iR4 * 1;
			outInx =
			    oL1 * 128 + oL2 * 64 + oL3 * 32 + oL4 * 16 + oR1 * 8 +
			    oR2 * 4 + oR3 * 2 + oR4 * 1;
			A[inInx][outInx]++;
		}

		int count = 0;
		double sumr = 0;
        for (int i = 0; i < 256; i++) {
            for (int j = 0; j < 256; j++) {
            	sumr = sumr + (A[i][j]-texts/256.0)*(M6[0][i][j]*texts-texts/256.0);
            }
        }

	    for (int m=0; m < 64; m++) {
	    	double sum = 0;
	        for (int i = 0; i < 256; i++) {
	            for (int j = 0; j < 256; j++) {

	            	sum = sum + (A[i][j]-texts/256.0)*(M6[m][i][j]*texts-texts/256.0);
	            }
	        }
	        if (sum>=sumr) count++;
	    }
	    printf("%d \n", count);
 }
}
