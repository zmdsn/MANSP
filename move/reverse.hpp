/*************************************************************************
 *  Name  : move.hpp
 *  Author: zmdsn
 *  Mail  : zmdsn@126.com 
 * Created Time: 2020年11月18日 星期三 13时47分27秒
 * 
 ************************************************************************/
#ifndef  __REVERSE__HPP__
#define  __REVERSE__HPP__
#include<iostream>
#include "move.hpp"
using namespace std;
class REVERSE:public Move_base{
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
    inline bool init_move() {
      if (!is_prepared) prepare();
      task_ip = problem->get_invert(R1->route[position1]);
      is_legal = is_legal_move();
    }
    // int u,v;
    // int x,y,xp,yp;
    int min_pos;
    int max_pos;
    // u x..y v
    bool is_legal_move(){
      is_legal = !(position1==0||position1+1==R1->route.size());
      return is_legal;
    }
    bool is_feasible(){
      feasible = 1;
      if(!is_legal)  return 0;
      return get_over_load()<=0;
    }
    VType get_move_costs(){
      // cout<<"inner_2opt"<<endl;
      costs = 0;
      if(!is_legal)  return 0;

      costs = get_replace_costs(R1,position1,task_ip);
      // cout<<costs<<endl;
      return costs;
    }
    bool move(){
      if (!is_legal)  return 0;
      R1->route[position1] = task_ip;
      return 1;
    }
    REVERSE(){}
    REVERSE(CARP_INDIVIDUAL *_R1,CARP_INDIVIDUAL *_R2,
        int _position1, int _position2):
      Move_base(R1,R2,position1,position2){}
    REVERSE(CARP_INDIVIDUAL *_R1,CARP_INDIVIDUAL *_R2):
      Move_base(R1,R2){}
};
#endif
