#include <iostream>
#include <cilk/cilk.h>
#include <sys/time.h>
using namespace std;
int **X;
int **Y;
int **Z;
int MATRIX_SIZE = (1 << 13);

void  init_matrices() {
	int i,j;
	struct timeval start,end;
	cout << "Init started\n";
	gettimeofday(&start,NULL); //Start timing the computation

	X = new int* [MATRIX_SIZE];
	Y = new int* [MATRIX_SIZE];
	Z = new int* [MATRIX_SIZE];

	for(i=0; i< MATRIX_SIZE;i++) {
		X[i] = new int[MATRIX_SIZE]; 
	}

	for(i=0; i< MATRIX_SIZE;i++) {
		Y[i] = new int[MATRIX_SIZE]; 
	}

	for(i=0; i< MATRIX_SIZE;i++) {
		Z[i] = new int[MATRIX_SIZE]; 
	}

	for (i=0; i<MATRIX_SIZE; i++) {
		for(j=0; j<MATRIX_SIZE;j++) {
			X[i][j] = i + j;
			Y[i][j] = i * j;
			Z[i][j] = 0;
		}
	}

	gettimeofday(&end,NULL); //Stop timing the computation
	double myTime = (end.tv_sec+(double)end.tv_usec/1000000) -
			 (start.tv_sec+(double)start.tv_usec/1000000);
	cout << "Init done " << myTime << " seconds.\n";
}

void destroy_matrices() {
	int i;
	for(i=0; i< MATRIX_SIZE;i++) {
		delete [] X[i]; 
		delete [] Y[i];
	}

	delete [] X;
	delete [] Y;
}	
