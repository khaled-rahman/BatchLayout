#!/bin/bash -l

#SBATCH -N 1
#SBATCH -p azad
#SBATCH -t 24:30:00
#SBATCH -J tools
#SBATCH -o tools.o%j

module load gcc
module load lang/java-jdk

srun -p azad -N 1 -n 1 -c 1 bash runtools.sh
