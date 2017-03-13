#include <iostream>
#include <boost/numeric/mtl/mtl.hpp>
#include <boost/numeric/mtl/recursion/matrix_recursator.hpp>
#include <cilk/cilk.h>
#include <time.h>
#include <sys/time.h>

using namespace mtl; using std::cout;
typedef morton_dense<double, recursion::morton_z_mask>  matrix_type;

void matMulParRec(mat::recursator<matrix_type> Z, mat::recursator<matrix_type> X,
		mat::recursator<matrix_type> Y) {
	matrix_type z = *Z;
	matrix_type x = *X;
	matrix_type y = *Y;

	if (num_rows(z) == 1 && num_cols(z) == 1) {
		z[0][0] = z[0][0] +  (x[0][0] * y[0][0]);
	} else {
		cilk_spawn matMulParRec(north_west(Z), north_west(X), north_west(Y));	
		cilk_spawn matMulParRec(north_east(Z), north_west(X), north_east(Y));	
		cilk_spawn matMulParRec(south_west(Z), south_west(X), north_west(Y));	
			   matMulParRec(south_west(Z), south_west(X), north_east(Y));
		sync;
		cilk_spawn matMulParRec(north_west(Z), north_east(X), south_west(Y));	
		cilk_spawn matMulParRec(north_east(Z), north_east(X), south_east(Y));	
		cilk_spawn matMulParRec(south_west(Z), south_east(X), south_west(Y));	
			   matMulParRec(south_east(Z), south_east(X), south_east(Y));
		sync;
	}
}

void init_matrix(mat::recursator<matrix_type> Z, mat::recursator<matrix_type> X,
		 mat::recursator<matrix_type> Y) {
	matrix_type z = *Z;
	matrix_type x = *X;
	matrix_type y = *Y;

	for (int i = 0; i < num_rows(z); i++) {
		for (int j = 0; j < num_rows(z); j++) {
			x[i][j] = (i + j + 2*i*j);
			y[i][j] = (i + j);
			z[i][j] = 0;
		}
	}

}

int main(int argc, char** args)
{
	int n = atoi(args[1]);
	int MATRIX_SIZE = 1 << n;
	matrix_type  Z(MATRIX_SIZE, MATRIX_SIZE);
	matrix_type  X(MATRIX_SIZE, MATRIX_SIZE);
	matrix_type  Y(MATRIX_SIZE, MATRIX_SIZE);
	mat::recursator<matrix_type>    recz(Z);
	mat::recursator<matrix_type>    recx(X);
	mat::recursator<matrix_type>    recy(Y);

	init_matrix(recz, recx, recy);	
	struct timeval start,end;
	gettimeofday(&start,NULL); //Start timing the computation
	matMulParRec(recz, recx, recy);
	gettimeofday(&end,NULL); //Stop timing the computation
	double myTime = (end.tv_sec+(double)end.tv_usec/1000000) -
			 (start.tv_sec+(double)start.tv_usec/1000000);
	cout << "Total time: " << myTime << " seconds.\n";
	
	cout << *recx << "\n";
	cout << *recy << "\n";
	cout << *recz << "\n";
	return 0;
}
