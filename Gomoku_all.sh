#!/bin/bash
#PBS -N Gomoku
#PBS -l nodes=1:ppn=20
#PBS -l walltime=24:00:00
#PBS -l mem=6GB
#PBS -t 0-27599
#PBS -j oe
#PBS -M svo213@nyu.edu
#PBS -m abe 

i=${PBS_ARRAYID}
allmodels=(final final_noprune final_nodelta final_noact final_notree final_nocenter final_no2conn final_no2unc final_no3 final_no4 final_nonoise final_weight_hvd final_triangle final_drop_hvd final_drop_type final_opp final_fixed_iters final_fixed_branch final_fixed_depth final_mcts_myopic final_mcts_rand final_drop_tile final_opt_weights)
allmodels2=(final final final final final final final final final final final final final final final final final_fixed_iters final_fixed_branch final_fixed_depth mcts mcts final_drop_tile final)
alldatas=(hvh gen eye learn1 learn2 learn3 tai1 tai2 tur)
allNs=(40 40 10 50 50 50 50 40 30)

k=0
for j in 4600 9200 10350 16100 21850 27600 33350 41400; do
  if [ "$j" -lt "$i" ]; then
    k=$(($k+1));
  else break;
  fi;
done

data=${alldatas[$k]}
Nplayers=${allNs[$k]}

group=$(($i%5+1))
player=$((Nplayers-(($j-$i-1)/5)%$Nplayers-1))
l=$((22-($j-$i-1)/(5*$Nplayers)))

model=${allmodels[$l]}
model2=${allmodels2[$l]}

direc=$SCRATCH/Gomoku/${data}/${model}

module purge
module load matlab/2014a gcc/4.9.2
export MATLABPATH=$MATLABPATH:$SCRATCH/Gomoku/Code:$SCRATCH/Gomoku/Code/mcs:$SCRATCH/Gomoku/Code/mcs/gls:$SCRATCH/Gomoku/Code/mcs/minq5:$direc
export LD_PRELOAD=$GCC_LIB/libstdc++.so

cd $direc
mkdir subject_${player}_group_${group}
cd subject_${player}_group_${group}
cp $direc/times.txt .
mkdir Output
rm Output/out*

echo "Gomoku_optim_mcs($player,$group); getbestx($player,$group,'${data}','${model2}'); exit;" | matlab -nodisplay

echo "Done"

