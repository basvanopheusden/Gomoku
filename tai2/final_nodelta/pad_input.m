function x=pad_input(x)
x=[10000*ones(1,size(x,2)); x(1:3,:); ones(2,size(x,2)); x(5,:); repmat(x(6:9,:),4,1); zeros(1,size(x,2)); repmat(x(4,:),16,1).*repmat(x(6:9,:),4,1); zeros(1,size(x,2)); zeros(17,size(x,2))];
end
