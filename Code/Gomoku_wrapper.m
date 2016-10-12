function r=Gomoku_wrapper(data,x)
  player=data{1};
  group=data{2};
  dataset=data{3};
  model=data{4};
  r=feval(['Gomoku_' dataset '_' model],player,group,pad_input(x));
  fprintf('x: %s, L: %f\n', sprintf('%3f ', x),r);
end
