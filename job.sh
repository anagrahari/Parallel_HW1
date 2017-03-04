#!/bin/bash
#SBATCH -J MatMul        # Job Name
#SBATCH -o Test.o%j    # Output and error file name (%j expands to jobID)
#SBATCH -n 6           # Total number of  tasks requested
#SBATCH -p normal  # Queue (partition) name -- normal, development, etc.
#SBATCH -t 03:30:00     # Run time (hh:mm:ss) - 2.5 hours
rm -f Result 
touch Result
./matmul_serial_ijk >> Result
./matmul_serial_ikj >> Result
./matmul_serial_jki >> Result
./matmul_serial_jik >> Result
./matmul_serial_kij >> Result
./matmul_serial_kji >> Result
