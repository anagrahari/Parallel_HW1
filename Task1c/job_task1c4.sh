#!/bin/bash
#SBATCH -J MatMul4        # Job Name
#SBATCH -o Test.o%j    # Output and error file name (%j expands to jobID)
#SBATCH -n 4           # Total number of  tasks requested
#SBATCH -p normal  # Queue (partition) name -- normal, development, etc.
#SBATCH -t 05:30:00     # Run time (hh:mm:ss) - 5.5 hours
export CILK_NWORKERS=4
rm -f Task_1c4 
touch Task_1c4
./matmul_par_ikj  13 >> Task_1c4
./matmul_par_kij  13 >> Task_1c4
