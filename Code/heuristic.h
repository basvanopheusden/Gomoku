#ifndef HEURISTIC_H_INCLUDED
#define HEURISTIC_H_INCLUDED
//#define DEFAULT_WEIGHTS

#include "board.h"
#include <random>
#include <vector>
#include <map>
using namespace std;

struct pattern{
  uint64 pieces;
  uint64 pieces_empty;
  int n;
  double weight_act;
  double weight_pass;
  double drop_rate;
  int ind;
  pattern(uint64,uint64,int,double*,double*,double*,int);
  bool is_active(board&);
  bool contained(board&);
  bool contained(board&,bool);
  bool just_active(board&);
  bool isempty(uint64);
  uint64 missing_pieces(board&,bool);
  double diff_act_pass();
  void update(double*,double*,double*);
};

class heuristic{
  public:
    static const unsigned int Nweights=17;
    double center_weight;
    double w_act[Nweights];
    double w_pass[Nweights];
    double delta[Nweights];
    unsigned int Nfeatures;
    double D0,K0;
    double stopping_thresh,pruning_thresh,gamma,lapse_rate,opp_scale;
    double exploration_constant;
    bool self;
    int iterations;
    double tree_stats[4];
    vector<pattern> feature;
    vector<pattern> feature_backup;
    map<uint64,double> vtile;
    //vector<pattern> feature_backup2;
    mt19937_64 engine;
    geometric_distribution<int> iter_dist;
    normal_distribution<double> noise;
    bernoulli_distribution lapse;
    bernoulli_distribution K_dist;
    bernoulli_distribution D_dist;
    int freq[4*BOARD_WIDTH][4*BOARD_WIDTH];
    heuristic();
    void get_params_from_file(char*,int);
    void get_params_from_file(char*,int,int);
    void get_params_from_matlab(double*);
    void get_features_from_file(char*);
    void addfeature(uint64, int );
    void addfeature(uint64, uint64, int , int );
    void update();
    void seed_generator(mt19937_64);
    void perturb_weights(double);
    void update_weights();
    double evaluate(board, zet);
    double evaluate(board);
    void write(ostream&);
    void write_to_header(char*);
    vector<zet> get_pruned_moves(board&, bool);
    vector<zet> get_moves(board&, bool, bool);
    vector<zet> get_moves(board&, uint64, uint64, bool);
    void check_moves(vector<zet>,board);
    double likelihood(board, zet);
    zet makerandommove(board,bool);
    zet makemove_dfs(board,bool);
    zet makemove_dfs(board,uint64,uint64,bool);
    zet makemove_bfs(board,bool);
    zet makemove_bfs2(board,bool);
    zet makemove_bfs3(board,bool);
    zet makemove_bfs(board,uint64,uint64,bool);
    zet makerandommove(board,uint64,uint64,bool);
    zet makemove_mcts(board,bool);
    int playout(board,bool);
    void remove_features();
    void restore_features();
    void eliminate_noise();
};

class superheuristic{
  public:
    heuristic* h;
    int N;
    mt19937_64 engine;
  public:
    void seed_generator(mt19937_64 generator){
      engine.seed(generator());
    }
    void init(heuristic* subject,int Nplayers){
      h=subject;
      N=Nplayers;
    }
    zet makemove_bfs(board, bool);
};

#endif // HEURISTIC_H_INCLUDED
