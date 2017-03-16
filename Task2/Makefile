CC=icpc
CFLAGS=-O0

all:
	$(CC) $(CFLAGS) -I$(HOME)/include/  parentheses.cpp -o parentheses 
	$(CC) $(CFLAGS) -I$(HOME)/include/  parentheses1.cpp -o parentheses1 
	$(CC) $(CFLAGS)  parentheses_nolib.cpp -o parentheses_nolib 

clean:
	rm -rf parentheses1
	rm -rf parentheses
	rm -rf parentheses_nolib
