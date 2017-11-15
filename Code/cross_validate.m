function cross_validate(data,model,model2,Nplayers)

params=load(['params_' data '_' model '_short.txt']);

for k=1:10
  for i=1:5*Nplayers
    player = floor((i-1)/5)
	group = -mod(i-1,5)-1
    allL(i,k) = Gomoku_wrapper({floor((i-1)/5),-mod(i-1,5)-1,data,model2},params(i,:)');
	dlmwrite(['loglik_' data '_' model '.txt'],allL,'Delimiter','\t')
  end
end
