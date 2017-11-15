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

data=tai2
model=final_optweights
#model2=$model
model2=final
Nplayers=40
player=12 #$((${SLURM_ARRAY_TASK_ID}%$Nplayers))
group=1 #$((${SLURM_ARRAY_TASK_ID}/$Nplayers+1))
direc=$SCRATCH/Gomoku/${data}/${model}

module purge
module load matlab/2016b
export MATLABPATH=$MATLABPATH:$SCRATCH/Gomoku/Code:$SCRATCH/Gomoku/Code/mcs:$SCRATCH/Gomoku/Code/mcs/gls:$SCRATCH/Gomoku/Code/mcs/minq5:$direc

cd $direc
mkdir subject_${player}_group_${group}
cd subject_${player}_group_${group}
cp $direc/times.txt .
mkdir Output
rm Output/out*

echo $model $data $player $group

echo "Gomoku_optim_mcs($player,$group); getbestx($player,$group,'${data}','${model2}'); exit;" | matlab -nodisplay

echo "Done"

