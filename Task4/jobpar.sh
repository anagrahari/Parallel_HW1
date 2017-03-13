#!/bin/bash
#SBATCH -J ParMergeSort        # Job Name
#SBATCH -o Test.o%j    # Output and error file name (%j expands to jobID)
#SBATCH -n 6           # Total number of  tasks requested
#SBATCH -p development  # Queue (partition) name -- normal, development, etc.
#SBATCH -t 02:00:00     # Run time (hh:mm:ss) - 2.5 hours
sh batchscriptparallel.sh
