/*************************************************************************
 *  Name  : move.hpp
 *  Author: zmdsn
 *  Mail  : zmdsn@126.com 
 * Created Time: 2020年11月18日 星期三 13时47分27秒
 * 
 ************************************************************************/
#ifndef  __K_RELOCATE__HPP__
#define  __K_RELOCATE__HPP__
#include<iostream>
#include "../move/move.hpp"
using namespace std;
class K_RELOCATE:public Move_base{
// (x,y) <==>  (u,v) 
 public:
  Move_base* m;
  DType load_before_u,load_u_v,load_after_v;
  std::vector<int> tasks;
  std::vector<int> tasks_inv;
  inline bool init_move(){
    is_legal= is_legal_move();
    if (is_legal){
      if (!is_prepared) prepare();
      tasks.clear();
      tasks_inv.clear();
      before_u = R1->route[position1-1];
      after_v  = R1->route[position1+length1+1];
      u = R1->route[position1];
      v = R1->route[position1+length1];
      up = problem->get_invert(u);
      vp = problem->get_invert(v);
      for (int i = 0; i <= length1; ++i) {
        tasks.push_back(R1->route[position1+i]);
        tasks_inv.push_back(
          problem->get_invert(R1->route[position1+length1-i]));
      }
    } 
    return is_legal;
  }
  DType get_over_load_different_route() {
    DType temp_load = 0;
    over_load = 0;
    R1->get_loads();
    R2->get_loads();
    loadi = get_load_u_v();
    // cout<<"loadi"<<problem->Capacity<<R1->loads<<R2->loads<<loadi<<endl;
    temp_load = R1->loads-load_u_v; 
    if (temp_load>problem->Capacity) {
      feasible = 0;
      over_load += temp_load-problem->Capacity;
    }
    temp_load = R2->loads+load_u_v; 
    if (temp_load>problem->Capacity) {
      feasible = 0;
      over_load += temp_load-problem->Capacity;
    }
    return over_load;
  }
  DType get_over_load(){
    if(!is_legal)  return 0;
    over_load_changed = 0;
    over_load = 0;
    feasible = 1;
    DType old_over_load = get_old_over_load();
    if (R1==R2) { 
      over_load = old_over_load;
      return old_over_load;
    }
    over_load = get_over_load_different_route();
    over_load_changed = over_load - old_over_load;
    return over_load;
  }
  DType get_load_u_v(){
    load_u_v = 0;
    for (int i=position1; i<=position1+length1; i++) {
      load_u_v += problem->get_demand(R1->route[i]);
    }
    return load_u_v;
  }
  // inline bool p2_not_be_zero(){
  //   if(position1==0) { return 0; } 
  //   if(position1+length1+1>=R1->route.size()) { return 0; } 
  //   if(position2==0) { return 0; } 
  //   return 1;
  // }
  // bool not_be_special() {
  //   if(R1==R2&&(position2>=position1
  //     &&position2<=position1+length1+1)) {return 0; } 
  //   return 1;
  // }
  bool is_legal_move(){
    is_legal  = position1>0;
    is_legal &= position1+length1+1 < R1->route.size();
    // is_legal &= length1+1 <= R1->route.size();
    is_legal &= (R1==R2) && !(R1==R2&&(position2>=position1
      &&position2<=position1+length1+1));
    // is_legal &= p2_not_be_zero();
    return is_legal;
  }
  bool is_feasible_different_route(){
    return get_over_load()<=0;
  }
  bool is_feasible(){
    if(!is_legal)  {
      feasible = 0;
      over_load = get_old_over_load();
      over_load_changed = 0;
      // cout<<"ov"<<over_load<<" "<<over_load_changed<<endl;
      return 0;
    }
    feasible = 1;
    if(get_over_load()>0)  return 0;
    return is_feasible_different_route();
  }
  VType relocate_costs(){
    VType cost;
    cost = get_delete_k_costs(R1,position1,length1+1);
    cost+= get_insert_k_costs(R2,position2,tasks);
    return cost;
  }
  VType relocate_costs_inv(){
    VType cost;
    cost = get_delete_k_costs(R1,position1,length1+1);
    cost+= get_insert_k_costs(R2,position2,tasks_inv);
    return cost;
  }
  VType get_move_costs(){
    costs = 0;
    over_load = 0;
    // init_move();
    if(!is_legal)  {return 0;}    
    get_over_load();
    VType cost1 = relocate_costs();
    VType cost2 = relocate_costs_inv();
    if(cost2<cost1){
      inv_task_i = 1;
      costs = cost2;
    } else {
      inv_task_i = 0;
      costs = cost1;
    }
    return costs;
  }
  bool relocate_same_route_move(std::vector<int>& tasks){
    if(position1>position2) {
      delete_k_task(R1->route,position1,length1+1);
      insert_k_task(R1->route,position2,tasks);
    } else{
      insert_k_task(R1->route,position2,tasks);
      delete_k_task(R1->route,position1,length1+1);
    }
    return 1;
  }
  bool move(){
    // cout<<"*************&*^"<<inv_task_i<<position2<<position1<<length1<<endl;
    // show(R1->route);
    // show(R2->route);
    if(!is_legal)  return 0;
    if(R1==R2) {
    // cout<<"moved***********";
      if (inv_task_i) {
        relocate_same_route_move(tasks_inv);
      } else {
        relocate_same_route_move(tasks);
      }
    } else {
      delete_k_task(R1->route,position1,length1+1);
      if (inv_task_i) {
        insert_k_task(R2->route,position2,tasks_inv);
      } else {
        insert_k_task(R2->route,position2,tasks);
      }
      R1->loads -= load_u_v;
      R2->loads += load_u_v;
    }
    R1->get_fitness();
    R2->get_fitness();
    return 1;
  }
  K_RELOCATE(){
    length1 = 1;
    // length1 for two tasks 
  }
  K_RELOCATE(CARP_INDIVIDUAL *_R1,CARP_INDIVIDUAL *_R2,
    int _position1, int _position2,int _length1):
    Move_base(_R1,_R2,_position1,_position2,_length1,0){
  }
  K_RELOCATE(CARP_INDIVIDUAL *_R1,CARP_INDIVIDUAL *_R2,
      int _position1, int _position2):
    Move_base(R1,R2,position1,position2){}
  K_RELOCATE(CARP_INDIVIDUAL *_R1,CARP_INDIVIDUAL *_R2):Move_base(R1,R2){}
};
#endif
