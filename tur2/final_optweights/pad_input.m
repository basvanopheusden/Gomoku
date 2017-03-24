function x=pad_input(x)
load wopt.mat
x=[10000*ones(1,size(x,2)); x([1:2 4],:); ones(2,size(x,2)); repmat(x(5,:),35,1).*repmat(wopt,1,size(x,2)); repmat(x(3,:),17,1)];
end
