#!/bin/bash
#SBATCH -J MatPar        # Job Name
#SBATCH -o Test1E.o%j    # Output and error file name (%j expands to jobID)
#SBATCH -n 12           # Total number of  tasks requested
#SBATCH -p normal  # Queue (partition) name -- normal, development, etc.
#SBATCH -t 06:00:00     # Run time (hh:mm:ss) - 2.5 hours
rm -f Result_1E 
touch Result_1E
./matmul_par_rec 13 1 >> Result_1E
./matmul_par_rec 13 2 >> Result_1E
./matmul_par_rec 13 3 >> Result_1E
./matmul_par_rec 13 4 >> Result_1E
./matmul_par_rec 13 6 >> Result_1E
./matmul_par_rec 13 7 >> Result_1E
./matmul_par_rec 13 8 >> Result_1E
./matmul_par_rec 13 9 >> Result_1E
./matmul_par_rec 13 10 >> Result_1E
./matmul_par_rec 13 11 >> Result_1E
./matmul_par_rec 13 12 >> Result_1E
