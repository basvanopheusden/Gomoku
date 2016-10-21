#include "heuristic.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <iomanip>

pattern::pattern(uint64 p,uint64 pe,int n_val,double* w_act,double* w_pass,double* delta,int i){
  pieces=p;
  pieces_empty=pe;
  n=n_val;
  ind=i;
  weight_act=w_act[i];
  weight_pass=w_pass[i];
  drop_rate=delta[i];
}

void pattern::update(double* w_act, double* w_pass, double* delta){
  weight_act=w_act[ind];
  weight_pass=w_pass[ind];
  drop_rate=delta[ind];
}

inline double pattern::diff_act_pass(){
  return weight_act-weight_pass;
}

inline bool pattern::contained(board& b){
  return b.contains(pieces,BLACK) || b.contains(pieces,WHITE);
}

inline bool pattern::contained(board& b,bool player){
  return b.contains(pieces,player);
}

inline bool pattern::is_active(board& b){
  return b.Nempty(pieces_empty)>=n;
}

inline bool pattern::just_active(board& b){
  return b.Nempty(pieces_empty)==n;
}

inline uint64 pattern::missing_pieces(board& b, bool player){
  return pieces & (~b.pieces[player]);
}

inline bool pattern::isempty(uint64 m){
  return m & pieces_empty;
}

void heuristic::get_params_from_file(char* filename,int subject=0){
  get_params_from_file(filename,subject,0);
}

void heuristic::get_params_from_file(char* filename,int subject=0,int group=0){
  ifstream input(filename,ios::in);
  string s;
  if(!input)
    cout<<"could not open input"<<endl;
  for(int i=0;i<5*subject+group-1;i++)
    getline(input,s);
  input>>stopping_thresh>>pruning_thresh>>D0>>lapse_rate>>opp_scale>>center_weight;
  for(unsigned int i=0;i<Nweights;i++)
    input>>w_act[i];
  for(unsigned int i=0;i<Nweights;i++)
    input>>w_pass[i];
  for(unsigned int i=0;i<Nweights;i++)
    input>>delta[i];
  input.close();
  update();
}

void heuristic::get_params_from_matlab(double* input){
  stopping_thresh=input[0];
  pruning_thresh=input[1];
  D0=input[2];
  lapse_rate=input[3];
  opp_scale=input[4];
  exploration_constant=input[5];
  center_weight=input[6];
  for(unsigned int i=0;i<Nweights;i++)
    w_act[i]=input[i+7];
  for(unsigned int i=0;i<Nweights;i++)
    w_pass[i]=input[i+Nweights+7];
  for(unsigned int i=0;i<Nweights;i++)
    delta[i]=input[i+2*Nweights+7];
  update();
}

void heuristic::get_features_from_file(char* filename){
  uint64 c,e;
  int i,n;
  ifstream input(filename,ios::in);
  if(input.is_open())
    while(!input.eof()){
      input>>hex>>c>>e;
      input>>dec>>i>>n;
      addfeature(c,e,i,n);
    }
  input.close();
}

void heuristic::addfeature(uint64 config, int i){
  int shiftc=colmax(config);
  int shiftr=rowmax(config);
  for(int col=0;col<shiftc;col++)
    for(int row=0;row<shiftr;row++)
      feature.push_back(pattern(shift(config,row,col),0,0,w_act,w_pass,delta,i));
  Nfeatures=feature.size();
}

void heuristic::addfeature(uint64 config, uint64 confempty, int i, int n){
  int cmin=colmin(config,confempty,n);
  int cmax=colmax(config,confempty,n);
  int rmin=rowmin(config,confempty,n);
  int rmax=rowmax(config,confempty,n);
  board b;
  for(int col=cmin;col<cmax;col++)
    for(int row=rmin;row<rmax;row++)
      if(num_bits(shift(confempty,row,col))>=n)
        feature.push_back(pattern(shift(config,row,col),shift(confempty,row,col),n,w_act,w_pass,delta,i));
  Nfeatures=feature.size();
}

void heuristic::update(){
  iter_dist=geometric_distribution<int>(gamma);
  K_dist=bernoulli_distribution(fmod(K0,1.0));
  D_dist=bernoulli_distribution(fmod(D0,1.0));
  noise=normal_distribution<double>(0.0,1.0);
  lapse=bernoulli_distribution(lapse_rate);
  for(uint64 m=1;m!=boardend;m<<=1)
    vtile[m]=1.0/sqrt(pow(uint64totile(m)/BOARD_WIDTH-1.5,2) + pow(uint64totile(m)%BOARD_WIDTH-4.0,2));
  update_weights();
}

void heuristic::update_weights(){
  for(unsigned int i=0;i<Nfeatures;i++)
    feature[i].update(w_act,w_pass,delta);
}

void heuristic::eliminate_noise(){
  lapse_rate=0.0;
  for(unsigned int i=0;i<Nweights;i++)
    delta[i]=0.0;
  for(unsigned int i=0;i<Nfeatures;i++)
    feature[i].drop_rate=0.0;
  noise=normal_distribution<double> (0.0,0.0);
  lapse=bernoulli_distribution(0.0);
}

void heuristic::seed_generator(mt19937_64 generator){
  engine.seed(generator());
}

double heuristic::evaluate(board b, zet m){
  return evaluate(b+m);
}

double heuristic::evaluate(board b){
  bool player=b.active_player();
  double temp=0.0;
  for(uint64 m=1;m!=boardend;m<<=1){
    if(b.contains(m,player))
      temp+=center_weight*vtile[m];
    if(b.contains(m,!player))
      temp-=center_weight*vtile[m];
  }
  for(unsigned int i=0;i<Nfeatures;i++)
    if(feature[i].is_active(b)){
      if(feature[i].contained(b,player))
        temp+=feature[i].weight_act;
      else if(feature[i].contained(b,!player))
        temp-=feature[i].weight_pass;
  }
  return player==BLACK?temp:-temp;
}

void heuristic::remove_features(){
  unsigned int i=0;
  while(i<Nfeatures){
    if(bernoulli_distribution{feature[i].drop_rate}(engine)){
      feature_backup.push_back(feature[i]);
      feature.erase(feature.begin()+i);
      Nfeatures--;
    }
    else i++;
  }
}

void heuristic::restore_features(){
  for(unsigned int i=0;i<feature_backup.size();i++){
    feature.push_back(feature_backup[i]);
    Nfeatures++;
  }
  feature_backup.clear();
}

void heuristic::write_to_header(char* filename){
  ofstream headout(filename,ios::out);
  headout<<"#include \"board.h\"\n"
  <<"#include \"heuristic.h\"\n\n"
  <<"#ifdef DEFAULT_WEIGHTS\n"
  <<"heuristic::heuristic(): weight {0.8,1.0,0.3,6.0,2.5,10.0,1.0,0.3,6.0,2.5,10.0,1.0,0.3,6.0,2.5,10.0,1.2},\n"
  <<"Nfeatures(0), D0(6.0), K0(5.0), gamma(0.025), delta(0.2), lapse_rate(0.01), opp_scale(1.5){\n"
  <<"\tupdate();\n}\n"
  <<"#else\n"
  <<"heuristic::heuristic(): Nfeatures("<<Nfeatures<<"), stopping_thresh("<<stopping_thresh<<"), pruning_thresh("
  <<pruning_thresh<<"),\ngamma("<<gamma<<") ,lapse_rate("<<lapse_rate<<"), opp_scale("<<opp_scale<<"),\nw_act{";
  for(unsigned int i=0;i<Nweights;i++)
    headout<<w_act[i]<<((i==Nweights-1)?"},\nw_pass{":",");
  for(unsigned int i=0;i<Nweights;i++)
    headout<<w_pass[i]<<((i==Nweights-1)?"},\ndelta{":",");
  for(unsigned int i=0;i<Nweights;i++)
    headout<<delta[i]<<((i==Nweights-1)?"},\nfeature{":",");
  for(unsigned int i=0;i<Nfeatures;i++)
    headout<<"{0x"<<hex<<feature[i].pieces<<"ULL,0x"<<feature[i].pieces_empty<<dec<<"ULL,"<<feature[i].n
    <<",w_act,w_pass,delta,"<<feature[i].ind<<((i==Nfeatures-1)?"}}{\n\tupdate();\n}\n":"},\n");
  headout<<"#endif";
  headout.close();
}

vector<zet> heuristic::get_moves(board& b, bool player, bool nosort=false){
  vector<zet> candidate;
  unsigned int i;
  uint64 m,m1,m2;
  map<uint64,int> lookup;
  double deltaL=0.0;
  double c_act,c_pass;
  if(player==self){
    c_act=2.0*opp_scale/(1.0+opp_scale);
    c_pass=2.0/(1.0+opp_scale);
  }
  else{
    c_act=2.0/(1.0+opp_scale);
    c_pass=2.0*opp_scale/(1.0+opp_scale);
  }
  for(i=0;i<Nfeatures;i++)
    if(feature[i].is_active(b)){
      if(feature[i].contained(b,player))
        deltaL-=c_pass*feature[i].diff_act_pass();
      else if(feature[i].contained(b,!player))
        deltaL-=c_act*feature[i].diff_act_pass();
    }
  for(i=1,m=1;m!=boardend;m<<=1){
    if(b.isempty(m)){
      candidate.push_back(zet(m,deltaL+center_weight*vtile[m]+noise(engine),player));
      lookup[m]=i;
      i++;
    }
  }
  for(i=0;i<Nfeatures;i++)
    if(feature[i].is_active(b)){
      m1=feature[i].missing_pieces(b,player);
      m2=feature[i].missing_pieces(b,!player);
      if((m1 & m2) && has_one_bit(m1))
        if(lookup[m1])
          candidate[lookup[m1]-1].val+=c_pass*feature[i].weight_pass;
      if(m1==0 && feature[i].just_active(b))
        for(m=1;m!=boardend;m<<=1)
          if(b.isempty(m) && feature[i].isempty(m))
            if(lookup[m])
              candidate[lookup[m]-1].val-=c_pass*feature[i].weight_pass;
      if(m2==0 && feature[i].just_active(b))
        for(m=1;m!=boardend;m<<=1)
          if(b.isempty(m) && feature[i].isempty(m))
            if(lookup[m])
              candidate[lookup[m]-1].val+=c_act*feature[i].weight_act;
    }
  if(nosort)
    return candidate;
  sort(candidate.begin(),candidate.end(),compare);
  return candidate;
}

/*vector<zet> heuristic::get_pruned_moves(board& b, bool player,unsigned int K){
  vector<zet> candidate=get_moves(b,player);
  if(candidate.size()>K)
    candidate.erase(candidate.begin()+K,candidate.end());
  return candidate;
}*/

vector<zet> heuristic::get_pruned_moves(board& b, bool player){
  vector<zet> candidate=get_moves(b,player);
  unsigned int i=1;
  while(i<candidate.size() && abs(candidate[0].val-candidate[i].val)<pruning_thresh)
    i++;
  if(i<candidate.size())
    candidate.erase(candidate.begin()+i,candidate.end());
  return candidate;
}

vector<zet> heuristic::get_moves(board& b, uint64 m1,uint64 m2, bool player){
  vector<zet> candidate=get_moves(b,player,true);
  vector<zet> Retval;
  for(vector<zet>::iterator it=candidate.begin();it!=candidate.end();++it)
    if(it->zet_id==m1 || it->zet_id==m2)
      Retval.push_back(*it);
  return Retval;
}

void heuristic::check_moves(vector<zet> candidates,board b){
  zet m;
  for(unsigned int i=0;i<candidates.size();i++){
    m=candidates[i];
    if(abs(evaluate(b+m)-evaluate(b)-(m.player==BLACK?m.val:-m.val))>0.01){
      b.write(m);
      (b+m).write();
      cout<<evaluate(b)<<"\t"<<evaluate(b,m)<<"\t"<<(m.player==BLACK?m.val:-m.val)<<endl;
      cin.get();
    }
  }
}

zet heuristic::makerandommove(board bstart, bool player){
  vector<uint64> options;
  int Noptions;
  for(uint64 m=1;m!=boardend;m<<=1)
    if(bstart.isempty(m))
      options.push_back(m);
  Noptions=options.size();
  if(Noptions>0)
    return zet(options[uniform_int_distribution<int>{0,Noptions-1}(engine)],0.0,player);
  return zet(0,0.0,player);
}

zet heuristic::makerandommove(board b, uint64 m1,uint64 m2,bool player){
  if(bernoulli_distribution{0.5}(engine))
    return zet(m1,0.0,player);
  return zet(m2,0.0,player);
}

zet heuristic::makemove_bfs(board bstart,bool player){
  int depth=D_dist(engine)+D0;
  unsigned int K=K_dist(engine)+K0;
  double alpha[depth+1];
  double beta[depth+1];
  board b[depth+1];
  double L[depth+1];
  unsigned int i[depth+1];
  vector<zet> candidate[depth+1];
  int level=0;
  int ibest=-1;
  bool onreturn=false;
  zet m;
  double retval;
  alpha[0]=-10000.0;
  beta[0]=10000.0;
  L[0]=evaluate(bstart);
  b[0]=bstart;
  iterations=0;
  if(lapse(engine))
    return makerandommove(bstart,player);
  remove_features();
  self=player;
  while(level!=-1){
    if(onreturn){
      level--;
      if(level!=-1){
        if((player+level)%2==BLACK && retval>alpha[level]){
          alpha[level]=retval;
          if(level==0){
            ibest=i[0];
            candidate[0][ibest].val=retval;
          }
        }
        if((player+level)%2==WHITE && retval<beta[level]){
          beta[level]=retval;
          if(level==0){
            ibest=i[0];
            candidate[0][ibest].val=retval;
          }
        }
        i[level]++;
        onreturn=false;
      }
    }
    else if(b[level].black_has_won())
      retval=10000.0-level-0.1*noise(engine),onreturn=true;
    else if(b[level].white_has_won())
      retval=-10000.0+level+0.1*noise(engine),onreturn=true;
    else if(b[level].is_full())
      retval=0.0,onreturn=true;
    else if(level==depth){
      retval=L[level];
      onreturn=true;
    }
    else if(candidate[level].empty()){
      iterations++;
      candidate[level]=get_pruned_moves(b[level],(player+level)%2);
      i[level]=0;
    }
    else if((player+level)%2==WHITE && (i[level]==candidate[level].size() || beta[level]<=alpha[level]))
      retval=beta[level],onreturn=true;
    else if((player+level)%2==BLACK && (i[level]==candidate[level].size() || alpha[level]>=beta[level]))
      retval=alpha[level],onreturn=true;
    else {
      m=candidate[level][i[level]];
      if((player+level)%2==BLACK)
        L[level+1]=L[level]+m.val;
      else L[level+1]=L[level]-m.val;
      b[level+1]=b[level]+m;
      alpha[level+1]=alpha[level];
      beta[level+1]=beta[level];
      candidate[level+1].clear();
      level++;
    }
  }
  restore_features();
  if(ibest==-1)
    return zet(0,0.0,player);
  return candidate[0][ibest];
}

zet heuristic::makemove_dfs(board bstart,uint64 m1, uint64 m2, bool player){
  int depth=D_dist(engine)+D0;
  unsigned int K=K_dist(engine)+K0;
  double alpha[depth+1];
  double beta[depth+1];
  board b[depth+1];
  double L[depth+1];
  unsigned int i[depth+1];
  vector<zet> candidate[depth+1];
  int level=0;
  int ibest=-1;
  bool onreturn=false;
  zet m;
  double retval;
  alpha[0]=-10000.0;
  beta[0]=10000.0;
  L[0]=evaluate(bstart);
  b[0]=bstart;
  iterations=0;
  if(lapse(engine))
    return makerandommove(bstart,player);
  remove_features();
  self=player;
  while(level!=-1){
    if(onreturn){
      level--;
      if(level!=-1){
        if((player+level)%2==BLACK && retval>alpha[level]){
          alpha[level]=retval;
          if(level==0){
            ibest=i[0];
            candidate[0][ibest].val=retval;
          }
        }
        if((player+level)%2==WHITE && retval<beta[level]){
          beta[level]=retval;
          if(level==0){
            ibest=i[0];
            candidate[0][ibest].val=retval;
          }
        }
        i[level]++;
        onreturn=false;
      }
    }
    else if(b[level].black_has_won())
      retval=10000.0-level-0.1*noise(engine),onreturn=true;
    else if(b[level].white_has_won())
      retval=-10000.0+level+0.1*noise(engine),onreturn=true;
    else if(b[level].is_full())
      retval=0.0,onreturn=true;
    else if(level==depth){
      retval=L[level];
      onreturn=true;
    }
    else if(candidate[level].empty()){
      iterations++;
      if(level==0)
        candidate[level]=get_moves(b[level],m1,m2,(player+level)%2);
      else candidate[level]=get_pruned_moves(b[level],(player+level)%2);
      i[level]=0;
    }
    else if((player+level)%2==WHITE && (i[level]==candidate[level].size() || beta[level]<=alpha[level]))
      retval=beta[level],onreturn=true;
    else if((player+level)%2==BLACK && (i[level]==candidate[level].size() || alpha[level]>=beta[level]))
      retval=alpha[level],onreturn=true;
    else {
      m=candidate[level][i[level]];
      if((player+level)%2==BLACK)
        L[level+1]=L[level]+m.val;
      else L[level+1]=L[level]-m.val;
      b[level+1]=b[level]+m;
      alpha[level+1]=alpha[level];
      beta[level+1]=beta[level];
      candidate[level+1].clear();
      level++;
    }
  }
  restore_features();
  if(ibest==-1)
    return zet(0,0.0,player);
  return candidate[0][ibest];
}

zet superheuristic::makemove_bfs(board b, bool player){
  uniform_int_distribution<int> dist(0,N-1);
  int subject=dist(engine);
  return h[subject].makemove_bfs(b,player);
}
