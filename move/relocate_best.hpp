/*************************************************************************
 *  Name  : move.hpp
 *  Author: zmdsn
 *  Mail  : zmdsn@126.com 
 * Created Time: 2020年11月18日 星期三 13时47分27秒
 * 
 ************************************************************************/
#ifndef  __RELOCATE_BEST__HPP__
#define  __RELOCATE_BEST__HPP__
#include<iostream>
#include "../move/move.hpp"
#include "../move/relocate.hpp"
using namespace std;
class RELOCATE_BEST:public RELOCATE{
// (x,y) <==>  (u,v) 
 public:
  MOVE* m;
  int u,e,v,x,y;
  int up,ep,vp,xp,yp;
  // DType get_over_load(){
  //   DType temp_load = 0,old_over_load=0;
  //   over_load_changed = 0;
  //   over_load = 0;
  //   old_over_load = get_old_over_load();
  //   if (!is_legal_move()) {
  //     over_load = old_over_load;
  //     return over_load;
  //   }
  //   loadi = problem->get_demand(task_i);
  //   temp_load = R1->loads-loadi; 
  //   over_load += problem->get_over_load(temp_load);
  //   temp_load = R2->loads+loadi; 
  //   over_load += problem->get_over_load(temp_load);
  //   over_load_changed = over_load-old_over_load;
  //   return over_load;
  // }
  // bool p2_not_be_zero(){
  //   return position2!=0;
  // }
  // bool not_be_special(){
  //   return !(R1==R2&&
  //     (position1==position2||position1==position2-1));
  // }
  bool is_legal_move(){
    if(!not_be_zero())     return 0;
    if(!p2_not_be_zero()) return 0;
    if(R1==R2)     return 0;
    if(!not_be_special()) return 0;
    return 1;
  }
  bool is_feasible_different_route(){
    return get_over_load()<=0;
  }
  bool is_feasible(){
    feasible = 1;
    if(!is_legal_move())  return 0;
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
  void init_move(){
    get_pre_back_val(R1,R2);
    // (u,e,v),(x,y)
    u = R1->route[position1-1];
    e = R1->route[position1];
    v = R1->route[position1+1];
    x = R2->route[position2-1];
    y = R2->route[position2];
    up=problem->get_invert(u);
    ep=problem->get_invert(e);
    vp=problem->get_invert(v);
    xp=problem->get_invert(x);
    yp=problem->get_invert(y);
  }
  VType get_move_costs(){
    // cout<<"relocate"<<endl;
    costs = 0;
    if(!is_legal_move())  {
      // over_load_changed = 0;
      return 0;
    }
    init_move();
    VType pe,pep,cost,old_cost;
    R1->get_costs();
    R2->get_costs();
    old_cost = R1->costs + R2->costs;
    pe = min(pre_val[x]+(*problem)(x,e),
        pre_val[xp]+(*problem)(xp,e));
    pep = min(pre_val[x]+(*problem)(x,ep),
        pre_val[xp]+(*problem)(xp,ep));
    pe += +problem->get_cost(e);
    pep+= +problem->get_cost(e);
    // cout<<pe<<"*"<<pep<<endl;
    cost  = min(
        min(pe+(*problem)(e,y)+back_val[y],
          pe+(*problem)(e,yp)+back_val[yp]),
        min(pep+(*problem)(ep,y)+back_val[y],
          pep+(*problem)(ep,yp)+back_val[yp]));
    // cout<<cost<<endl;
    cost += min(
        min(pre_val[u]+(*problem)(u,v)+back_val[v],
          pre_val[u]+(*problem)(u,vp)+back_val[vp]),
        min(pre_val[up]+(*problem)(up,v)+back_val[v],
          pre_val[up]+(*problem)(up,vp)+back_val[vp]));
    // cout<<cost<<endl;
    cost -= old_cost;
    costs = cost;
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
    R1->get_best_in_sort(R1);
    R2->get_best_in_sort(R2);
    R1->get_fitness();
    R2->get_fitness();
    return 1;
  }
  RELOCATE_BEST(){}
  RELOCATE_BEST(CARP_INDIVIDUAL *_R1,CARP_INDIVIDUAL *_R2,
      int _position1, int _position2):
    RELOCATE(R1,R2,position1,position2){}
  RELOCATE_BEST(CARP_INDIVIDUAL *_R1,CARP_INDIVIDUAL *_R2):RELOCATE(R1,R2){}
};
#endif
