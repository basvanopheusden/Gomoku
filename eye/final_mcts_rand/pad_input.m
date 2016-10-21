function x=pad_input(x)
x=[10000*ones(2,size(x,2)); x(1:2,:); ones(1,size(x,2)); x(3,:); zeros(35,size(x,2))];
end
