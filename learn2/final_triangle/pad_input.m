function x=pad_input(x)
x=[10000*ones(1,size(x,2)); x([1:2 4],:); ones(2,size(x,2)); x(6,:); repmat(x(7:10,:),4,1); x(11,:); repmat(x(5,:),17,1).*[repmat(x(7:10,:),4,1); x(11,:)]; repmat(x(3,:),17,1)];
end
