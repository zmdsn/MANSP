/*************************************************************************
 *  Name  : move.hpp
 *  Author: zmdsn
 *  Mail  : zmdsn@126.com 
 * Created Time: 2020年11月18日 星期三 13时47分27秒
 * 
 ************************************************************************/
#ifndef  __Relocate__HPP__
#define  __Relocate__HPP__
#include<iostream>
#include "../move/move_base.hpp"
// 
using namespace std;
class Relocate:public Move_base{
  // (x,y) <==>  (u,v) 
 public:
  Move_base* m;
 /* inline bool prepare() {
    R1->get_loads();
    R2->get_loads();
    R1->get_over_load();
    R2->get_over_load();
    R1->get_costs();
    R2->get_costs();
    is_prepared = 1;
  }*/
  inline bool init_move() {
    if (!is_prepared) prepare();
    old_over_load = get_old_over_load();
    task_i  = R1->route[position1];
    loadi   = problem->get_demand(task_i);
    task_ip = problem->get_invert(task_i);
    inv_task_i = 0;
    is_legal= is_legal_move();
    //cout<<position1<<" ti "<<task_i<<endl;
    // return is_feasible();
    return is_legal;
  }
  inline DType get_over_load() {
    DType temp_load = 0;
    over_load_changed = 0;
    temp_load = R1->loads - loadi; 
    over_load = problem->get_over_load(temp_load);
    temp_load = R2->loads+loadi; 
    // cout<<temp_load<<loadi;
    over_load += problem->get_over_load(temp_load);
    over_load_changed = over_load - old_over_load;
    // cout<<"ov"<<R1->loads<<R2->loads<<over_load<<endl;
    return over_load;
  }
  inline bool p2_not_be_zero() {
    return position2!=0;
  }
  inline bool not_be_special() {
    return (R1!=R2||!(R1==R2&&(position1==position2||position1==position2-1)));
  }
  inline bool is_legal_move() {
    is_legal = task_i!=0&&not_be_special()&&p2_not_be_zero();
    // cout<<"is"<<(task_i!=0)<<not_be_special()<<p2_not_be_zero() <<endl;
    return is_legal;
  }
  inline VType relocate_costs() {
    VType cost;
    cost = get_delete_costs(R1,position1);
    // cout<<"*"<<cost<<endl;
    cost+= get_insert_costs(R2,position2,task_i);
    // cout<<cost<<endl;
    return cost;
  }
  inline VType relocate_costs_inv() {
    VType cost;
    cost = get_delete_costs(R1,position1);
    cost+= get_insert_costs(R2,position2,task_ip);
    return cost;
  }
  VType get_move_costs() {
    // cout<<"relocate"<<endl;
    costs = 0;
    if(!is_legal)return 0;
    VType cost1 = relocate_costs();
    // cout<<cost1<<endl;
    VType cost2 = relocate_costs_inv();
    // cout<<cost2<<endl;
    if(cost2 < cost1) {
      inv_task_i = 1;
      costs = cost2;
    } else {
      inv_task_i = 0;
      costs = cost1;
    }
    return costs;
  }
  bool relocate_same_route_move() {
    int e = inv_task_i?task_ip:task_i;
    //cout<<"m "<<e<<position1<<" "<<position2<<endl;
    if(position1>=position2) {
      delete_task(R1->route,position1);
      insert_task(R2->route,position2,e);
    } else{
      insert_task(R2->route,position2,e);
    //  show(R2->route);
      delete_task(R1->route,position1);
      //show(R1->route);
    }
    return 1;
  }
  bool move() {
    if(!is_legal)  return 0;
    // cout<<"moved***********";
    if(R1==R2) {
      relocate_same_route_move();
    } else {
      // delete_task();
      // insert_task();
      // R1->route.delete_task(position1);
      delete_task(R1->route,position1);
      // cout<<inv_task_i<<endl;
      int e = inv_task_i?task_ip:task_i;
      insert_task(R2->route,position2,e);
      R1->loads += loadj-loadi;
      R2->loads += loadi-loadj;
    }
    VType cost = problem->get_cost(task_i);
    // R1->costs -= costs;
    // R2->costs += costs;
    R1->get_fitness();
    R2->get_fitness();
    // cout<<over_load<<"&*^"<<endl;
    // exit(0);
    // cout<<inv_task_i<<"moved\n";
    return 1;
  }
  Relocate() {}
  Relocate(CARP_INDIVIDUAL *_R1,CARP_INDIVIDUAL *_R2,
      int _position1, int _position2):
    Move_base(_R1,_R2,_position1,_position2) {}
  Relocate(CARP_INDIVIDUAL *_R1,CARP_INDIVIDUAL *_R2):Move_base(_R1,_R2) {}
};
#endif
