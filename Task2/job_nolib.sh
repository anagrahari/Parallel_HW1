#!/bin/bash
#SBATCH -J ParNolib        # Job Name
#SBATCH -o Test.o%j    # Output and error file name (%j expands to jobID)
#SBATCH -n 8           # Total number of  tasks requested
#SBATCH -p normal  # Queue (partition) name -- normal, development, etc.
#SBATCH -t 08:00:00     # Run time (hh:mm:ss) - 2.5 hours
rm -f Result_nolib 
touch Result_nolib
./parentheses_nolib 1 >> Result_nolib
./parentheses_nolib 2 >> Result_nolib
./parentheses_nolib 3 >> Result_nolib
./parentheses_nolib 4 >> Result_nolib
./parentheses_nolib 5 >> Result_nolib
./parentheses_nolib 6 >> Result_nolib
./parentheses_nolib 7 >> Result_nolib
./parentheses_nolib 8 >> Result_nolib
./parentheses_nolib 9 >> Result_nolib
./parentheses_nolib 10 >> Result_nolib
./parentheses_nolib 11 >> Result_nolib
./parentheses_nolib 12 >> Result_nolib
