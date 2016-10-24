#!/bin/bash
#PBS -N Gomoku
#PBS -l nodes=1:ppn=20
#PBS -l walltime=15:00:00
#PBS -l mem=6GB
#PBS -t 0-199
#PBS -j oe
#PBS -M svo213@nyu.edu
#PBS -m abe

data=hvh
model=final_no2conn
Nplayers=40
player=$((${PBS_ARRAYID}%$Nplayers))
group=$((${PBS_ARRAYID}/$Nplayers+1))
direc=$SCRATCH/Gomoku/${data}/${model}

module purge
module load matlab/2014a gcc/4.9.2
export MATLABPATH=$MATLABPATH:/scratch/svo213/Gomoku/Code:$direc
export LD_PRELOAD=$GCC_LIB/libstdc++.so

cd $direc
mkdir subject_${player}_group_${group}
cd subject_${player}_group_${group}
cp $direc/times.txt .
mkdir Output
rm Output/out*

echo "${data} ${model} ${player} ${group}"

echo "Gomoku_optim_mcs($player,$group); getbestx($player,$group,'${data}','final'); exit;" | matlab -nodisplay

echo "Done"

