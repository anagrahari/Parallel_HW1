#include <iostream>
#include <boost/numeric/mtl/mtl.hpp>
#include <boost/numeric/mtl/recursion/matrix_recursator.hpp>
#include <cilk/cilk.h>
#include <time.h>
#include <sys/time.h>

using namespace mtl; using std::cout;
typedef morton_dense<double, recursion::morton_z_mask>  matrix_type;


void matMulParRec(mat::recursator<matrix_type> Z, mat::recursator<matrix_type> X,
		mat::recursator<matrix_type> Y, int BASENO) {
	matrix_type z = *Z;
	matrix_type x = *X;
	matrix_type y = *Y;

	if (num_rows(z) == BASENO && num_cols(z) == BASENO) {
		for(int k = 0; k < BASENO; k++)
			for(int i = 0; i < BASENO; i++)
				for(int j = 0; j< BASENO; j++)
					z[i][j] = z[i][j] +  (x[i][k] * y[k][j]);
	} else {
		cilk_spawn matMulParRec(north_west(Z), north_west(X), north_west(Y), BASENO);	
		cilk_spawn matMulParRec(north_east(Z), north_west(X), north_east(Y), BASENO);	
		cilk_spawn matMulParRec(south_west(Z), south_west(X), north_west(Y), BASENO);	
			   matMulParRec(south_east(Z), south_west(X), north_east(Y), BASENO);
		sync;
		cilk_spawn matMulParRec(north_west(Z), north_east(X), south_west(Y), BASENO);	
		cilk_spawn matMulParRec(north_east(Z), north_east(X), south_east(Y), BASENO);	
		cilk_spawn matMulParRec(south_west(Z), south_east(X), south_west(Y), BASENO);	
			   matMulParRec(south_east(Z), south_east(X), south_east(Y), BASENO);
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
			y[i][j] = (i + j + 3*i);
			z[i][j] = 0;
		}
	}

}

int main(int argc, char** args)
{
	struct timeval start,end;
	int MATRIX_SIZE = 1 << atoi(args[1]);
	int BASENO = 1 << atoi(args[2]);
	matrix_type  Z(MATRIX_SIZE, MATRIX_SIZE);
	matrix_type  X(MATRIX_SIZE, MATRIX_SIZE);
	matrix_type  Y(MATRIX_SIZE, MATRIX_SIZE);

	mat::recursator<matrix_type>    recz(Z);
	mat::recursator<matrix_type>    recx(X);
	mat::recursator<matrix_type>    recy(Y);

	cout << "Matrix row,columns: " << MATRIX_SIZE << "\tBase Case: " << BASENO <<"\n";

	init_matrix(recz, recx, recy);
	
	gettimeofday(&start,NULL); //Start timing the computation
	matMulParRec(recz, recx, recy, BASENO);
	gettimeofday(&end,NULL); //Stop timing the computation

	double myTime = (end.tv_sec+(double)end.tv_usec/1000000) -
			 (start.tv_sec+(double)start.tv_usec/1000000);

	cout << "Total time: " << myTime << " seconds.\n";
	/*cout << *recx<< "\n";
	cout << *recy << "\n";
	cout << *recz << "\n";
	*/
	return 0;
}
