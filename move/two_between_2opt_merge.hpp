/*************************************************************************
 *  Name  : move.hpp
 *  Author: zmdsn
 *  Mail  : zmdsn@126.com 
 * Created Time: 2020年11月18日 星期三 13时47分27秒
 * 
 ************************************************************************/
#ifndef  __TwoBetween2OPTMerge__HPP__
#define  __TwoBetween2OPTMerge__HPP__
#include<iostream>
#include "../move/between_2opt_merge.hpp"
using namespace std;

class TwoBetween2OPTMerge:public Between2OPTMerge{
  // (x,y) <==>  (u,v) 
  public:
    vector<int> P1;
    vector<int> P2;
    vector<int> load1; 
    vector<int> load2;
    VType merge_costs(int _change_mode) {
      VType cost=0;
      int a = _change_mode&1?up:u;
      int b = _change_mode&2?vp:v;
      int c = _change_mode&4?xp:x;
      int d = _change_mode&8?yp:y;
      cost  = (*problem)(a,d);
      cost += (*problem)(c,b);
      cost += pre_val[a];
      cost += back_val[b];
      cost += pre_val[c];
      cost += back_val[d];
      return cost;
    }
    VType get_move_costs(){
      // cout<<"between_2opt"<<endl;
      costs = 0;
      if(!is_legal)  return 0;
      // init_move();
      VType cost=0;
      costs = INF;
      // costs = merge_costs();
      for (int i = 1; i <= 16; ++i) {
        cost = merge_costs(i);
        // cout<<i<<" "<<cost<<endl;
        if(cost<costs) {
          costs = cost;
        }
      }
      costs -= R1->costs+R2->costs;
      // cout<<costs<<endl;
      return costs;
    }
    bool move(){
      if(!is_legal)  return 0;
      // show(R1->route);
      // show(R2->route);
      P1.clear();
      P2.clear();
      for (int i = 0; i <= position1; ++i) {
        P1.push_back(R1->route[i]);
      }
      for (int i = position2+1; i <R2->route.size(); ++i) {
        P1.push_back(R2->route[i]);
      }
      for (int i = 0; i <= position2; ++i) {
        P2.push_back(R2->route[i]);
      }
      for (int i = position1+1; i <R1->route.size(); ++i) {
        P2.push_back(R1->route[i]);
      }
      R1->set_route(P1);
      R2->set_route(P2);
      R1->get_best_in_sort(R1);
      R2->get_best_in_sort(R2);
      // show(R1->route);
      // show(R2->route);
      return 1;
    }
    TwoBetween2OPTMerge(){}
    TwoBetween2OPTMerge(CARP_INDIVIDUAL *_R1,CARP_INDIVIDUAL *_R2,
        int _position1, int _position2):
      Between2OPTMerge(R1,R2,position1,position2){init_move();}
    TwoBetween2OPTMerge(CARP_INDIVIDUAL *_R1,CARP_INDIVIDUAL *_R2):
      Between2OPTMerge(_R1,_R2){}
};
#endif
