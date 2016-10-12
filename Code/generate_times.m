function generate_times(filename_in,filename_out,c)
  
    %this controls the expected total computational time

    out=load(filename_in);
    N=max(out(:,1))+1;
    %Number of trials

    x=linspace(1e-6,1-1e-6,1e6);
    dilog=pi^2/6+cumsum(log(x)./(1-x))/length(x);
    L=[0 cumsum(1./(1:1e6))];
    l=accumarray(out(:,1)+1,L(out(:,3)))./histc(out(:,1)+1,1:N);
    %negative loglik estimate for each trial
    p=exp(-l);
    %convert to probability estimate
    lambda=c/mean(interp1(x,sqrt(dilog./x),p,'spline','extrap'));
    %lagrange multiplier to enforce computational time 
    n=lambda*interp1(x,sqrt(x.*dilog),p,'spline','extrap');
    %times for each trial
    dlmwrite(filename_out,max(1,uint8(n)),'\n')
end
