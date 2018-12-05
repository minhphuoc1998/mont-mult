#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "hugeint.h"
#include "seqmon.h"

#define NUM_THREADS 2
#define WLM 64
#define NUM_TESTM 100000

uInt nk[NUM_TESTM][WLM];
uInt ak[NUM_TESTM][WLM];
uInt bk[NUM_TESTM][WLM];
uInt rk[NUM_TESTM][WLM];
uInt n0p[NUM_TESTM];

clock_t t;
double time_taken;
int WL, NUM_TEST;
char *DEST;

int main(int argc, char **argv)
{
	if (argc < 4)
	{
		printf("use: ./seqtest <length_of_test> <number_of_test> <data_file>\n");
		return 0;
	}
	
	WL = (int) atoi(argv[1]);
	NUM_TEST = (int) atoi(argv[2]);
	DEST = argv[3];

	FILE *file;
	file = fopen(DEST, "r");
	int j;
	for (j = 0; j < NUM_TEST; j ++)
	{
		readi(file, (hugeInt) &nk[j], WL);
		readi(file, (hugeInt) &ak[j], WL);
		readi(file, (hugeInt) &bk[j], WL);
		
		uInt n0 = nk[j][0];
		n0p[j] = nZPrime(n0, 32);
	}
	
	printf("Number of Test: %d\n", NUM_TEST);
	printf("Length: %d bits\n", WL * (1 << 7));
	
	// Montgomery SOS
	t = clock();
	for (j = 0; j < NUM_TEST; j ++)
	{
		monProSOS(rk[j], ak[j], bk[j], nk[j], n0p[j], WL);
	}
	t = clock() - t;
	time_taken = ((double)t)/CLOCKS_PER_SEC;
	printf("Time for SOS: %f\n", time_taken);
	
	// Montgomery CIOS
	t = clock();
	for (j = 0; j < NUM_TEST; j ++)
	{
		monProCIOS(rk[j], ak[j], bk[j], nk[j], n0p[j], WL);
	}
	t = clock() - t;
	time_taken = ((double)t)/CLOCKS_PER_SEC;
	printf("Time for CIOS: %f\n", time_taken);
	
	// Montgomery FIOS
	t = clock();
	for (j = 0; j < NUM_TEST; j ++)
	{
		monProFIOS(rk[j], ak[j], bk[j], nk[j], n0p[j], WL);
	}
	t = clock() - t;
	time_taken = ((double)t)/CLOCKS_PER_SEC;
	printf("Time for FIOS: %f\n", time_taken);
	
	// Montgomery FIOS Modified
	t = clock();
	for (j = 0; j < NUM_TEST; j ++)
	{
		monProFIOSm(rk[j], ak[j], bk[j], nk[j], n0p[j], WL);
	}
	t = clock() - t;
	time_taken = ((double)t)/CLOCKS_PER_SEC;
	printf("Time for FIOS (Modified): %f\n\n", time_taken);
	return 0;
}