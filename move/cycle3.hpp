/*************************************************************************
 *  Name  : move.hpp
 *  Author: zmdsn
 *  Mail  : zmdsn@126.com 
 * Created Time: 2020年11月18日 星期三 13时47分27秒
 * 
 ************************************************************************/
#ifndef  __3CYCLE__HPP__
#define  __3CYCLE__HPP__
#include<iostream>
#include "move.hpp"
using namespace std;
class CYCLE3:public MOVE{
  // (x,y) <==>  (u,v) 
  public:
    MOVE* m;
    int min_pos;
    int max_pos;
    // int position3;
    int e1,e2,e3;
    // CARP_INDIVIDUAL *R3;
    DType get_over_load(DType load){
      if (load>problem->Capacity) {
        return load-problem->Capacity;
      }
      return 0;
    }
    DType mark_over_load(DType load){
      if (load>problem->Capacity) {
        feasible = 0;
        over_load += get_over_load(load);
      }
      return over_load;
    }
    DType get_over_load(){
      DType temp_load = 0;
      over_load_changed = 0;
      over_load = 0;
      feasible = 1;
      get_ei(0);
      DType d1 = problem->get_demand(e1);
      DType d2 = problem->get_demand(e2);
      DType d3 = problem->get_demand(e3);
      R1->get_loads();
      R2->get_loads();
      R3->get_loads();
      mark_over_load(R1->loads+d3-d1);
      mark_over_load(R2->loads+d1-d2);
      mark_over_load(R3->loads+d2-d3);
      over_load_changed = over_load - R1->over_load-
        R2->over_load - R3->over_load;
      return over_load;
    }
    bool is_legal_move(){
      feasible = 0;
      over_load = 0; 
      if(position1==0||position2==0||position3==0) {
        return 0;
      } 
      if(R1==R2||R2==R3||R3==R1) {
        return 0;
      } 
      if(position1+1>=R1->route.size()||
          position2+1>=R2->route.size()||
          position3+1>=R3->route.size()) {
        return 0;
      } 
      return 1;
    }
    bool is_feasible(){
      feasible = 1;
      if(!is_legal_move())  return 0;
      get_over_load();
      if(over_load>0)  return 0;
      return 1;
    }
    void get_ei(int change_mode) {
      get_unit_mode(change_mode);
      e1 = unit_mode[1]?R3->route[position3]:
        problem->get_invert(R3->route[position3]);
      e2 = unit_mode[1]?R1->route[position1]:
        problem->get_invert(R1->route[position1]);
      e3 = unit_mode[1]?R2->route[position2]:
        problem->get_invert(R2->route[position2]);
    }
    VType get_move_costs(int change_mode) {
      VType cost = 0;
      get_ei(change_mode);
      cost  = get_replace_costs(R1,position1,e1);
      cost += get_replace_costs(R2,position2,e2);
      cost += get_replace_costs(R3,position3,e3);
      return cost;
    }
    VType get_move_costs(){
      // cout<<"3cycle"<<endl;
      VType cost = 0;
      costs = 0;
      if(!is_legal_move())  return 0;
      costs = INF;
      for (int i = 0; i < 8; ++i) {
        cost = get_move_costs(i);
        if(cost<costs){
          costs = cost;
          change_mode = i;
        }
      }
      // cout<<costs<<endl;
      return costs;
    }
    bool move(){
      if (!is_legal_move())  return 0;
      get_ei(change_mode);
      R1->route[position1] = e1;
      R2->route[position2] = e2;
      R3->route[position3] = e3;
      return 1;
    }
    CYCLE3(){}
    CYCLE3(CARP_INDIVIDUAL *_R1,CARP_INDIVIDUAL *_R2,CARP_INDIVIDUAL *_R3,
        int _position1, int _position2,int _position3):
      MOVE(R1,R2,R3,_position1,_position2,_position2){}
    CYCLE3(CARP_INDIVIDUAL *_R1,CARP_INDIVIDUAL *_R2):MOVE(R1,R2){}
};
#endif
