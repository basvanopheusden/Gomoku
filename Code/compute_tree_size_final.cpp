#include "heuristic.h"
#include "data_struct.h"

#include <ctime>
#include <chrono>
using namespace std;

int main(int argc,char** argv){
  int n=0;
  int Nplayers = atoi(argv[2]);
  int Nsamples = atoi(argv[3]);
  heuristic h;
  data_struct dat;
  unsigned int seed=unsigned(time(0));
  mt19937_64 global_generator;
  global_generator.seed(seed);
  h.seed_generator(global_generator);
  for(unsigned int i=0;i<Nplayers;i++)
    for(unsigned int g=1;g<=5;g++){
      h.get_params_from_file(argv[1],i,g);
      for(unsigned int j=0;j<dat.Nboards;j++){
        if(dat.alltrials[j].b.num_pieces()>5 && dat.alltrials[j].b.num_pieces()<30){
          n=0;
          for(int k=0;k<Nsamples;k++){
            h.makemove_bfs(dat.alltrials[j].b,dat.alltrials[j].player);
            n+=h.iterations;
          }
          cout<<((double) n)/Nsamples<<"\n";
        }
      }
    }
  return 0;
}
