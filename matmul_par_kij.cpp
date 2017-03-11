#include <iostream>
#include <cilk/cilk.h>
#include <sys/time.h>
#include <math.h>
#include <cstdlib>
#include "matmul.h"
using namespace std;


int main(int argc, char *argv []) {
	int n = atoi(argv[1]);
	struct timeval start,end;
	MATRIX_SIZE = 1 << n;
	cout << "Run with size:" << MATRIX_SIZE << "\n";
	{
		init_matrices();
		gettimeofday(&start,NULL); //Start timing the computation
		for(int k = 0;k < MATRIX_SIZE;k++) {
			cilk_for(int i = 0;i < MATRIX_SIZE;i++) {
				cilk_for(int j = 0;j < MATRIX_SIZE;j++) {
					Z[i][j] = Z[i][j] + X[i][k] * Y[k][j];
				}
			}
		}
		gettimeofday(&end,NULL); //Stop timing the computation
		double myTime = (end.tv_sec+(double)end.tv_usec/1000000) -
				 (start.tv_sec+(double)start.tv_usec/1000000);
		cout << "I,J Matmul(k,i,j): " << myTime << " seconds.\n";
		destroy_matrices();
	}

	{
		init_matrices();
		gettimeofday(&start,NULL); //Start timing the computation
		for(int k = 0;k < MATRIX_SIZE;k++) {
			for(int i = 0;i < MATRIX_SIZE;i++) {
				cilk_for(int j = 0;j < MATRIX_SIZE;j++) {
					Z[i][j] = Z[i][j] + X[i][k] * Y[k][j];
				}
			}
		}
		gettimeofday(&end,NULL); //Stop timing the computation
		double myTime = (end.tv_sec+(double)end.tv_usec/1000000) -
				 (start.tv_sec+(double)start.tv_usec/1000000);
		cout << "J Matmul(k,i,j): " << myTime << " seconds.\n";
		destroy_matrices();
	}

	{
		init_matrices();
		gettimeofday(&start,NULL); //Start timing the computation
		for(int k = 0;k < MATRIX_SIZE;k++) {
			cilk_for(int i = 0;i < MATRIX_SIZE;i++) {
				for(int j = 0;j < MATRIX_SIZE;j++) {
					Z[i][j] = Z[i][j] + X[i][k] * Y[k][j];
				}
			}
		}
		gettimeofday(&end,NULL); //Stop timing the computation
		double myTime = (end.tv_sec+(double)end.tv_usec/1000000) -
				 (start.tv_sec+(double)start.tv_usec/1000000);
		cout << "I Matmul(k,i,j): " << myTime << " seconds.\n";
		destroy_matrices();
	}
}
