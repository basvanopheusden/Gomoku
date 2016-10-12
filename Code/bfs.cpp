#include "bfs.h"
#include "heuristic.h"

mt19937_64 node::engine;

zet heuristic::makemove_bfs(board b,bool player){
  node game_tree(b,evaluate(b),player,1);
  node *n=&game_tree;
  uint64 mold,m=0x0ULL;
  int t=0,tmax=stopping_thresh;
  vector<zet> candidates;
  int max_iterations=iter_dist(engine)+1;
  game_tree.seed_engine(engine);
  iterations=0;
  if(lapse(engine))
    return makerandommove(b,player);
  remove_features();
  self=player;
  while(iterations<max_iterations && t<tmax && !game_tree.determined()){
    candidates=get_pruned_moves(n->b,n->player);
    n->expand(candidates);
    n=game_tree.select();
    mold=m;
    m=game_tree.bestmove().zet_id;
    if(mold==m)
      t++;
    else t=0;
    iterations++;
  }
  //tree_stats[0]=iterations;
  //tree_stats[1]=game_tree.Nleaves();
  //game_tree.add_squares(freq);
  //game_tree.add_depths(freq);
  //game_tree.add_Nchildren(freq);
  restore_features();
  //cout<<game_tree.diff_best_secondbest()<<endl;
  return game_tree.bestmove();
}

zet heuristic::makemove_bfs(board b,uint64 m1, uint64 m2, bool player){
  node game_tree(b,evaluate(b),player,1);
  node *n=&game_tree;
  vector<zet> candidates;
  int max_iterations=iter_dist(engine);
  game_tree.seed_engine(engine);
  iterations=0;
  if(lapse(engine))
    return makerandommove(b,m1,m2,player);
  remove_features();
  self=player;
  game_tree.expand(get_moves(b,m1,m2,player));
  n=game_tree.select();
  while(iterations<max_iterations && !game_tree.determined()){
    candidates=get_pruned_moves(n->b,n->player);
    n->expand(candidates);
    n=game_tree.select();
    iterations++;
  }
  restore_features();
  return game_tree.bestmove();
}
