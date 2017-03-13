#include <iostream>
#include <cilk/cilk.h>
#include <sys/time.h>
#include <math.h>
#include "matmul.h"
using namespace std;


int main() {
	int i,j,k;
	struct timeval start,end;

	init_matrices();

	gettimeofday(&start,NULL); //Start timing the computation
	for(i = 0;i < MATRIX_SIZE;i++) {
		for(k = 0;k < MATRIX_SIZE;k++) {
			for(j = 0;j < MATRIX_SIZE;j++) {
				Z[i][j] = Z[i][j] + X[i][k] * Y[k][j];
			}
		}
	}
	gettimeofday(&end,NULL); //Stop timing the computation
	double myTime = (end.tv_sec+(double)end.tv_usec/1000000) -
			 (start.tv_sec+(double)start.tv_usec/1000000);
	cout << "Matmul(i,k,j): " << myTime << " seconds.\n";

	destroy_matrices();
}
