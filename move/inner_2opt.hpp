/*************************************************************************
 *  Name  : move.hpp
 *  Author: zmdsn
 *  Mail  : zmdsn@126.com 
 * Created Time: 2020年11月18日 星期三 13时47分27秒
 * 
 ************************************************************************/
#ifndef  __INNER_2OPT__HPP__
#define  __INNER_2OPT__HPP__
#include<iostream>
#include "move.hpp"
using namespace std;
class INNER_2OPT:public Move_base{
  // (x,y) <==>  (u,v) 
  public:
    Move_base* m;
    VType get_over_load(){
      DType temp_load = 0;
      over_load_changed = 0;
      over_load = 0;
      feasible = 1;
      R1->get_loads();
      if (R1->loads>problem->Capacity) {
        feasible = 0;
        over_load = R1->loads-problem->Capacity;
      }
      return over_load;
    }
    // int u,v;
    // int x,y,xp,yp;
    int min_pos;
    int max_pos;
    // u x..y v
    inline bool init_move() {
      if (!is_prepared) prepare();
      min_pos = min(position1,position2);
      max_pos = min(position1,position2);
      u = R1->route[min_pos-1];
      x = R1->route[min_pos];
      y = R1->route[max_pos];
      v = R1->route[max_pos+1];
      xp = problem->get_invert(x);
      yp = problem->get_invert(y);
      is_legal= is_legal_move();
      //cout<<position1<<" ti "<<task_i<<endl;
      // return is_feasible();
      return is_legal;
    }
    inline bool is_legal_move() {
      if(position1==0||position2==0) {
        return 0;
      } 
      if(position1+1>=R1->route.size()||
          position2+1>=R2->route.size()) {
        return 0;
      } 
      return 1;
    }
    VType reverse_costs(){
      VType cost;
      cost  = (*problem)(u,yp);
      cost += (*problem)(xp,v);
      // cout<<u<<x<<y<<v<<endl;
      // cout<<cost<<endl;
      cost -= (*problem)(u,x);
      cost -= (*problem)(y,v);
      // cout<<cost<<endl;
      return cost;
    }
    VType get_move_costs(){
      // cout<<"inner_2opt"<<endl;
      costs = 0;
      if(!is_legal_move())  return 0;
      costs = reverse_costs();
      // cout<<costs<<endl;
      return costs;
    }
    bool move(){
      min_pos = min(position1,position2);
      max_pos = min(position1,position2);
      if (!is_legal_move())  return 0;
      // cout<<max_pos<<min_pos<<"ss\n";
      reverse_tasks(R1->route,min_pos,max_pos-min_pos);
      return 1;
    }
    INNER_2OPT(){}
    INNER_2OPT(CARP_INDIVIDUAL *_R1,CARP_INDIVIDUAL *_R2,
        int _position1, int _position2):
      Move_base(R1,R2,position1,position2){}
    INNER_2OPT(CARP_INDIVIDUAL *_R1,CARP_INDIVIDUAL *_R2):
      Move_base(R1,R2){}
};
#endif
