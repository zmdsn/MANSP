/*************************************************************************
 *  Name  : move.hpp
 *  Author: zmdsn
 *  Mail  : zmdsn@126.com 
 * Created Time: 2020年11月18日 星期三 13时47分27秒
 * 
 ************************************************************************/
#ifndef  __SWAP_MOVE__HPP__
#define  __SWAP_MOVE__HPP__
#include<iostream>
#include "move.hpp"
using namespace std;
class SWAP_MOVE:public Move_base{
  // (x,y) <==>  (u,v) 
  public:
	  Move_base* m;
	  inline bool init_move() {
	    if (!is_prepared) prepare();
	    old_over_load = get_old_over_load();
	    task_i  = R1->route[position1];
	    loadi   = problem->get_demand(task_i);
	    task_ip = problem->get_invert(task_i);
	    inv_task_i = 0;
	    task_j  = R2->route[position2];
	    loadj   = problem->get_demand(task_j);
	    task_jp = problem->get_invert(task_j);
	    inv_task_j = 0;
	    is_legal= is_legal_move();
	    //cout<<position1<<" ti "<<task_i<<endl;
	    // return is_feasible();
	    return is_legal;
	  }
	  inline DType get_over_load() {
	    DType temp_load = 0;
	    over_load_changed = 0;
	    temp_load = R1->loads - loadi + loadj; 
	    over_load = problem->get_over_load(temp_load);
	    temp_load = R2->loads + loadi - loadj; 
	    // cout<<temp_load<<loadi;
	    over_load += problem->get_over_load(temp_load);
	    over_load_changed = over_load - old_over_load;
	    // cout<<"ov"<<R1->loads<<R2->loads<<over_load<<endl;
	    return over_load;
	  }
   /* MOVE* m;
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
    }*/
    inline bool tasks_not_be_zero(){
	//cout<<task_i<<","<<task_j<<endl;
      return task_i!=0 && task_j!=0;
    }
    inline bool is_legal_move(){
      return tasks_not_be_zero();
    }
    inline bool is_feasible_different_route(){
      return get_over_load()<=0;
    }
    inline bool is_feasible(){
      feasible = 1;
      if(!is_legal)  return 0;
      // if(is_same_route())    return 1;
      if(get_over_load()>0)  return 0;
      return is_feasible_different_route();
    }
    VType get_swap_costs_adjacent(CARP_INDIVIDUAL*R,
        int position,int task1,int task2){
      VType cost=0;
      int p = position, q=p+1;
      int u = R1->route[p-1];
      int x = R1->route[p];
      int y = R1->route[q];
      int v = R1->route[q+1];
      // cout<<u<<x<<y<<v;
      cost -= (*problem)(u,x);
      cost -= (*problem)(x,y);
      cost -= (*problem)(y,v);
      // cout<<cost<<" ";
      cost += (*problem)(u,task1);
      cost += (*problem)(task1,task2);
      cost += (*problem)(task2,v);
      // cout<<cost<<" ";
      return cost;
    }
    VType get_swap_costs_same(int _change_mode){
      // cout<<"same"<<_change_mode;
      if(_change_mode==0) return 0;
      if(_change_mode>1) return INF;
      return get_invert_costs();
    }
    VType get_swap_costs(int _change_mode){
      get_unit_mode(_change_mode);
      int task1 = unit_mode[0]?task_ip:task_i;
      int task2 = unit_mode[1]?task_jp:task_j;
      // show(unit_mode);
      if(R1==R2&&position2==position1+1) {
        return get_swap_costs_adjacent(R1,position1,task2,task1);
      }
      if(R1==R2&&position2==position1-1) {
        return get_swap_costs_adjacent(R1,position2,task1,task2);
      }
      if(R1==R2&&position2==position1) {
        return get_swap_costs_same(_change_mode);
      }
      VType cost = 0;
      if(R1!=R2||(R1==R2)&&position2!=position1) {
        cost  = get_replace_costs(R2,position2,task1);
				//cout<<cost<<"ss";
        cost += get_replace_costs(R1,position1,task2);
				//cout<<cost<<endl;
      }
      return cost;
    }
    VType get_move_costs(){
      // cout<<"swap"<<endl;
      VType cost;
      costs = 0;
      if(!is_legal)  return 0;
      costs = INF;
      for (int i=0; i<4; i++) {
        cost = get_swap_costs(i);
        if(cost<costs){
          costs = cost;
          change_mode = i;
          change_mode_to_task_inv(change_mode);
        }
      }
      return costs;
    }
    bool move(){
      if(!is_legal)  return 0;
      if(R1==R2&&position1==position2) {
        invert_task();
        return 1;
      }
      int task1 = inv_task_i?task_ip:task_i;
      int task2 = inv_task_j?task_jp:task_j;
      replace_task(R2->route,position2,task1);
      replace_task(R1->route,position1,task2);
      //R1->get_fitness();
      //R2->get_fitness();
      return 1;
    }
    SWAP_MOVE(){}
    SWAP_MOVE(CARP_INDIVIDUAL *_R1,CARP_INDIVIDUAL *_R2,
        int _position1, int _position2):
      Move_base(R1,R2,position1,position2){}
    SWAP_MOVE(CARP_INDIVIDUAL *_R1,CARP_INDIVIDUAL *_R2):
	Move_base(R1,R2){}
};
#endif
