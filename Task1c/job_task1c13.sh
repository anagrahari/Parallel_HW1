#!/bin/bash
#SBATCH -J MatMul13        # Job Name
#SBATCH -o Test.o%j    # Output and error file name (%j expands to jobID)
#SBATCH -n 13           # Total number of  tasks requested
#SBATCH -p development  # Queue (partition) name -- normal, development, etc.
#SBATCH -t 05:30:00     # Run time (hh:mm:ss) - 2.5 hours
rm -f Task_1c13 
touch Task_1c13
./matmul_par_ikj  13 >> Task_1c13
