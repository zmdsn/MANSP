/*************************************************************************
 *  Name  : move.hpp
 *  Author: zmdsn
 *  Mail  : zmdsn@126.com 
 * Created Time: 2020年11月18日 星期三 13时47分27秒
 * 
 ************************************************************************/
#ifndef  __RELOCATE__HPP__
#define  __RELOCATE__HPP__
#include<iostream>
#include "../move/move.hpp"
using namespace std;
class RELOCATE:public MOVE{
// (x,y) <==>  (u,v) 
 public:
  MOVE* m;
  DType get_over_load(){
    DType temp_load = 0,old_over_load=0;
    over_load_changed = 0;
    over_load = 0;
    old_over_load = get_old_over_load();
    if (!is_legal_move()) {
      over_load = old_over_load;
      return over_load;
    }
    loadi = problem->get_demand(task_i);
    temp_load = R1->loads-loadi; 
    over_load += problem->get_over_load(temp_load);
    temp_load = R2->loads+loadi; 
    over_load += problem->get_over_load(temp_load);
    over_load_changed = over_load-old_over_load;
    return over_load;
  }
  // DType get_over_load(){
  //   DType temp_load = 0;
  //   over_load_changed = 0;
  //   over_load = 0;
  //   feasible = 1;
  //   if (R1==R2) {
  //     R1->get_loads();
  //     if (R1->loads>problem->Capacity) {
  //       feasible = 0;
  //       over_load = R1->loads-problem->Capacity;
  //     }
  //     return 0;
  //   }
  //   R1->get_loads();
  //   R2->get_loads();
  //   loadi = problem->get_demand(task_i);
  //   temp_load = R1->loads-loadi; 
  //   if (temp_load>problem->Capacity) {
  //     feasible = 0;
  //     over_load += temp_load-problem->Capacity;
  //   }
  //   temp_load = R2->loads+loadi; 
  //   if (R2->loads+loadi>problem->Capacity) {
  //     feasible = 0;
  //     over_load += temp_load-problem->Capacity;
  //   }
  //   over_load_changed = over_load-R1->over_load-R2->over_load;
  //   // cout<<over_load<<"^*^"<<endl;
  //   // exit(0);
  //   return over_load;
  // }
  bool p2_not_be_zero(){
    if(position2==0) {
      feasible = 0;
      over_load = R1->over_load+R2->over_load; 
      // cout<<"ov"<<over_load<<endl;
      costs = 0;
      return 0;
    } 
    return 1;
  }
  bool not_be_special(){
    if(R1==R2&&(position1==position2||position1==position2-1)) {
      feasible = 0;
      over_load = 0; 
      costs = 0;
      return 0;
    } 
    return 1;
  }
  bool is_legal_move(){
    if(!not_be_zero())     return 0;
    if(!not_be_special()) return 0;
    if(!p2_not_be_zero()) return 0;
    return 1;
  }
  bool is_feasible_different_route(){
    return get_over_load()<=0;
  }
  bool is_feasible(){
    feasible = 1;
    if(!is_legal_move())  return 0;
    // if(get_over_load()>0)  return 0;
    if(is_same_route())    return 1;
    return is_feasible_different_route();
  }
  VType relocate_costs(){
    VType cost;
    cost = get_delete_costs(R1,position1);
    // cout<<"*"<<cost<<endl;
    cost+= get_insert_costs(R2,position2,task_i);
    // cout<<cost<<endl;
    return cost;
  }
  VType relocate_costs_inv(){
    VType cost;
    cost = get_delete_costs(R1,position1);
    cost+= get_insert_costs(R2,position2,task_ip);
    return cost;
  }
  VType get_move_costs(){
    // cout<<"relocate"<<endl;
    costs = 0;
    if(!is_legal_move())  return 0;
    if(R1==R2&&(position1==position2||position1==position2-1)) {
      costs = 0;
      return costs;
    } 
    VType cost1 = relocate_costs();
    // cout<<cost1<<endl;
    VType cost2 = relocate_costs_inv();
    // cout<<cost2<<endl;
    if(cost2<cost1){
      inv_task_i = 1;
      costs = cost2;
    } else {
      inv_task_i = 0;
      costs = cost1;
    }
    return costs;
  }
  bool relocate_same_route_move(){
    if(position1>position2) {
      delete_task();
      insert_task();
    } else{
      insert_task();
      delete_task();
    }
    return 1;
  }
  bool move(){
    if(!is_legal_move())  return 0;
    // cout<<"moved***********";
    if(R1==R2) {
      relocate_same_route_move();
    } else {
      delete_task();
      insert_task();
      R1->loads += loadj-loadi;
      R2->loads += loadi-loadj;
    }
    R1->get_fitness();
    R2->get_fitness();
    // cout<<over_load<<"&*^"<<endl;
    // exit(0);
    return 1;
  }
  RELOCATE(){}
  RELOCATE(CARP_INDIVIDUAL *_R1,CARP_INDIVIDUAL *_R2,
      int _position1, int _position2):
    MOVE(R1,R2,position1,position2){}
  RELOCATE(CARP_INDIVIDUAL *_R1,CARP_INDIVIDUAL *_R2):MOVE(R1,R2){}
};
#endif
