function x=pad_input(x)
x=[10000*ones(1,size(x,2)); x([1:2 4],:); ones(2,size(x,2)); x(8,:); [ones(4,size(x,2)); repmat(x(5,:),4,1); repmat(x(6,:),8,1)].*repmat(x(9:12,:),4,1); zeros(1,size(x,2)); repmat(x(7,:),16,1).*[ones(4,size(x,2)); repmat(x(5,:),4,1); repmat(x(6,:),8,1)].*repmat(x(9:12,:),4,1); zeros(1,size(x,2)); repmat(x(3,:),17,1)];
end
