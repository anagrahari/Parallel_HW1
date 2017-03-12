#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <stdbool.h>
#include "parallel_merge.h"

void swap(int *a, int *b) {
	int temp = *a;
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

int binary_search( int value, const int* a, int left, int right )
{
	int low  = left;
	int high =  left > right + 1 ? left : right + 1;
	while( low < high )
	{
		int mid = ( low + high ) / 2;
		if ( value <= a[ mid ] ) high = mid;
		else     low  = mid + 1;
	}
	return high;
}

void pmerge( int* t, int p1, int r1, int p2, int r2, int* a, int p3 )
{
	int length1 = r1 - p1 + 1;
	int length2 = r2 - p2 + 1;
	if ( length1 < length2 ) {
		swap(&p1,  &p2 );
		swap(&r1,  &r2 );
		swap( &length1, &length2 );
	}
	if ( length1 == 0 ) return;
	int q1 = ( p1 + r1 ) / 2;
	int q2 = binary_search( t[ q1 ], t, p2, r2 );
	int q3 = p3 + ( q1 - p1 ) + ( q2 - p2 );
	a[ q3 ] = t[ q1 ];

	 pmerge( t, p1,     q1 - 1, p2, q2 - 1, a, p3     );
	 pmerge( t, q1 + 1, r1,     q2, r2,     a, q3 + 1 );
}

void insertSort(int *A, int len) {
	for (int i = 0; i < len; i++) {
		int j = i;
		while (j > 0 && A[j-1] > A[j]) {
			swap(&A[j], &A[j-1]);
			j = j - 1;
		}
	}
}

void pmergesort(int* from, int l, int r, int* dst, bool fromToDst)
{
	if (r == l) {
		if (fromToDst)  dst[l] = from[l];
		return;
	}

	if ((r - l) <= BASE_LEN  && !fromToDst ) {
		insertSort( from + l, r - l + 1 );
		return;
	}

	int m = (( r + l ) / 2);
	 pmergesort(from, l,     m, dst, !fromToDst);      
		   pmergesort(from, m + 1, r, dst, !fromToDst);      

	if (fromToDst)
		pmerge( from, l, m, m + 1, r, dst, l);
	else    
		pmerge(dst, l, m, m + 1, r, from, l);
}


int main (int c, char *args[]) {
	int n = atoi(args[1]);
	struct timeval start,end;
	ARR_SIZE = 1 << n;
	BASE_LEN = 1 << atoi(args[2]);

	cout << "Run with arr_size: " << ARR_SIZE << "\n";
	cout << "Base Length: " << BASE_LEN << "\n" ;

	init_arr();
	gettimeofday(&start,NULL); //Start timing the computation
	pmergesort(A, 0, ARR_SIZE-1, T, true);
	gettimeofday(&end,NULL); //Stop timing the computation
	double myTime = (end.tv_sec+(double)end.tv_usec/1000000) -
			 (start.tv_sec+(double)start.tv_usec/1000000);
	cout << "serialmsort: " << myTime << " seconds.\n";
	//printarr(A,ARR_SIZE);
	//printarr(T,ARR_SIZE);
	destroy_arr();
}
