for i=1:size(models,2)
  cd(['../final_' models{i}])
  init0
  x=x0(:,2);
  y=pad_input(x);
  fprintf('model: %s,\ttest_pad: %f,\ttest_shrink: %f\nx: %s\n',models{i},sum(abs(x-shrink_input(pad_input(x)))),sum(abs(y-pad_input(shrink_input(y)))),sprintf('%.3f ', x'));

end


