#include <iostream>
#include <boost/numeric/mtl/mtl.hpp>
#include <boost/numeric/mtl/recursion/matrix_recursator.hpp>
#include <cilk/cilk.h>
#include <time.h>
#include <sys/time.h>
/*
#define north_west _11 
#define north_east _12
#define south_west _21
#define south_east _22
*/
using namespace mtl; using std::cout;
typedef morton_dense<double, recursion::morton_z_mask>  matrix_type;

void serial_parentheses(mat::recursator<matrix_type> A) {
        int i, j, k;
        matrix_type a = *A;
        int n = num_rows(*A);
        for (i = 0; i < n; i++)
                for(j=i+2; j < n; j++)
                        for (k=i; k < j; k++)
                                a[i][j] = a[i][j] > a[i][k] + a[k][j] ?
                                                a[i][k] + a[k][j] : a[i][j];
}

void serial_parenthesesA(mat::recursator<matrix_type> A) {
        int i, j, k, t;
        matrix_type a = *A;
        int n = num_rows(*A);
        for (t = 2; t < n; t++){
                for(i =0 ; i < n-t; i++){
			j = t + i;
                        for (k=i+1; k <= j; k++){
                                a[i][j] = a[i][j] > a[i][k] + a[k][j] ?
                                                a[i][k] + a[k][j] : a[i][j];
			}
		}
	}
}

void serial_parenthesesC(mat::recursator<matrix_type> A, mat::recursator<matrix_type> U,
                mat::recursator<matrix_type> V) {
        int i, j, k;
        matrix_type a = *A;
	matrix_type u = *U;
        matrix_type v = *V;
        int n = num_rows(*A);
        for (i = 0; i < n; i++)
                for(j =0 ; j < n; j++)
                        for (k=0; k < n; k++)
                                a[i][j] = a[i][j] > u[i][k] + v[k][j] ?
                                                u[i][k] + v[k][j] : a[i][j];
}

void serial_parenthesesB(mat::recursator<matrix_type> A, mat::recursator<matrix_type> U,
                mat::recursator<matrix_type> V) {
        int i, j, k, t;
        matrix_type a = *A;
        matrix_type u = *U;
        matrix_type v = *V;
        int n = num_rows(*A);
        for (t = n-1; t >= 0; t--){
                for(i =t ; i < n; i++){
			j = i - t;
                        for (k=i; k < n; k++){
                                a[i][j] = a[i][j] > u[i][k] + v[k][j] ?
                                                u[i][k] + v[k][j] : a[i][j];
			}
			for (k =0; k <= j;k++) {
				a[i][j] = a[i][j] > u[i][k] + v[k][j] ?
                                                u[i][k] + v[k][j] : a[i][j];
			}
		}
	}
}
void cPar(mat::recursator<matrix_type> X, mat::recursator<matrix_type> U,
		mat::recursator<matrix_type> V, int BASENO) {
	matrix_type x = *X;
	matrix_type u = *U;
	matrix_type v = *V;

	if (num_rows(X) == BASENO && num_cols(X) == BASENO) {
		serial_parenthesesC(X, U, V);
	//	x[0][0] = x[0][0]  < (u[0][0] + v[0][0]) ? x[0][0] : (u[0][0] + v[0][0]);
	} else {
		cilk_spawn cPar(north_west(X), north_west(U), north_west(V), BASENO);	
		cilk_spawn cPar(north_east(X), north_west(U), north_east(V), BASENO);	
		cilk_spawn cPar(south_west(X), south_west(U), north_west(V), BASENO);	
			   cPar(south_east(X), south_west(U), north_east(V), BASENO);
		
		sync;
		
		cilk_spawn cPar(north_west(X), north_east(U), south_west(V), BASENO);	
		cilk_spawn cPar(north_east(X), north_east(U), south_east(V), BASENO);	
		cilk_spawn cPar(south_west(X), south_east(U), south_west(V), BASENO);	
			   cPar(south_east(X), south_east(U), south_east(V), BASENO);
		
		sync;
	}
}

void bPar(mat::recursator<matrix_type> X, mat::recursator<matrix_type> U,
		mat::recursator<matrix_type> V, int BASENO) {
	matrix_type x = *X;
	matrix_type u = *U;
	matrix_type v = *V;

	if (num_rows(X) == BASENO && num_cols(X) == BASENO) {
		serial_parenthesesB(X, U, V);
	//	x[0][0] = x[0][0]  < (u[0][0] + v[0][0]) ? x[0][0] : (u[0][0] + v[0][0]);
	} else {
		bPar(south_west(X), south_east(U), north_west(V), BASENO);

		cilk_spawn cPar(north_west(X), north_east(U), south_west(X), BASENO);
			   cPar(south_east(X), south_west(X), north_east(V), BASENO);
		
		sync;
		
		cilk_spawn bPar(north_west(X), north_west(U), north_west(V), BASENO);
			   bPar(south_east(X), south_east(U), south_east(V), BASENO);
	
		sync;
	
		cPar(north_east(X), north_east(U), south_east(X), BASENO);
		
		cPar(north_east(X), north_west(X), north_east(V), BASENO);
		
		bPar(north_east(X), north_west(U), south_east(V), BASENO);
	}
}

void aPar(mat::recursator<matrix_type> X, int BASENO) {
	if (num_rows(*X) == BASENO && num_cols(*X) == BASENO) {
		serial_parenthesesA(X);
		return;
	} else {
		cilk_spawn aPar(north_west(X), BASENO);
			   aPar(south_east(X), BASENO);
		sync;
		bPar(north_east(X), north_west(X), south_east(X), BASENO);
	}
}

void init_matrix(mat::recursator<matrix_type> A) {
	matrix_type a = *A;
	for (int i = 0; i < num_rows(a); i++) {
		for (int j = 0; j < num_rows(a); j++) {
			if (i == j - 1) 
				a[i][j] = (i + j + 2*i*j);
			else
				a[i][j] = LONG_MAX;
		}
	}

}

int main(int argc, char** args)
{
	int n = atoi(args[1]);
	int MATRIX_SIZE = 1 << n;
	int BASENO = 1 << atoi(args[2]);
	matrix_type  A(MATRIX_SIZE, MATRIX_SIZE);
	mat::recursator<matrix_type>    rec(A);

	init_matrix(rec);	
	struct timeval start,end;
	gettimeofday(&start,NULL); //Start timing the computation
	aPar(rec, BASENO);
	gettimeofday(&end,NULL); //Stop timing the computation
	cout << *rec << "\n";
	double myTime = (end.tv_sec+(double)end.tv_usec/1000000) -
			 (start.tv_sec+(double)start.tv_usec/1000000);
	cout << "Total time: " << myTime << " seconds.\n";
	init_matrix(rec);
	serial_parentheses(rec);
	cout << "serial \n"<<  *rec << "\n";
	
	return 0;
}
