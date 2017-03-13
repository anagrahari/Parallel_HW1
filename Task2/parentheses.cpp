#include <iostream>
#include <boost/numeric/mtl/mtl.hpp>
#include <boost/numeric/mtl/recursion/matrix_recursator.hpp>
#include <cilk/cilk.h>
#include <sys/time.h>
#include <limits.h>
/*
#define north_west _11 
#define north_east _12
#define south_west _21
#define south_east _22
*/
using namespace mtl; using std::cout;
typedef morton_dense<double, recursion::morton_z_mask>  matrix_type;

void cPar(mat::recursator<matrix_type> X, mat::recursator<matrix_type> U,
		mat::recursator<matrix_type> V) {
	matrix_type x = *X;
	matrix_type u = *U;
	matrix_type v = *V;

	if (num_rows(X) == 1 && num_cols(X) == 1) {
		x[0][0] = x[0][0]  < (u[0][0] + v[0][0]) ? x[0][0] : (u[0][0] + v[0][0]);
	} else {
		cilk_spawn cPar(north_west(X), north_west(U), north_west(V));	
		cilk_spawn cPar(north_east(X), north_west(U), north_east(V));	
		cilk_spawn cPar(south_west(X), south_west(U), north_west(V));	
		cPar(south_east(X), south_west(U), north_east(V));
		sync;
		cilk_spawn cPar(north_west(X), north_east(U), south_west(V));	
		cilk_spawn cPar(north_east(X), north_east(U), south_east(V));	
		cilk_spawn cPar(south_west(X), south_east(U), south_west(V));	
		cPar(south_east(X), south_east(U), south_east(V));
		sync;
	}
}

void bPar(mat::recursator<matrix_type> X, mat::recursator<matrix_type> U,
		mat::recursator<matrix_type> V) {
	matrix_type x = *X;
	matrix_type u = *U;
	matrix_type v = *V;

	if (num_rows(X) == 1 && num_cols(X) == 1) {
		x[0][0] = x[0][0]  < (u[0][0] + v[0][0]) ? x[0][0] : (u[0][0] + v[0][0]);
	} else {
		bPar(south_west(X), south_east(U), north_east(V));
		cilk_spawn cPar(north_west(X), north_east(U), south_west(X));
		cPar(south_east(X), south_west(X), north_east(V));
		sync;
		cilk_spawn bPar(north_west(X), north_west(U), north_west(V));
		bPar(south_east(X), south_east(U), south_east(V));
		sync;
		cPar(north_east(X), north_east(U), south_east(X));
		cPar(north_east(X), north_west(X), north_east(V));
		bPar(north_east(X), north_west(U), south_east(V));
	}
}

void aPar(mat::recursator<matrix_type> X) {
	if (num_rows(*X) == 1 && num_cols(*X) == 1) {
		return;
	} else {
		cilk_spawn aPar(north_west(X));
		aPar(south_east(X));
		sync;
		bPar(north_east(X), north_west(X), south_east(X));
	}
}

void init_matrix(mat::recursator<matrix_type> A) {
	matrix_type a = *A;
	for (int i = 0; i < num_rows(a); i++) {
		for (int j = 0; j < num_rows(a); j++) {
			if (i == j - 1) 
				a[i][j] = i + j + 2*i*j;
			else
				a[i][j] = LONG_MAX;
		}
	}

}
/*
void init_mat(mat::recursator<matrix_type> A) {
	matrix_type Z = *A;
	int COUNT = num_rows(Z);
	int i, j;
	for(i=0;i<COUNT;i++)
		for(j=0;j<COUNT;j++)
			Z[i][j] = INT_MAX/4;

	for(i=0;i<3;i++)
		Z[i][i+1]=i+2;

	for(i=4;i<7;i++)
		Z[i][i+1]=i+1;


	Z[3][4]=4;
}
*/

void serial_parentheses(mat::recursator<matrix_type> A) {
	int i, j, k;
	matrix_type a = *A;
	for (i=0; i < num_rows(*A); i++) 
		for(j=i+2; j < num_rows(*A); j++)
			for (k=i; k < j; k++)
				a[i][j] = a[i][j] > a[i][k] + a[k][j] ? 
						a[i][k] + a[k][j] : a[i][j];	
}
	
int main(int argc, char** args)
{
	int n = atoi(args[1]);
	int MATRIX_SIZE = 1 << n;
	matrix_type  A(MATRIX_SIZE, MATRIX_SIZE);
	mat::recursator<matrix_type>    rec(A);

	init_matrix(rec);	
	struct timeval start,end;
	gettimeofday(&start,NULL); //Start timing the computation
	aPar(rec);
	gettimeofday(&end,NULL); //Stop timing the computation
	double myTime = (end.tv_sec+(double)end.tv_usec/1000000) -
			 (start.tv_sec+(double)start.tv_usec/1000000);
	cout << "Total time: " << myTime << " seconds.\n";
	return 0;
}
