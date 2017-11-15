#!/bin/bash
#SBATCH --nodes=1
#SBATCH --array=0
#SBATCH --cpus-per-task=20
#SBATCH --time=24:00:00
#SBATCH --mem=6GB
#SBATCH --job-name=Gomoku
#SBATCH --mail-type=ALL
#SBATCH --mail-user=svo213@nyu.edu
#SBATCH --output=Gomoku_%j.out

alldatas=(tai2)

allNs=(40)

allmodels=(final_optweights)

allmodels2=(final)


data=${alldatas[${SLURM_ARRAY_TASK_ID}]}
Nplayers=${allNs[${SLURM_ARRAY_TASK_ID}]}

model=${allmodels[${SLURM_ARRAY_TASK_ID}]}
model2=${allmodels2[${SLURM_ARRAY_TASK_ID}]}

direc=$SCRATCH/Gomoku/${data}/${model}

cd $direc
mkdir Output
rm Output/out.txt

module purge
module load matlab/2016b
export MATLABPATH=$MATLABPATH:$SCRATCH/Gomoku/Code:$SCRATCH/Gomoku/Code/mcs:$SCRATCH/Gomoku/Code/mcs/gls:$SCRATCH/Gomoku/Code/mcs/minq5:$direc

echo $model $data

echo "cross_validate('${data}','${model}','${model2}',$Nplayers); exit;" | matlab -nodisplay

echo "Done"

