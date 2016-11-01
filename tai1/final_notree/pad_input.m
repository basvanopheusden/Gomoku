function x=pad_input(x)
x=[10000*ones(2,size(x,2)); ones(1,size(x,2)); x(2,:); ones(2,size(x,2)); x(4,:); repmat(x(5:8,:),4,1); zeros(1,size(x,2)); repmat(x(3,:),16,1).*repmat(x(5:8,:),4,1); zeros(1,size(x,2)); repmat(x(1,:),17,1)];
end
