function getbestx(player,group,dataset,model)
		
	filename='mcsresult.mat';
	if exist(filename,'file')
		load(filename)
		L=zeros(nbasket,10);
		for j=1:10
			for i=1:nbasket
				L(i,j)=Gomoku_wrapper({player,group,dataset,model},xmin(:,i))
			end
		end
		[l,i]=min(mean(L,2))
		x=xmin(:,i)
		L
		save bestresult.mat x L l i -mat
		dlmwrite('params_inferred.txt',pad_input(x),'delimiter','\t')
	end
end
