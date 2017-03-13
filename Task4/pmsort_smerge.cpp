#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "parallel_merge.h"

template <typename T>
void swap(T *a, T *b) {
	T temp = *a;
	*a = *b;
	*b = temp;
}

void printarr(int *arr, int len) {
	int i;
	for(i=0; i < len; i++) {
		printf("%d\t", arr[i]);
	}
	printf("\n");
}

void insertionSort(int *A, int len) {
	for (int i = 0; i < len; i++) {
		int j = i;
		while (j > 0 && A[j-1] > A[j]) {
			swap<int>(&A[j], &A[j-1]);
			j--;
		}
	}
}

void merge(int *A, int p, int q, int r) {
	int n = r-p+1;
	int *TEMP = new int[n];

	for(int ind = 0; ind < n; ind++) {
		TEMP[ind] = A[p + ind];
	}

	int i = 0;
	int j = ((q - p)) + 1;
	int k = p;
	while (i <= (q - p) && j < n) { 
		if(TEMP[i] <= TEMP[j]) {
			A[k++] = TEMP[i++];
		} else {
			A[k++] = TEMP[j++];
		}
	}

	while( j < n) {
		A[k++] = TEMP[j++];
	}

	while( i <= (q - p)) {
		A[k++] = TEMP[i++];
	}
	delete [] TEMP;
}

void mergeSort(int * A, int p, int r)
{
	if ( r - p + 1 <= 16) {
		insertionSort(A + p,r - p + 1 );
	} else {
		if(p < r) {
			int q = (p + r) / 2;
			cilk_spawn mergeSort(A, p, q);
				   mergeSort(A, q + 1,r);
			sync;
			merge(A, p, q, r);	   
		}
	}
}

int main(int argc, char *args[]) {
	int n = atoi(args[1]);
	struct timeval start,end;
	ARR_SIZE = 1 << n;
	BASE_LEN = 1 << atoi(args[2]);
	cout << "Run with arr_size: " << ARR_SIZE << "\n";
	cout << "Base Length: " << BASE_LEN << "\n" ;

	init_arr();
	gettimeofday(&start,NULL); //Start timing the computation
	mergeSort(A, 0, ARR_SIZE - 1);		
	gettimeofday(&end,NULL); //Stop timing the computation
	double myTime = (end.tv_sec+(double)end.tv_usec/1000000) -
			 (start.tv_sec+(double)start.tv_usec/1000000);
	cout << "Parmsort: " << myTime << " seconds.\n";
	printarr(A,ARR_SIZE);
	destroy_arr();
}
