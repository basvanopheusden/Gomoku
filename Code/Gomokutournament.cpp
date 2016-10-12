#include "heuristic.h"

#define WHITE_WINS_GAME -1
#define BLACK_WINS_GAME 1
#define GAME_DRAWN 0
#define UNDETERMINED 2

#include <ctime>
using namespace std;

int playgame(heuristic& h1, heuristic& h2){
  zet m;
  board b;
  bool player=BLACK;
  do {
    if(player==BLACK)
      m=h1.makemove_bfs(b,player);
    else m=h2.makemove_bfs(b,player);
    b.add(m);
    player=!player;
  } while(m.zet_id!=0);
  if(b.white_has_won())
    return WHITE_WINS_GAME;
  if(b.black_has_won())
    return BLACK_WINS_GAME;
  return GAME_DRAWN;
}

void round_robin(heuristic* participant,int Nparticipants,ofstream& output,int firstplayer,int lastplayer){
  int result;
  for(int i=firstplayer;i<lastplayer;i++){
    for(int j=0;j<Nparticipants;j++){
      result=playgame(participant[i],participant[j]);
      output<<result<<"\t"<<flush;
    }
    cout<<i<<endl;
    output<<endl;
  }
}

int main(int argc, char* argv[]){
  heuristic h[200];
  unsigned int seed=unsigned(time(0));
  mt19937_64 global_generator;
  int first=atoi(argv[1]);
  int last=atoi(argv[2]);
  ofstream output(argv[4],ios::out);
  cout<<"seed = "<<seed<<endl;
  global_generator.seed(seed);
  for(int i=0;i<200;i++){
    h[i].seed_generator(global_generator);
    h[i].get_params_from_file(argv[3],i);
  }
  round_robin(h,200,output,first,last);
  output.close();
  return 0;
}
