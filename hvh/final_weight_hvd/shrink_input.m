function x=shrink_input(x)
x=[x([2:3 42 4],:); x(12,:)./x(8,:); x(16,:)./x(8,:);  x(25,:)./x(8,:); x(7:11,:)];
end