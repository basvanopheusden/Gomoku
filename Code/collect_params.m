function [params,loglik]=collect_params(nsubjects,ngroups)
  load x0.mat
  nparams=size(shrink_input(x0),1);
  allx=zeros(nparams,nsubjects*ngroups);
  allL=zeros(1,nsubjects*ngroups);
  for j=1:nsubjects
    for k=1:ngroups
      filename=['subject_' num2str(j-1) '_group_' num2str(k) '/bestresult.mat'];
      if exist(filename)
        load(filename)
        allx(:,(j-1)*ngroups+k)=x;
        allL((j-1)*ngroups+k)=l;
      else 
		filename=['subject_' num2str(j-1) '_group_' num2str(k) '/params_inferred.txt'];
	    x = load(filename);
        allx(:,(j-1)*ngroups+k)=x;
		fprintf('file not found:%i,%i\n',j,k)
      end
  end
  loglik=allL;
  params=pad_input(allx);
end

