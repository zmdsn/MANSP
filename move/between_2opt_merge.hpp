/*************************************************************************
 *  Name  : move.hpp
 *  Author: zmdsn
 *  Mail  : zmdsn@126.com 
 * Created Time: 2020年11月18日 星期三 13时47分27秒
 * 
 ************************************************************************/
#ifndef  __Between2OPTMerge__HPP__
#define  __Between2OPTMerge__HPP__
#include<iostream>
#include "../move/move_base.hpp"
using namespace std;

class Between2OPTMerge:public Move_base{
  // (x,y) <==>  (u,v) 
  public:
    Move_base* m;
    vector<int> P1;
    vector<int> P2;
    vector<int> load1; 
    vector<int> load2;
    void get_load1(){
      loadi = 0;
      load1.resize(R1->route.size(),0);
      R1->get_loads();
      for (int i=0; i<R1->route.size(); i++) {
        loadi += problem->get_demand(R1->route[i]);
        load1[i] = loadi;
      }
    }
    void get_load2(){
      loadj = 0;
      load2.resize(R2->route.size(),0);
      R2->get_loads();
      for (int i=0; i<R2->route.size(); i++) {
        loadj += problem->get_demand(R2->route[i]);
        load2[i] = loadj;
      }
    }
    inline bool prepare() {
      pre_val.resize(problem->task_number*2+1);
      back_val.resize(problem->task_number*2+1);
      get_val(R1);
      get_val_back(R1);
      if (R1!=R2) {
        get_val(R2);
        get_val_back(R2);
      }
      get_load1();
      get_load2();
      // R1->get_costs();
      // R2->get_costs();
      is_prepared = 1;
      return 1;
    }
    inline bool init_move() {
      is_legal= is_legal_move();
      if (is_legal) {
        if (!is_prepared) prepare();
        u = R1->route[position1];
        v = R1->route[position1+1];
        x = R2->route[position2];
        y = R2->route[position2+1];
        up = problem->get_invert(u);
        vp = problem->get_invert(v);
        xp = problem->get_invert(x);
        yp = problem->get_invert(y);
        // R1->get_loads();
        // R2->get_loads();
        loadi = load1[position1];
        loadj = load2[position2];
      }
      return 1;
    }
    DType get_over_load(){
      // if(!is_legal)  return 0;
      if (!is_legal) {
        over_load = old_over_load;
        return over_load;
      }
      over_load_changed = 0;
      over_load = 0;
      DType old_over_load = get_old_over_load();
      DType total_over_load = R1->loads + R2->loads;
      DType temp_load;
      temp_load = loadi+R2->loads-loadj;
      over_load += problem->get_over_load(temp_load);
      temp_load = loadj+R1->loads-loadi;
      over_load += problem->get_over_load(temp_load);
      return over_load;
    }
    bool is_legal_move(){
      is_legal = (R1!=R2)&&!(position1==0||position2==0);
      is_legal&=!(position1+1>=R1->route.size()||
          position2+1>=R2->route.size());
      return is_legal;
    }
    bool is_feasible(){
      feasible = 1;
      if(!is_legal)    return 0;
      if(get_over_load()>0)    return 0;
      return 1;
    }
    VType merge_costs(){
      VType cost=0;
      cost  = (*problem)(u,y);
      cost += (*problem)(x,v);
      cost -= (*problem)(u,v);
      cost -= (*problem)(x,y);
      return cost;
    }
    VType get_move_costs(){
      // cout<<"between_2opt"<<endl;
      costs = 0;
      if(!is_legal)  return 0;
      // init_move();
      costs = merge_costs();
      // cout<<costs<<endl;
      return costs;
    }
    bool move(){
      if(!is_legal)  return 0;
      // show(R1->route);
      // show(R2->route);
      P1.clear();
      P2.clear();
      for (int i = 0; i <= position1; ++i) {
        P1.push_back(R1->route[i]);
      }
      for (int i = position2+1; i <R2->route.size(); ++i) {
        P1.push_back(R2->route[i]);
      }
      for (int i = 0; i <= position2; ++i) {
        P2.push_back(R2->route[i]);
      }
      for (int i = position1+1; i <R1->route.size(); ++i) {
        P2.push_back(R1->route[i]);
      }
      R1->set_route(P1);
      R2->set_route(P2);
      // show(R1->route);
      // show(R2->route);
      return 1;
    }
    Between2OPTMerge(){}
    Between2OPTMerge(CARP_INDIVIDUAL *_R1,CARP_INDIVIDUAL *_R2,
        int _position1, int _position2):
      Move_base(R1,R2,position1,position2){init_move();}
    Between2OPTMerge(CARP_INDIVIDUAL *_R1,CARP_INDIVIDUAL *_R2):
  Move_base(_R1,_R2){}
};
#endif
