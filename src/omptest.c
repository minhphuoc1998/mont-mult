#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "hugeint.h"
#include "ompmon.h"

#define WLM 64
#define NUM_TESTM 100000

uInt nk[NUM_TESTM][WLM];
uInt ak[NUM_TESTM][WLM];
uInt bk[NUM_TESTM][WLM];
uInt rk[NUM_TESTM][WLM];
uInt n0p[NUM_TESTM];

clock_t t;
double time_taken;
int WL, NUM_TEST, NUM_THREADS;
char *DEST;

int main(int argc, char **argv)
{
	if (argc < 5)
	{
		printf("use: ./omptest <length_of_test> <number_of_test> <data_file> <number_of_threads>\n");
		return 0;
	}
	
	WL = (int) atoi(argv[1]);
	NUM_TEST = (int) atoi(argv[2]);
	DEST = argv[3];
	NUM_THREADS = (int) atoi(argv[4]);
	
	FILE *file;
	file = fopen(DEST, "r");

	for (int j = 0; j < NUM_TEST; j ++)
	{
		readi(file, (hugeInt) &nk[j], WL);
		readi(file, (hugeInt) &ak[j], WL);
		readi(file, (hugeInt) &bk[j], WL);
		
		uInt n0 = nk[j][0];
		n0p[j] = nZPrime(n0, 32);
	}

	printf("Number of Test: %d\n", NUM_TEST);
	printf("Length: %d bits\n", WL * (1 << 7));
	printf("Number of threads: %d\n", NUM_THREADS);

	// Montgomery Naive Parallel FIOS
	t = clock();
	for (int j = 0; j < NUM_TEST; j ++)
	{
		monProPFIOS(rk[j], ak[j], bk[j], nk[j], n0p[j], WL, NUM_THREADS);
	}
	t = clock() - t;
	time_taken = ((double)t)/CLOCKS_PER_SEC;
	printf("Time for Naive pFIOS: %f\n", time_taken);
	
	// Montgomery New Parallel FIOS
	t = clock();
	for (int j = 0; j < NUM_TEST; j ++)
	{
		monProPFIOSn(rk[j], ak[j], bk[j], nk[j], n0p[j], WL, NUM_THREADS);
	}
	t = clock() - t;
	time_taken = ((double)t)/CLOCKS_PER_SEC;
	printf("Time for New pFIOS: %f\n\n", time_taken);

	return 0;
}