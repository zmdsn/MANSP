/*************************************************************************
 *  Name  : move.hpp
 *  Author: zmdsn
 *  Mail  : zmdsn@126.com 
 * Created Time: 2020年11月18日 星期三 13时47分27秒
 ************************************************************************/
#ifndef  __TwoBetween2OPTReverse__HPP__
#define  __TwoBetween2OPTReverse__HPP__
#include<iostream>
#include "../move/between_2opt_reverse.hpp"

using namespace std;
class TwoBetween2OPTReverse:public Between2OPTReverse{
  // (x,y) <==>  (u,v) 
  public:
    VType reverse_costs(int _change_mode) {
      VType cost=0;
      int a = _change_mode&1?up:u;
      int b = _change_mode&2?v:vp;
      int backb = _change_mode&2?vp:v;
      int c = _change_mode&4?x:xp;
      int prec = _change_mode&4?xp:x;
      int d = _change_mode&8?yp:y;
      cost  = (*problem)(a,c);
      cost += (*problem)(b,d);
      cost += pre_val[a];
      cost += pre_val[prec];
      cost += back_val[backb];
      cost += back_val[d];
      // cout<<a<<b<<c<<d<<endl;
      return cost;
    }
    VType get_move_costs(){
      // cout<<"between_2opt"<<endl;
      costs = 0;
      if(!is_legal)  return 0;
      VType cost=0;
      costs = INF;
      for (int i = 1; i <= 16; ++i) {
        cost = reverse_costs(i);
        // cout<<i<<" "<<cost<<endl;
        if(cost<costs) {
          costs = cost;
          // change_mode = i;
        }
      }
      costs -= R1->costs+R2->costs;
      // cout<<costs<<endl;
      return costs;
    }
    bool move(){
      if(!is_legal)  return 0;
      P1.clear();
      P2.clear();
      // show(R1->route);
      // show(R2->route);
      for (int i = 0; i <= position1; ++i) {
        P1.push_back(R1->route[i]);
      }
      for (int i = position2; i >=0; --i) {
        P1.push_back(problem->get_invert(R2->route[i]));
      }
      for (int i = R1->route.size()-1; i >=position1+1; --i) {
        P2.push_back(problem->get_invert(R1->route[i]));
      }
      for (int i = position2+1; i <R2->route.size(); ++i) {
        P2.push_back(R2->route[i]);
      }
      R1->set_route(P1);
      R2->set_route(P2);
      R1->get_best_in_sort(R1);
      R2->get_best_in_sort(R2);
      // show(R1->route);
      // show(R2->route);
      return 1;
    }
    TwoBetween2OPTReverse(){}
    TwoBetween2OPTReverse(CARP_INDIVIDUAL *_R1,CARP_INDIVIDUAL *_R2,
        int _position1, int _position2):
      Between2OPTReverse(R1,R2,position1,position2){init_move();}
    TwoBetween2OPTReverse(CARP_INDIVIDUAL *_R1,CARP_INDIVIDUAL *_R2):
      Between2OPTReverse(_R1,_R2){}
};

#endif
