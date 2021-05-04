/*************************************************************************
 *  Name  : move.hpp
 *  Author: zmdsn
 *  Mail  : zmdsn@126.com 
 * Created Time: 2020年11月18日 星期三 13时47分27秒
 * 
 ************************************************************************/
#ifndef  __BETWEEN_BEST_2OPT__HPP__
#define  __BETWEEN_BEST_2OPT__HPP__
#include<iostream>
#include "../move/move.hpp"
using namespace std;
class BETWEEN_BEST_2OPT:public MOVE{
  // (x,y) <==>  (u,v) 
  public:
    MOVE* m;
    vector<int> P1;
    vector<int> P2;
    vector<int> load1; 
    vector<int> load2;
    vector<int> pre_cost;
    vector<int> back_cost;
    VType add_over_load(DType& load){
      if(load > problem->Capacity) {
        feasible = 0;
        over_load += load-problem->Capacity;
      }
      return over_load;
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
      over_load = 0;
      over_load_changed = 0;
      if(R1==R2) return 0;
      DType load;
      feasible = 1;
      if (change_mode) {
        load = loadi+loadj;
        add_over_load(load);
        load = R1->loads-loadi+R2->loads-loadj;
        add_over_load(load);
      } else {
        load = loadi+R2->loads;
        add_over_load(load);
        load = loadj+R1->loads;
        add_over_load(load);
      }
      over_load_changed = over_load-R1->over_load-R2->over_load;
      return over_load;
    }
    bool is_legal_move(){
      feasible = 0;
      over_load = 0; 
      if(R1==R2) {
        return 0;
      } 
      if(position1==0||position2==0) {
        return 0;
      } 
      if(position1+1>=R1->route.size()||
          position2+1>=R2->route.size()) {
        return 0;
      } 
      return 1;
    }
    bool is_feasible(){
      feasible = 1;
      if(!is_legal_move())    return 0;
      if(get_over_load()>0)    return 0;
      return 1;
    }
    void init_move() {
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
    VType merge_costs(){
      VType cost=0;
      cost  = (*problem)(u,y);
      cost += (*problem)(x,v);
      cost -= (*problem)(u,v);
      cost -= (*problem)(x,y);
      return cost;
    }
    VType reverse_costs(){
      VType cost=0;
      cost  = (*problem)(u,xp);
      cost += (*problem)(yp,v);
      cost -= (*problem)(u,v);
      cost -= (*problem)(x,y);
      return cost;
    }
    VType get_move_costs(){
      // cout<<"between_2opt"<<endl;
      // 这里有值得商榷的地方
      costs = 0;
      if(!is_legal_move())  return 0;
      init_move();
      VType cost;
      cost = merge_costs();
      change_mode = 0;
      costs = cost;
      cost = reverse_costs();
      if(cost<costs&&is_feasible()){
        costs = cost;
        change_mode = 1;
      }
      // cout<<costs<<endl;
      return costs;
    }
    bool move(){
      if(!is_legal_move())  return 0;
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
    BETWEEN_BEST_2OPT(){}
    BETWEEN_BEST_2OPT(CARP_INDIVIDUAL *_R1,CARP_INDIVIDUAL *_R2,
        int _position1, int _position2):
      MOVE(R1,R2,position1,position2){init_move();}
    BETWEEN_BEST_2OPT(CARP_INDIVIDUAL *_R1,CARP_INDIVIDUAL *_R2):MOVE(R1,R2){}
};
#endif
