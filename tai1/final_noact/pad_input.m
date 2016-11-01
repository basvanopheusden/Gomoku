function x=pad_input(x)
x=[10000*ones(1,size(x,2)); x([1:2 4],:); ones(2,size(x,2)); x(5,:); repmat(x(6:9,:),4,1); zeros(1,size(x,2)); repmat(x(6:9,:),4,1); zeros(1,size(x,2)); repmat(x(3,:),17,1)];
end
