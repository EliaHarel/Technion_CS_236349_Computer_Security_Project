#include "des_noop.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(){
	unsigned char kkkk = 255;
	int r = kkkk;
	printf("%d\n", r);
	return 0;
}


char* int2bin(int num){
	char daniel[8] = malloc(8);
	for (int i = 7; i >= 0; --i){
		daniel[i] = num%2;
		num = num/2;
	}
	return daniel;
}