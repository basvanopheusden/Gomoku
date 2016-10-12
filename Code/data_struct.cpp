#include "board.h"
#include "data_struct.h"

#include <algorithm>
#include <fstream>
using namespace std;

void data_struct::add(board b, uint64 m, bool player, double t=0.0, unsigned int p=0){
  alltrials.emplace_back(b,m,player,t,p);
  if(p>=Nplayers)
    Nplayers=p+1;
  Nboards++;
}

void data_struct::add_2afc(board b, uint64 m, uint64 m1, uint64 m2, bool player, int v1, int v2, double t=0.0, unsigned int p=0){
  alltrials_2afc.emplace_back(b,m,m1,m2,player,v1,v2,t,p);
  if(p>=Nplayers)
    Nplayers=p+1;
  Nboards_2afc++;
}

void data_struct::add_eval(board b, int score, int opt_score, bool player, double t=0.0, unsigned int p=0){
  alltrials_eval.emplace_back(b,score,opt_score,player,t,p);
  if(p>=Nplayers)
    Nplayers=p+1;
  Nboards_eval++;
}

void data_struct::execute_command_gianni_2afc(vector<string> c){
  string subject=c[0],color=c[1],bp=c[2],wp=c[3],tile=c[4];
  board b(binstringtouint64(bp),binstringtouint64(wp));
  bool player=(color=="0")?BLACK:WHITE;
  uint64 m=tilestringtouint64(tile);
  uint64 m1=tilestringtouint64(c[6]);
  uint64 m2=tilestringtouint64(c[7]);
  int v1=atoi(c[8].c_str());
  int v2=atoi(c[9].c_str());
  double t=atof(c[5].c_str());
  unsigned int player_id=atoi(subject.c_str());
  if(c.size()>12)
    player_id=3*player_id+atoi(c[12].c_str())/2;
  add_2afc(b,m,m1,m2,player,v1,v2,t,player_id);
}

void data_struct::execute_command_gianni_eval(vector<string> c){
  string subject=c[0],color=c[1],bp=c[2],wp=c[3],tile=c[4];
  board b(binstringtouint64(bp),binstringtouint64(wp));
  int score=atoi(tile.c_str());
  bool player=((color=="0")?BLACK:WHITE);
  int opt_score=atoi(c[6].c_str());
  double t=atof(c[5].c_str());
  unsigned int player_id=atoi(subject.c_str());
  if(c.size()>9)
    player_id=3*player_id+atoi(c[9].c_str())/2;
  add_eval(b,score,opt_score,player,t,player_id);
}

void data_struct::execute_command_gianni(vector<string> c){
  string subject=c[0],color=c[1],bp=c[2],wp=c[3],tile=c[4];
  uint64 m=tilestringtouint64(tile);
  bool player=(color=="0")?BLACK:WHITE;
  board b(binstringtouint64(bp),binstringtouint64(wp));
  double t=atof(c[5].c_str());
  unsigned int player_id=atoi(subject.c_str());
  if(c.size()>8)
    player_id=5*player_id+atoi(c[8].c_str());
  add(b,m,player,t,player_id);
}

void data_struct::load_file_gianni(char* filename, int trial_type=0){
  ifstream input(filename,ios::in);
  string line;
  vector<string> word;
  int index;
  if(!input.is_open())
    cout<<"file not found"<<endl;
  while(!input.eof()){
    getline(input,line);
    index=0;
    while(index!=-1){
      index=line.find(',');
      word.push_back(line.substr(0,index));
      line=line.substr(index+1);
    }
    if(!word.empty()){
      if(trial_type==AFC2)
        execute_command_gianni_2afc(word);
      else if(trial_type==EVAL)
        execute_command_gianni_eval(word);
      else execute_command_gianni(word);
    }
    word.clear();
  }
}

void data_struct::clear(){
  alltrials.clear();
  alltrials_2afc.clear();
  alltrials_eval.clear();
  Nboards=Nboards_2afc=Nboards_eval=0;
  Nplayers=0;
}

double data_struct::compute_chance(int subject,int group){
  double L=0.0;
  vector<unsigned int> boards=select_boards(subject,group);
  for(unsigned int i=0;i<boards.size();i++)
    L+=log(4*BOARD_WIDTH-alltrials[i].b.num_pieces());
  return L/boards.size();
}

void data_struct::load_board_file(char* filename){
  ifstream input(filename,ios::in);
  uint64 pblack,pwhite,m;
  string s;
  while(!input.eof()){
    input>>s;
    input>>pblack>>pwhite;
    input>>m;
    add(board(pblack,pwhite),m,(s=="Black")?BLACK:WHITE);
  }
}

void data_struct::save_board_file(char* filename){
  ofstream output2(filename,ios::out);
  board b;
  uint64 m;
  bool player;
  for(unsigned int i=0;i<Nboards;i++){
    b=alltrials[i].b;
    m=alltrials[i].m;
    player=alltrials[i].player;
    output2<<(player==BLACK?"Black\t":"White\t")<<b.pieces[BLACK]<<"\t"<<b.pieces[WHITE]<<"\t"<<m<<endl;
  }
}

void data_struct::make_groups(int Ngroups, mt19937_64 engine){
  vector<int> board_by_player[Nplayers];
  unsigned int N,k;
  for(unsigned int j=0;j<Nboards;j++)
    board_by_player[alltrials[j].player_id].push_back(j);
  for(unsigned int p=0;p<Nplayers;p++){
    N=board_by_player[p].size();
    for(unsigned int i=0;i<N;i++){
      k=uniform_int_distribution<unsigned int> {i,N-1}(engine);
      alltrials[board_by_player[p][k]].group=(Ngroups*i)/N+1;
      board_by_player[p][k]=board_by_player[p][i];
    }
  }
  for(unsigned int p=0;p<Nplayers;p++)
    board_by_player[p].clear();
  for(unsigned int j=0;j<Nboards_2afc;j++)
    board_by_player[alltrials_2afc[j].player_id].push_back(j);
  for(unsigned int p=0;p<Nplayers;p++){
    N=board_by_player[p].size();
    for(unsigned int i=0;i<N;i++){
      k=uniform_int_distribution<unsigned int> {i,N-1}(engine);
      alltrials_2afc[board_by_player[p][k]].group=(Ngroups*i)/N+1;
      board_by_player[p][k]=board_by_player[p][i];
    }
  }
  for(unsigned int p=0;p<Nplayers;p++)
    board_by_player[p].clear();
  for(unsigned int j=0;j<Nboards_eval;j++)
    board_by_player[alltrials_eval[j].player_id].push_back(j);
  for(unsigned int p=0;p<Nplayers;p++){
    N=board_by_player[p].size();
    for(unsigned int i=0;i<N;i++){
      k=uniform_int_distribution<unsigned int> {i,N-1}(engine);
      alltrials_eval[board_by_player[p][k]].group=(Ngroups*i)/N+1;
      board_by_player[p][k]=board_by_player[p][i];
    }
  }
}

vector<unsigned int> data_struct::select_boards(int subject, int group){
  return select_boards(subject,group,0);
}


vector<unsigned int> data_struct::select_boards(int subject, int group, int type){
  vector<unsigned int> boards;
  if(type==AFC2){
    for(unsigned int i=0;i<Nboards_2afc;i++)
      if((subject==-1 || alltrials_2afc[i].player_id==subject) && (group==0 || alltrials_2afc[i].group==group || (group<0 && alltrials_2afc[i].group!=-group)))
        boards.push_back(i);
  }
  else if(type==EVAL){
    for(unsigned int i=0;i<Nboards_eval;i++)
      if((subject==-1 || alltrials_eval[i].player_id==subject) && (group==0 || alltrials_eval[i].group==group || (group<0 && alltrials_eval[i].group!=-group)))
        boards.push_back(i);
  }
  else for(unsigned int i=0;i<Nboards;i++)
    if((subject==-1 || alltrials[i].player_id==subject) && (group==0 || alltrials[i].group==group || (group<0 && alltrials[i].group!=-group)))
      boards.push_back(i);
  return boards;
}

/*void data_struct::print(bool pause=false){
  for(unsigned int i=0;i<Nboards;i++){
    allboards[i].write(allmoves[i]);
    if(pause)
      cin.get();
  }
}*/

/*void data_struct::execute_command_zeyan(string c[5],bool talk=false){
  zet m;
  static bool savemode=false;
  static uint64 t=0;
  static int filenum=0;
  static int gamenum=0;
  static board b;
  int player;
  uint64 tcurr=atoll(c[0].c_str());
  if(c[1]=="start"){
    filenum++;
    if(talk)
       cout<<c[1]<<endl;
  }
  else if (c[1]=="time out" ||
     c[1]=="switch to computer to attack" || c[1]=="show win over" ||
     c[1]=="load new board" || c[1]=="start mode" || c[1]=="game end win" ||
     c[1]=="request draw" || c[1]=="game end draw"){
    if(talk)
       cout<<c[1]<<endl;
    return;
  }
  else if(c[1]=="start exercise"){
    if(talk)
      cout<<c[1]<<endl;
    savemode=false;
  }
  else if(c[1]=="start mode 3"){
    if(talk)
       cout<<c[1]<<endl;
    savemode=true;
  }
  else if(c[1]== "player resets board"){
    b.reset();
    if(talk)
      b.write();
  }
  else if(c[1]=="new hvh"){
    b.reset();
    gamenum++;
    if(talk)
      b.write();
  }
  else if(c[1]=="reset by board"){
    b.reset();
    b.add(c[3],BLACK);
    b.add(c[4],WHITE);
    if(talk)
      b.write();
  }
  else if(c[1]=="0" || c[1]=="1"){
    m=zet(stringstouint64(c[3],c[2]),0.0,c[1]=="0"?BLACK:WHITE);
    player=2*filenum+(c[1]=="0"?0:1);
    if(savemode)
      add(b,m,(tcurr-t)*1000,player);
    b.add(m);
    if(talk)
      b.write();
  }
  else cout<<"Unknown command:\t"<<c[1]<<endl;
  t=tcurr;
}

void data_struct::add_names_zeyan(char* filename){
  string s(filename);
  unsigned int i=s.find('-');
  unsigned int j=s.find('-',i+1);
  unsigned int k=s.find('-',j+1);
  if(k==s.length()){
    player_name.push_back(s.substr(i+1,j-1));
    Nplayers++;
  }
  else {
    player_name.push_back(s.substr(i+1,j-1));
    player_name.push_back(s.substr(j+1,k-1));
    Nplayers+=2;
  }
}

void data_struct::load_file_zeyan(char* filename){
  ifstream input(filename,ios::in);
  add_names_zeyan(filename);
  string line;
  string word[5];
  int index;
  int i;
  while(!input.eof()){
    getline(input,line);
    index=0;
    i=0;
    while(index!=-1){
      index=line.find(',');
      word[i]=line.substr(0,index);
      line=line.substr(index+1);
      i++;
    }
    execute_command_zeyan(word,false);
  }
}


void data_struct::add_names_gianni(char* filename){
  string s(filename);
  unsigned int namestart=s.find_last_of("/\\");
  unsigned int i=s.find('_',namestart+1);
  unsigned int j=s.find('_',i+1);
  if(j==s.length()){
    player_name.push_back(s.substr(namestart+1,i-namestart-1));
    Nplayers++;
  }
  else {
    player_name.push_back(s.substr(namestart+1,i-namestart-1));
    player_name.push_back(s.substr(i+1,j-i-1));
    Nplayers+=2;
  }
}*/

void data_struct::write_to_header(char* filename){
  ofstream headout(filename,ios::out);
  headout<<"#include \"board.h\"\n"
  <<"#include \"data_struct.h\"\n\n"
  <<"#ifdef NO_DATA\n"
  <<"data_struct::data_struct(): Nplayers(0), Nboards(0), Nboards_2afc(0), Nboards_eval(0){}\n"
  <<"#else\n"
  <<"data_struct::data_struct(): Nplayers("<<Nplayers<<"), Nboards("<<Nboards<<"), Nboards_2afc("
  <<Nboards_2afc<<"), Nboards_eval("<<Nboards_eval<<"),\nalltrials{";
  for(unsigned int i=0;i<Nboards;i++)
    headout<<hex<<"{{0x"<<alltrials[i].b.pieces[BLACK]<<"ULL,0x"<<alltrials[i].b.pieces[WHITE]<<"ULL},0x"
    <<alltrials[i].m<<"ULL,"<<((alltrials[i].player==BLACK)?"BLACK":"WHITE")<<","
    <<dec<<alltrials[i].thinking_time<<","<<alltrials[i].player_id<<","<<alltrials[i].group<<"}"
    <<((i<Nboards-1)?",\n":"},\nalltrials_2afc{");
  for(unsigned int i=0;i<Nboards_2afc;i++)
    headout<<hex<<"{{0x"<<alltrials_2afc[i].b.pieces[BLACK]<<"ULL,0x"<<alltrials_2afc[i].b.pieces[WHITE]<<"ULL},0x"
    <<alltrials_2afc[i].m<<"ULL,0x"<<alltrials_2afc[i].m1<<"ULL,0x"<<alltrials_2afc[i].m2<<"ULL,"
    <<((alltrials_2afc[i].player==BLACK)?"BLACK":"WHITE")<<","<<dec<<alltrials_2afc[i].v1<<","<<alltrials_2afc[i].v2<<","
    <<alltrials_2afc[i].thinking_time<<","<<alltrials_2afc[i].player_id<<","<<alltrials_2afc[i].group<<"}"
    <<((i<Nboards_2afc-1)?",\n":"},\nalltrials_eval{");
  for(unsigned int i=0;i<Nboards_eval;i++)
    headout<<hex<<"{{0x"<<alltrials_eval[i].b.pieces[BLACK]<<"ULL,0x"<<alltrials_eval[i].b.pieces[WHITE]<<"ULL},"
    <<dec<<alltrials_eval[i].score<<","<<alltrials_eval[i].opt_score<<","<<((alltrials_eval[i].player==BLACK)?"BLACK":"WHITE")<<","
    <<alltrials_eval[i].thinking_time<<","<<alltrials_eval[i].player_id<<","<<alltrials_eval[i].group<<"}"
    <<((i<Nboards_eval-1)?",\n":"}{}\n");
  headout<<"#endif\n";
  headout.close();
}

void data_struct::write_to_text(char* filename){
  ofstream output(filename,ios::out);
  for(unsigned int i=0;i<Nboards;i++)
    output<<alltrials[i].player_id<<"\t"<<alltrials[i].player<<"\t"<<hex<<alltrials[i].b.pieces[BLACK]
          <<"\t"<<alltrials[i].b.pieces[WHITE]<<alltrials[i].m<<"\t"<<dec<<alltrials[i].thinking_time<<"\t"<<alltrials[i].group<<endl;
  output.close();
}

void data_struct::load_from_text(char* filename){
  ifstream input(filename,ios::in);
  board b;
  unsigned int player_id;
  bool player;
  double thinking_time;
  int group;
  uint64 m;
  while(!input.eof()){
    input>>player_id>>player>>hex>>b.pieces[BLACK]>>b.pieces[WHITE]>>m>>thinking_time>>group;
    add(b,m,player,thinking_time,player_id);
    alltrials[Nboards-1].group=group;
  }
  input.close();
}
