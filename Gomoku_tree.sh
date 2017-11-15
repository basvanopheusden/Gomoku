#!/bin/bash
#SBATCH --nodes=1
#SBATCH --array=0-133
#SBATCH --cpus-per-task=1
#SBATCH --time=1:00:00
#SBATCH --mem=6GB
#SBATCH --job-name=Gomoku
#SBATCH --mail-type=ALL
#SBATCH --mail-user=svo213@nyu.edu
#SBATCH --output=Gomoku_%j.out

allmodels=(final final_noprune final_nodelta final_noact final_notree final_nocenter final_no2conn final_no2unc final_no3 final_no4 final_nonoise final_weight_hvd final_triangle final_drop_hvd final_drop_type final_opp final_optweights)
alldatas=(hvh gen eye learn1 learn2 learn3 tai1 tai2 tur1 tur2)
allNs=(40 40 10 50 50 50 50 40 30 15 15)

k=$((${SLURM_ARRAY_TASK_ID}/17))
j=$((${SLURM_ARRAY_TASK_ID}%17))

data=${alldatas[$k]}
Nplayers=${allNs[$k]}

model=${allmodels[$j]}

direc=$SCRATCH/Gomoku/${data}/${model}

cd $direc

./scratch/svo213/Gomoku/Code/compute_tree_size params_${data}_${model}.txt $Nplayers 100 > tree_size_${data}_${model}.txt

echo "Done"

