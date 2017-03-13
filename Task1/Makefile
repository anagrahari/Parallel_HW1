CC=icpc
CFLAGS=-g

all:
	$(CC) $(CFLAGS) -o matmul_serial_ijk matmul_serial_ijk.cpp
	$(CC) $(CFLAGS) -o matmul_serial_ikj matmul_serial_ikj.cpp
	$(CC) $(CFLAGS) -o matmul_serial_jik matmul_serial_jik.cpp
	$(CC) $(CFLAGS) -o matmul_serial_jki matmul_serial_jki.cpp
	$(CC) $(CFLAGS) -o matmul_serial_kij matmul_serial_kij.cpp
	$(CC) $(CFLAGS) -o matmul_serial_kji matmul_serial_kji.cpp
	$(CC) $(CFLAGS) -o matmul_par_ikj matmul_par_ikj.cpp
	$(CC) $(CFLAGS) -o matmul_par_kij matmul_par_kij.cpp
	$(CC) $(CFLAGS) -o matmul_par_rec matmul_par_rec.cpp
	$(CC) $(CFLAGS) -I/home1/04757/tg840570/include/  matmul_par_rec_new.cpp -o matmul_par_rec_new 
clean:
	rm -rf  matmul_serial_ijk
	rm -rf  matmul_serial_ikj
	rm -rf  matmul_serial_jik
	rm -rf  matmul_serial_jki
	rm -rf  matmul_serial_kji
	rm -rf  matmul_serial_kij
	rm -rf	matmul_par_ikj
	rm -rf	matmul_par_kij
	rm -rf	matmul_par_rec
	rm -rf	matmul_par_rec_new
