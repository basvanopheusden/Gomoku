#!/bin/bash
#PBS -N Gomoku_tour
#PBS -l nodes=1:ppn=1
#PBS -l walltime=00:15:00
#PBS -l mem=1GB 
#PBS -t 0-19
#PBS -j oe
#PBS -M svo213@nyu.edu
#PBS -m abe

first=$((200*${PBS_ARRAYID}/20))
last=$((200*(${PBS_ARRAYID}+1)/20))

module purge
module load gcc/4.9.2

cd $SCRATCH/Gomoku

time ./Gomoku_tournament $first $last hvh/newcenter/params_hvh_newcentere.txt tournament$first-$last.txt

echo "Done"

