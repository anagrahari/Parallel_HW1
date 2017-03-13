#!/bin/bash
#SBATCH -J Parentheses        # Job Name
#SBATCH -o Test.o%j    # Output and error file name (%j expands to jobID)
#SBATCH -n 8           # Total number of  tasks requested
#SBATCH -p development  # Queue (partition) name -- normal, development, etc.
#SBATCH -t 02:00:00     # Run time (hh:mm:ss) - 2.5 hours
rm -f Result 
touch Result
./parentheses 1 >> Result
./parentheses 2 >> Result
./parentheses 3 >> Result
./parentheses 4 >> Result
./parentheses 5 >> Result
./parentheses 6 >> Result
./parentheses 7 >> Result
./parentheses 8 >> Result
./parentheses 9 >> Result
./parentheses 10 >> Result
./parentheses 11 >> Result
./parentheses 12 >> Result
