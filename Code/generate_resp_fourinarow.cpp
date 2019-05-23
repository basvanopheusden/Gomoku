#include "heuristic.h"
#include "mex.h"

#include <random>
#include <ctime>
#include <chrono>
using namespace std;

void mexFunction(int nlhs, mxArray *plhs[],int nrhs, const mxArray *prhs[]){
  heuristic h;
  random_device rd;
  mt19937_64 global_generator(rd());
  const mwSize *dims = mxGetDimensions(prhs[0]);
  const int N = dims[1];
  uint64 bp,wp;
  bool player;
  double* paramptr=mxGetPr(prhs[1]);
  h.seed_generator(global_generator);
  h.get_params_from_matlab(paramptr);
  plhs[0] = mxCreateNumericMatrix(1, N, mxINT32_CLASS, mxREAL);
  int* output = (int*) mxGetData(plhs[0]);
  uint64 m;
  for(int i=0;i<N;i++){
    bp = *mxGetUint64s(mxGetCell(prhs[0],i*3));
    wp = *mxGetUint64s(mxGetCell(prhs[0],i*3+1));
    player = *mxGetLogicals(mxGetCell(prhs[0],i*3+2));
    m = h.makemove_bfs(board(bp,wp),player).zet_id;
    output[i] = uint64totile(m);
  }
}
