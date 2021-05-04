/*************************************************************************
 *  Name  : move.hpp
 *  Author: zmdsn
 *  Mail  : zmdsn@126.com 
 * Created Time: 2020年11月18日 星期三 13时47分27秒
 * 
 ************************************************************************/
#ifndef  __Between2OPTReverse__HPP__
#define  __Between2OPTReverse__HPP__
#include<iostream>
#include "../move/between_2opt_merge.hpp"
using namespace std;
class Between2OPTReverse:public Between2OPTMerge{
  // (x,y) <==>  (u,v) 
  public:
    // Move_base* m;
    // vector<int> P1;
    // vector<int> P2;
    // vector<int> load1; 
    // vector<int> load2;
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
      temp_load = loadi+loadj;
      over_load += problem->get_over_load(temp_load);
      temp_load = total_over_load-temp_load;
      over_load += problem->get_over_load(temp_load);
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
      costs = 0;
      if(!is_legal)  return 0;
      // init_move();
      costs = reverse_costs();
      // cout<<costs<<endl;
      return costs;
    }
    bool move(){
      if(!is_legal)  return 0;
      P1.clear();
      P2.clear();
      // show(R1->route);
      // show(R2->route);
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
      R1->set_route(P1);
      R2->set_route(P2);
      // show(R1->route);
      // show(R2->route);
      return 1;
    }
    Between2OPTReverse(){}
    Between2OPTReverse(CARP_INDIVIDUAL *_R1,CARP_INDIVIDUAL *_R2,
        int _position1, int _position2):
      Between2OPTMerge(R1,R2,position1,position2){init_move();}
    Between2OPTReverse(CARP_INDIVIDUAL *_R1,CARP_INDIVIDUAL *_R2):
  Between2OPTMerge(_R1,_R2){}
};
#endif
