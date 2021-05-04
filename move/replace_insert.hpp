/*************************************************************************
 *  Name  : move.hpp
 *  Author: zmdsn
 *  Mail  : zmdsn@126.com 
 * Created Time: 2020年11月18日 星期三 13时47分27秒
 * 
 ************************************************************************/
#ifndef  __REPLACE_INSERT__HPP__
#define  __REPLACE_INSERT__HPP__
#include<iostream>
#include "move.hpp"
using namespace std;
// class InsertPosition {
//   int task;
//   int position;
//   VType costs;
// }

class REPLACE_INSERT:public MOVE{
  // (x,y) <==>  (u,v) 
  public:
    MOVE* m;
    VType get_over_load(){
      DType temp_load = 0;
      over_load_changed = 0;
      over_load = 0;
      feasible = 1;
      if (R1==R2) {
        R1->get_loads();
        if (R1->loads>problem->Capacity) {
          feasible = 0;
          over_load = R1->loads-problem->Capacity;
        }
        return 0;
      }
      R1->get_loads();
      R2->get_loads();
      loadi = problem->get_demand(task_i);
      loadj = problem->get_demand(task_j);
      temp_load = R1->loads-loadi+loadj; 
      if (temp_load>problem->Capacity) {
        feasible = 0;
        over_load += temp_load-problem->Capacity;
      }
      temp_load = R2->loads+loadi-loadj; 
      if (temp_load>problem->Capacity) {
        feasible = 0;
        over_load += temp_load-problem->Capacity;
      }
      over_load_changed = over_load-R1->over_load-R2->over_load;
      return over_load;
    }
    bool tasks_not_be_zero(){
      if(task_i==0 ||task_j==0) {
        feasible = 0;
        over_load = 0; 
        costs = 0;
        return 0;
      } 
      return 1;
    }
    bool is_legal_move(){
      if(R1==R2) return 0;
      if(R1->route.size()<3||R2->route.size()<3) return 0;
      if(!tasks_not_be_zero()) return 0;
      return 1;
    }
    bool is_feasible_different_route(){
      return get_over_load()<=0;
    }
    bool is_feasible(){
      feasible = 1;
      if(!is_legal_move())  return 0;
      if(is_same_route())    return 1;
      if(get_over_load()>0)  return 0;
      return is_feasible_different_route();
    }
    VType get_best_insert_costs_notk(CARP_INDIVIDUAL* R,int k,int e) {
      VType cost=INF, best_cost=INF;
      int ep = problem->get_invert(e);
      if (R->route.size()==3) {
        best_cost = (*problem)(0,e) + (*problem)(e,0) + problem->get_cost(e);
        to_insert_task = e; 
        insert_position = R->route.size()-1;
        cost = (*problem)(0,ep) + (*problem)(ep,0) + problem->get_cost(ep);
        if (best_cost>cost) {
          best_cost = cost;
          to_insert_task = ep;
        }
        return best_cost;
      }
      for (int i = 1; i < R->route.size(); ++i) {
        if (i==k||i==k+1) continue;      // not k
        cost = get_insert_costs(R,i,e);
        if (best_cost>cost) {
          best_cost = cost;
          to_insert_task = e; 
          insert_position = i;
        }
        cost = get_insert_costs(R,i,ep);
        if (best_cost>cost) {
          best_cost = cost;
          to_insert_task = ep;
          insert_position = i;
        }
      }
      return best_cost;
    }
    VType get_best_insert_costs() {
      return get_best_insert_costs_notk(R1,position1,task_j);
    }
    VType get_move_costs(){
      // cout<<"replace insert"<<endl;
      if(!is_legal_move())  return 0;
      VType cost;
      costs = INF;
      for (int i=0; i<2; i++) {
        int task1 = i?task_ip:task_i;
        cost  = get_replace_costs(R2,position2,task1);
        if(cost<costs){
          costs = cost;
          inv_task_i = i;
        }
      }
      costs += get_best_insert_costs();
      costs += get_delete_costs(R1,position1);
      return costs;
    }
    bool move(){
      if(!is_legal_move())  return 0;
      int task1 = inv_task_i?task_ip:task_i;
      if (insert_position<position1) {
        delete_task (R1->route,position1);
        insert_task (R1->route,insert_position,to_insert_task);
      } else {
        insert_task (R1->route,insert_position,to_insert_task);
        delete_task (R1->route,position1);
      }
      replace_task(R2->route,position2,task1);
      R1->get_fitness();
      R2->get_fitness();
      return 1;
    }
    REPLACE_INSERT(){}
    REPLACE_INSERT(CARP_INDIVIDUAL *_R1,CARP_INDIVIDUAL *_R2,
        int _position1, int _position2):
      MOVE(R1,R2,position1,position2){}
    REPLACE_INSERT(CARP_INDIVIDUAL *_R1,CARP_INDIVIDUAL *_R2):MOVE(R1,R2){}
};
#endif
