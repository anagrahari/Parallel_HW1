#!/bin/bash
#SBATCH -J MatMul        # Job Name
#SBATCH -o Test.o%j    # Output and error file name (%j expands to jobID)
#SBATCH -n 1           # Total number of  tasks requested
#SBATCH -p normal  # Queue (partition) name -- normal, development, etc.
#SBATCH -t 00:40:00     # Run time (hh:mm:ss) - 2.5 hours
./matmul_serial_ijk > Result
