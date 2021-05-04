/*************************************************************************
 *  Name  : move.hpp
 *  Author: zmdsn
 *  Mail  : zmdsn@126.com 
 * Created Time: 2020年11月18日 星期三 13时47分27秒
 * 
 ************************************************************************/
#ifndef  __INV_BETWEEN_2OPT__HPP__
#define  __INV_BETWEEN_2OPT__HPP__
#include<iostream>
#include "../move/move.hpp"
using namespace std;
class INV_BETWEEN_2OPT:public Move_base{
  // (x,y) <==>  (u,v) 
  public:
    Move_base* m;
    vector<int> P1;
    vector<int> P2;
    vector<int> load1; 
    vector<int> load2;
    inline bool prepare() {
      pre_val.resize(problem->task_number*2+1);
      back_val.resize(problem->task_number*2+1);
      get_val(R1);
      get_val_back(R1);
      if (R1!=R2) {
        get_val(R2);
        get_val_back(R2);
      }
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
        R1->get_loads();
        R2->get_loads();
        get_load1();
        get_load2();
        loadi = load1[position1];
        loadj = load2[position2];
      }
      return 1;
    }
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
      if (change_mode) {
        temp_load = loadi+loadj;
        over_load += problem->get_over_load(temp_load);
        temp_load = total_over_load-temp_load;
        over_load += problem->get_over_load(temp_load);
      } else {
        temp_load = loadi+R2->loads-loadj;
        over_load += problem->get_over_load(temp_load);
        temp_load = loadj+R1->loads-loadi;
        over_load += problem->get_over_load(temp_load);

      }
      over_load_changed = over_load-old_over_load;
      return over_load;
    }
    bool is_legal_move(){
      is_legal = !(R1==R2)&&!(position1==0||position2==0);
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
    VType merge_costs(int _change_mode) {
      VType cost=0;
      int a = _change_mode&1?problem->get_invert(u):u;
      int b = _change_mode&2?problem->get_invert(v):v;
      int c = _change_mode&4?problem->get_invert(x):x;
      int d = _change_mode&8?problem->get_invert(y):y;
      cost  = (*problem)(a,d);
      cost += (*problem)(c,b);
      cost += pre_val[a];
      cost += back_val[b];
      cost += pre_val[c];
      cost += back_val[d];
      return cost;
    }
    VType reverse_costs(int _change_mode) {
      VType cost=0;
      int a = _change_mode&1?problem->get_invert(u):u;
      int b = _change_mode&2?v:problem->get_invert(v);
      int c = _change_mode&4?x:problem->get_invert(x);
      int d = _change_mode&8?problem->get_invert(y):y;
      cost  = (*problem)(a,d);
      cost += (*problem)(c,b);
      cost += pre_val[a];
      cost += back_val[b];
      cost += pre_val[c];
      cost += back_val[d];
      cost -= R1->costs+R2->costs;
      return cost;
    }
    VType get_move_costs(){
      // cout<<"between_2opt"<<endl;
      // 这里有值得商榷的地方
      costs = 0;
      if(!is_legal)  return 0;
      // init_move();
      VType cost;
      // cost = merge_costs();
      // change_mode = 0;
      // costs = cost;
      // cost = reverse_costs();
      // if(cost<costs&&is_feasible()){
      //   costs = cost;
      //   change_mode = 1;
      // }
      for (int i = 1; i <= 16; ++i) {
        cost = merge_costs(i);
        if(cost<costs) {
          costs = cost;
          costs -= R1->costs+R2->costs;
          change_mode = i;
        }
      }
      // cout<<costs<<endl;
      for (int i = 1; i <= 16; ++i) {
        cost = reverse_costs(i);
        if(cost<costs) {
          costs = cost;
          costs -= R1->costs+R2->costs;
          change_mode = i+32;
        }
      }
      return costs;
    }
    bool move(){
      if(!is_legal)  return 0;
      P1.clear();
      P2.clear();
      if(change_mode){
        for (int i = 0; i <= position1; ++i) {
          P1.push_back(R1->route[i]);
        }
        for (int i = position2; i >=0; --i) {
          P1.push_back(problem->get_invert(R2->route[i]));
        }
        for (int i = R1->route.size()-1; i >=position1+1; --i) {
          P2.push_back(problem->get_invert(R1->route[i]));
        }
        for (int i = position2+1; i <R2->route.size(); ++i) {
          P2.push_back(R2->route[i]);
        }
      } else {
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
      }
      R1->set_route(P1);
      R2->set_route(P2);
      return 1;
    }
    INV_BETWEEN_2OPT(){}
    INV_BETWEEN_2OPT(CARP_INDIVIDUAL *_R1,CARP_INDIVIDUAL *_R2,
        int _position1, int _position2):
      Move_base(R1,R2,position1,position2){init_move();}
    INV_BETWEEN_2OPT(CARP_INDIVIDUAL *_R1,CARP_INDIVIDUAL *_R2):
  Move_base(_R1,_R2){}
};
#endif
