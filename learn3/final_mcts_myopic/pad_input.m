function x=pad_input(x)
x=[10000*ones(2,size(x,2)); x([1 3]); ones(1,size(x,2)); x([4 6],:); repmat(x(7:10,:),4,1); zeros(1,size(x,2)); repmat(x(5,:),16,1).*repmat(x(7:10,:),4,1); zeros(1,size(x,2)); repmat(x(2,:),17,1)];
end
