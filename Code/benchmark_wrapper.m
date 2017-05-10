function varargout = benchmark_wrapper(n,x,data)
%BENCHMARK_WRAPPER A black-box wrapper for the benchmark suite.
% 
%   The wrapper is called in two different ways:
%
%   [LB,UB,PLB,PUB,XMIN,DATA] = BENCHMARK_WRAPPER(N) takes a dataset or 
%   subproblem identifier N as input (a non-negative integer) and returns: 
%   lower/upper bounds LB and UB for the problem variables, plausible 
%   lower/upper bounds PLB and PUB, the location of the true minimum XMIN 
%   (if known), an auxiliary data structure DATA (possibly loaded from file).
%   You can set PLB=LB and PUB=UB if plausible variable ranges are unknown.
%   If XMIN is unknown, return a vector of NaNs. LB, UB, PLB, PUB and XMIN
%   are assumed to be row vectors of length NVARS, where NVARS is the number 
%   of variables. DATA can be the empty matrix if no auxiliary data are 
%   necessary.
%
%   FVAL = BENCHMARK_WRAPPER(N,X,DATA) takes as input a dataset identifier N, 
%   a variable vector X and a dataset structure DATA (as previously returned) 
%   and returns the value of the objective function evaluated at X.
%   For many problems, the objective function is the negative log likelihood.
%

if nargin == 1
    % Initialization call -- define problem and set up data
    
    lb =[0.1 0.001 0.0 0.01 0.25 -inf*ones(5,1)];
    ub =[10.0 1 1.0 1.0 4.0 inf*ones(5,1)];
    plb =[0.1 0.001 0.0 0.01 0.5 zeros(5,1)];
    pub =[10.0 1 0.5 0.2 2.0 20*ones(5,1)];
    xmin=NaN(1,10);
	s=[2 3 33 28 29 32 37 36 14 15 0 1 13 12 34 35 30 22 31 20 21 23 17 16 25 24 18 19 5 4 7 39 6 38 10 11 26 27 9 8]; %subject indices ordered by data size
	data=s(n);
 
    varargout = {lb,ub,plb,pub,xmin,data};
    
else
    % Iteration call -- evaluate objective function
    
 
	if exist(fullfile(pwd,'times.txt'),'file')==2
		data_size=[307 298 206 202 202 202 202 201 185 181 161 160 156 153 146 146 141 139 139 138 132 132 131 130 112 109 105 101 93 92 92 91 90 89 83 82 40 40 37 36];
		x=x(:);
		y=[10000; x([1:2 4]); 1; 1; x(6); repmat(x(7:10),4,1); 0; repmat(x(5),16,1).*repmat(x(7:10),4,1); 0; repmat(x(3),17,1)];
		fval = data_size(n)*Gomoku_luigi(data,0,y);
	else
		error('Please move times.txt to the present working directory')
	end

	varargout = {fval};    
end

end