function r=Gomoku_wrapper(data,x)
  player=data{1};
  group=data{2};
  dataset=data{3};
  model=data{4};
  if strcmp(dataset,'tai2')==1
    r=feval(['Gomoku_' dataset '_' model],player+50,group,pad_input(x));
  else r=feval(['Gomoku_' dataset '_' model],player,group,pad_input(x));
  end
  fprintf('x: %s, L: %f\n', sprintf('%3f ', x),r);
end
