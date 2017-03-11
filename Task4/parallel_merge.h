#include <iostream>
#include <stdlib.h>
#include <cilk/cilk.h>
#include <sys/time.h>
using namespace std;
int *T;
int *A;
int ARR_SIZE = (1<<12);
int BASE_LEN = (1<<4);


void init_arr() {
	struct timeval start,end;
	int i;
	srand(time(NULL));
	T = new int [ARR_SIZE];
	A = new int [ARR_SIZE];

	cout << "Init started\n";
	gettimeofday(&start,NULL); //Start timing the computation
	for (i = 0; i < ARR_SIZE; i++)
		A[i] = rand() % 1000;
		
	gettimeofday(&end,NULL); //Stop timing the computation
	double myTime = (end.tv_sec+(double)end.tv_usec/1000000) -
			 (start.tv_sec+(double)start.tv_usec/1000000);
	cout << "Init done " << myTime << " seconds.\n";
}

void destroy_arr() {
	delete [] T;
	delete [] A;
}
