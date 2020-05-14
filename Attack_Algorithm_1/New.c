#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <math.h>

/*-------------- Values --------------*/
#define eps 	                    1e-12
#define INF     		            2e18
#define INTMax  	                0x7FFFFFFF
#define PI                          acos(-1.0)
/*-------------- Functions --------------*/
#define scan(n)                     scanf(" %d",&n)
#define scanD(n)                    scanf(" %lf",&n)
#define FOR(x, a, b)                for (int x=a;x<b;++x)
#define min3(a, b, c)               min(a,min(b,c))
#define max3(a, b, c)               max(a,max(b,c))
#define SQR(a)                      ((a)*(a))

int main() {    
	int a = 0xffffffff;
	printf("GoodBye World?%x\n",a );

	return 0;
}

	
