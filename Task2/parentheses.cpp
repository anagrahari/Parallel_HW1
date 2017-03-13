#include <iostream>
#include <boost/numeric/mtl/mtl.hpp>
#include <boost/numeric/mtl/recursion/matrix_recursator.hpp>
#include <cilk/cilk.h>
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
int main(int, char**)
{
	matrix_type  A(8, 8);
	mat::recursator<matrix_type>    rec(A);
	init_matrix(rec);	
	cout << "A is \n" << *(rec) << "\n";
	aPar(rec);
	cout << "A is \n" << *(rec) << "\n";
	/*cout << "upper right quadrant (north_east) of A is \n" 
		<< *north_east(rec) << "\n";
	cout << "upper left  quadrant (north_west) of A is \n" 
		<< *north_west(rec) << "\n";
	cout << "lower right quadrant (south_east) of A is \n" 
		<< *south_east(rec) << "\n";
	cout << "lower quadrant (south_west) of A is \n" 
		<< *south_west(rec) << "\n";
	*/
	return 0;
}
