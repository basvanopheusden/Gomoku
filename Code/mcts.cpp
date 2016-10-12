#include "mcts.h"
#include "heuristic.h"
using namespace MCTS;

node::node(board b_val, bool p_val): b(b_val), player(p_val), parent(NULL),
    child(NULL), m(0x0ULL), Nvisits(0), Ndraws(0), Nwins(0), Nchildren(0){}

node::~node(){
  if(child){
    for(unsigned int i=0;i<Nchildren;i++)
      delete child[i];
    delete[] child;
  }
  parent=NULL;
}

void node::expand(vector<zet> candidate){
  Nchildren=candidate.size();
  if(Nchildren>0){
    child=new node*[Nchildren];
    for(unsigned int i=0;i<Nchildren;i++){
      if(player==BLACK)
        child[i]=new node(b+candidate[i],!player);
      else child[i]=new node(b+candidate[i],!player);
      child[i]->parent=this;
      child[i]->m=candidate[i].zet_id;
    }
  }
}

node* node::select(double c){
  double v=-1000.0,vchild;
  node* n=NULL;
  for(unsigned int i=0;i<Nchildren;i++){
    vchild=(child[i]->Nvisits==0)?0.0:((((player==BLACK)?1.0:-1.0)*child[i]->Nwins)/child[i]->Nvisits
       +c*sqrt(log(Nvisits)/child[i]->Nvisits));
    if(vchild>v)
      v=vchild,n=child[i];
  }
  if(n)
    return n->select(c);
  return this;
}

void node::backpropagate(int r){
  Nvisits++;
  switch(r){
    case BLACK_WINS_GAME: Nwins++; break;
    case GAME_DRAWN: Ndraws++; break;
  }
  if(parent)
    parent->backpropagate(r);
}

zet node::bestmove(){
  int j=0;
  for(unsigned int i=1;i<Nchildren;i++)
    if(child[i]->Nvisits>child[j]->Nvisits)
      j=i;
  return zet(child[j]->m,0.0,player);
}

int heuristic::playout(board b, bool player){
  vector<zet> candidate;
  if(b.black_has_won())
    return BLACK_WINS_GAME;
  if(b.white_has_won())
    return WHITE_WINS_GAME;
  if(b.is_full())
    return GAME_DRAWN;
  candidate=get_moves(b,player,false);
  if(candidate.size()>0)
    return playout(b+candidate[0],!player);
  return 4994; //should never happen
}

zet heuristic::makemove_mcts(board b,bool player){
  node game_tree(b,player);
  node *n=&game_tree;
  int max_iterations=iter_dist(engine)+1;
  if(lapse(engine))
    return makerandommove(b,player);
  remove_features();
  self=player;
  for(iterations=0;iterations<max_iterations;iterations++){
    n->backpropagate(playout(n->b,n->player));
    n->expand(get_moves(n->b,n->player,false));
    n=game_tree.select(exploration_constant);
  }
  restore_features();
  return game_tree.bestmove();
}
