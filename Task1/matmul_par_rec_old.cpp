#include <iostream>
#include <cilk/cilk.h>
#include <sys/time.h>
#include <math.h>
#include <cstdlib>
#include "matmul.h"
using namespace std;

void matMultiplyRec(int Zx0, int Zxn, int Zy0,int Zyn, int Xx0, int Xxn, int Xy0, int Xyn, int Yx0, int Yxn, int Yy0,int  Yyn, int n, int baseno) {
	if(n == baseno) {
		for(int k =0;k < n; k++ ) {
			for(int i = Zx0; i <= Zxn; i++) {
				for(int j = Zy0; j<= Zyn; j++) {
					Z[i][j] = Z[i][j] + X[i][Xy0 + k] * Y[Yx0 + k][j];
				}
			}
		}
	}
	else {
		cilk_spawn matMultiplyRec(Zx0,Zx0 +(n/2 -1),Zy0,Zy0 +(n/2 -1),Xx0,Xx0 +(n/2 -1),Xy0,Xy0 +( n/2 -1),Yx0,Yx0 +( n/2 -1),Yy0,Yy0 +( n/2 -1),n/2 , baseno);
		cilk_spawn matMultiplyRec(Zx0,Zx0 +(n/2 -1),Zy0 + n/2,Zy0 + (n-1),Xx0,Xx0 +(n/2 -1),Xy0,Xy0 +( n/2 -1),Yx0,Yx0 +( n/2 -1),Yy0 + n/2,Yy0 + (n-1),n/2, baseno);
		cilk_spawn matMultiplyRec(Zx0 + n/2,Zx0 + (n-1),Zy0,Zy0 +( n/2 -1),Xx0 + n/2,Xx0 + (n-1),Xy0,Xy0 +( n/2 -1),Yx0,Yx0 +( n/2 -1),Yy0,Yy0 +( n/2 -1),n/2, baseno);
		matMultiplyRec(Zx0 + n/2,Zx0 + (n-1),Zy0 + n/2,Zy0 + (n-1),Xx0 + n/2,Xx0 + (n-1),Xy0,Xy0 + (n-1),Yx0,Yx0 +( n/2 -1),Yy0 + n/2,Yy0 + (n-1),n/2, baseno);
		cilk_sync;
		cilk_spawn matMultiplyRec(Zx0,Zx0 +(n/2 -1),Zy0,Zy0 +( n/2 -1),Xx0,Xx0 +( n/2 -1),Xy0 + n/2,Xy0 + (n-1),Yx0 + n/2,Yx0 + (n-1),Yy0,Yy0 +( n/2 -1),n/2, baseno);
		cilk_spawn matMultiplyRec(Zx0,Zx0 +(n/2 -1),Zy0 + n/2,Zy0 + (n-1),Xx0,Xx0 +( n/2 -1),Xy0 + n/2,Xy0 + (n-1),Yx0 + n/2,Yx0 + (n-1),Yy0 + n/2,Yy0 + (n-1),n/2, baseno);
		cilk_spawn matMultiplyRec(Zx0 + n/2,Zx0 + (n-1),Zy0,Zy0 +( n/2 -1),Xx0 + n/2,Xx0 + (n-1),Xy0 + n/2,Xy0 + (n-1),Yx0 + n/2,Yx0 + (n-1),Yy0,Yy0 +( n/2 -1),n/2, baseno);
		matMultiplyRec(Zx0 + n/2,Zx0 + (n-1),Zy0 + n/2,Zy0 + (n-1),Xx0 + n/2,Xx0 + (n-1),Xy0 + n/2,Xy0 + (n-1),Yx0 + n/2,Yx0 + (n-1),Yy0 + n/2,Yy0 + (n-1),n/2, baseno);        
		cilk_sync;

	}
}

int main(int argc, char *argv []) {
	int baseno = 1 << atoi(argv[1]);
	
	struct timeval start,end;
	MATRIX_SIZE = 1 << 10;
	cout << "Run with matrix size:" << MATRIX_SIZE << "\n";
	{
		init_matrices();
		cout << "BaseNo: " << baseno << "\n";
		gettimeofday(&start,NULL); //Start timing the computation
		matMultiplyRec(0,MATRIX_SIZE-1,0,MATRIX_SIZE-1,0,MATRIX_SIZE-1,0,MATRIX_SIZE-1,0,MATRIX_SIZE-1,0,MATRIX_SIZE-1, MATRIX_SIZE, baseno);
		gettimeofday(&end,NULL); //Stop timing the computation
		double myTime = (end.tv_sec+(double)end.tv_usec/1000000) -
			(start.tv_sec+(double)start.tv_usec/1000000);
		cout << "Matmul recursive: " << myTime << " seconds.\n";
		destroy_matrices();
	}
}
