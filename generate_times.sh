#!/bin/bash
#PBS -N Gomoku
#PBS -l nodes=1:ppn=20
#PBS -l walltime=1:00:00
#PBS -l mem=6GB
#PBS -t 0-74
#PBS -j oe
#PBS -M svo213@nyu.edu
#PBS -m abe

data=tur2
model=final
Nplayers=15
player=$((${PBS_ARRAYID}%$Nplayers))
group=$((${PBS_ARRAYID}/$Nplayers+1))
direc=$SCRATCH/Gomoku/${data}/${model}

module purge
module load gcc/4.9.2

cd $direc

echo "${data} ${model} ${player} ${group}"

time for i in {0..99}; do ./../../Code/generate_times_${data} ${player} ${group} out_${player}_${group}.txt; done

echo "Done"

