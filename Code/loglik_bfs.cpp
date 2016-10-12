#include "heuristic.h"
#include "board_list.h"
#include "data_struct.h"

#include <ctime>
#include <chrono>
#include <thread>
#include <mutex>
using namespace std;

mutex board_list_mutex;

void worker_thread(heuristic h,data_struct* dat,todolist* board_list){
  int i=-1;
  bool success=false;
  uint64 m;
  board_list_mutex.lock();
  while(board_list->get_next(i,success)){
    board_list_mutex.unlock();
    m=h.makemove_bfs(dat->alltrials[i].b,dat->alltrials[i].player).zet_id;
    success=(m==dat->alltrials[i].m);
    board_list_mutex.lock();
  }
  board_list_mutex.unlock();
}

void worker_thread_super(superheuristic s,data_struct* dat,todolist* board_list){
  int i=-1;
  bool success=false;
  uint64 m;
  board_list_mutex.lock();
  while(board_list->get_next(i,success)){
    board_list_mutex.unlock();
    m=s.makemove_bfs(dat->alltrials[i].b,dat->alltrials[i].player).zet_id;
    success=(m==dat->alltrials[i].m);
    board_list_mutex.lock();
  }
  board_list_mutex.unlock();
}

double compute_loglik_threads(heuristic& h,data_struct* dat,todolist* board_list){
  thread t[NTHREADS];
  for(int i=0;i<NTHREADS;i++)
    t[i]=thread(worker_thread,h,dat,board_list);
  for(int i=0;i<NTHREADS;i++)
    t[i].join();
  return board_list->get_Ltot();
}

double compute_loglik_threads_super(superheuristic& s,data_struct* dat,todolist* board_list){
  thread t[NTHREADS];
  for(int i=0;i<NTHREADS;i++)
    t[i]=thread(worker_thread_super,s,dat,board_list);
  for(int i=0;i<NTHREADS;i++)
    t[i].join();
  return board_list->get_Ltot();
}

double compute_loglik(heuristic& h, data_struct& dat, bool talk, int subject,
                      int group, char* times_file, char* output_file){
  todolist* board_list;
  double res;
  if(subject==-1){
    if(times_file==NULL)
      board_list=new todolist(dat.Nboards);
    else board_list=new todolist(dat.Nboards,times_file);
  }
  else{
    if(times_file==NULL)
      board_list=new todolist(dat.select_boards(subject,group));
    else board_list=new todolist(dat.select_boards(subject,group),times_file);
  }
  if(talk){
    if(output_file==NULL)
      board_list->set_cout();
    else board_list->set_output(output_file);
  }
  res=compute_loglik_threads(h,&dat,board_list);
  delete board_list;
  return res;
}

double compute_entropy(superheuristic& s,data_struct& dat,char* filename){
  todolist board_list(dat.Nboards);
  board_list.set_output(filename);
  for(unsigned int i=0;i<dat.Nboards;i++)
    dat.alltrials[i].m=s.makemove_bfs(dat.alltrials[i].b,dat.alltrials[i].player).zet_id;
  return compute_loglik_threads_super(s,&dat,&board_list);
}

double compute_entropy(heuristic& h,data_struct& dat,char* filename){
  todolist board_list(dat.Nboards);
  board_list.set_output(filename);
  for(unsigned int i=0;i<dat.Nboards;i++)
    dat.alltrials[i].m=h.makemove_bfs(dat.alltrials[i].b,dat.alltrials[i].player).zet_id;
  return compute_loglik_threads(h,&dat,&board_list);
}

double compute_entropy(heuristic& h,data_struct& dat){
  todolist board_list(dat.Nboards);
  for(unsigned int i=0;i<dat.Nboards;i++)
    dat.alltrials[i].m=h.makemove_bfs(dat.alltrials[i].b,dat.alltrials[i].player).zet_id;
  return compute_loglik_threads(h,&dat,&board_list);
}


double compute_crossentropy(heuristic& h1,heuristic& h2,data_struct& dat,char* filename){
  todolist board_list(dat.Nboards);
  board_list.set_output(filename);
  for(unsigned int i=0;i<dat.Nboards;i++)
    dat.alltrials[i].m=h1.makemove_bfs(dat.alltrials[i].b,dat.alltrials[i].player).zet_id;
  return compute_loglik_threads(h2,&dat,&board_list);
}

int main(int argc, char* argv[]){
  data_struct dat;
  heuristic h;
  unsigned int seed=unsigned(time(0));
  mt19937_64 global_generator;
  global_generator.seed(seed);
  h.seed_generator(global_generator);
  int player=atoi(argv[1]);
  int group=atoi(argv[2]);
  cout<<compute_loglik(h,dat,true,player,group,NULL,argv[3])<<endl;
  return 0;
}
