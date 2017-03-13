CC=icpc
CFLAGS=-g

all:
	$(CC) $(CFLAGS) -o pmsort_pmerge pmsort_pmerge.cpp 
	$(CC) $(CFLAGS) -o pmsort_smerge pmsort_smerge.cpp
	$(CC) $(CFLAGS) -o serial_msort_merge serial_msort_merge.cpp 
clean:
	rm -rf pmsort_pmerge
	rm -rf pmsort_smerge
	rm -rf serial_msort_merge
