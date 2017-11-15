#!/bin/bash
allmodels="final final_noprune final_nodelta final_noact final_notree final_nocenter final_no2conn final_no2unc final_no3 final_no4 final_nonoise final_weight_hvd final_triangle final_drop_hvd final_drop_type final_opp final_fixed_iters final_fixed_branch final_fixed_depth final_mcts_myopic final_mcts_rand final_drop_tile final_optweights"
alldatas=(hvh gen eye learn1 learn2 learn3 tai1 tai2 tur1 tur2)
allNs=(40 40 10 50 50 50 50 40 30 15 15)

for m in $allmodels; do 
for i in {0..7}; do 
for n in $(seq 0 $((${allNs[$i]}-1))); do
for g in {1..5}; do
if [ ! -e ${alldatas[$i]}/$m/subject_${n}_group_${g}/bestresult.mat ]; then
echo ${alldatas[$i]} $m ${n} ${g}; 
fi;
done;
done;
done;
done;
