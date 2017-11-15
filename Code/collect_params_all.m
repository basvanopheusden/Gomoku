function collect_params_all()
	allN = [40 40 10 50 50 50 50 40];
	alldata = {'hvh','gen','eye','learn1','learn2','learn3','tai1','tai2'};
	allmodels = {'final','final_noprune','final_nodelta','final_noact','final_notree','final_nocenter','final_no2conn','final_no2unc','final_no3','final_no4','final_nonoise','final_weight_hvd','final_triangle','final_drop_hvd','final_drop_type','final_opp','final_fixed_iters','final_fixed_branch','final_fixed_depth','final_mcts_myopic','final_mcts_rand','final_drop_tile','final_optweights'};
	for d=1:8
		for m=1:length(allmodels)
			fprintf('%s, %s\n',alldata{d},allmodels{m})
			cd(['/scratch/svo213/Gomoku/' alldata{d} '/' allmodels{m}])
			[params,loglik] = collect_params(allN(d),5);
			dlmwrite(['params_' alldata{d} '_' allmodels{m} '.txt'],params','Delimiter','\t')
			dlmwrite(['params_' alldata{d} '_' allmodels{m} '_short.txt'],shrink_input(params)','Delimiter','\t')
		end
	end
end
