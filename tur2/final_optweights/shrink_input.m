function x=shrink_input(x)
load wopt.mat
x=[x([2:3 42 4],:); mean(x(7:41,:)./repmat(wopt,1,size(x,2)))];
end
