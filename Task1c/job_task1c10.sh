#!/bin/bash
#SBATCH -J MatMul10        # Job Name
#SBATCH -o Test.o%j    # Output and error file name (%j expands to jobID)
#SBATCH -n 10           # Total number of  tasks requested
#SBATCH -p normal  # Queue (partition) name -- normal, development, etc.
#SBATCH -t 05:30:00     # Run time (hh:mm:ss) - 2.5 hours
export CILK_NWORKERS=10
rm -f Task_1c10
touch Task_1c10
./matmul_par_ikj  13 >> Task_1c10
./matmul_par_kij  13 >> Task_1c10
