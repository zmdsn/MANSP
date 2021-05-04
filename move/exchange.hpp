/*************************************************************************
 *  Name  : move.hpp
 *  Author: zmdsn
 *  Mail  : zmdsn@126.com 
 * Created Time: 2020年11月18日 星期三 13时47分27秒
 * 
 ************************************************************************/
#ifndef  __EXCHANGE__HPP__
#define  __EXCHANGE__HPP__
#include<iostream>
#include "../move/move.hpp"
// #include "move.hpp"
using namespace std;
class EXCHANGE:public Move_base{
  // (x,y) <==>  (u,v) 
  public:
    Move_base* m;
    bool can_be_move;
    DType load_before_u,load_u_v,load_after_v;
    DType load_before_x,load_x_y,load_after_y;
    inline bool init_move(){
      is_legal = is_legal_move();
     // if(is_legal){
        before_u = R1->route[position1-1];
        after_v  = R1->route[position1+length1+1];
        before_x = R2->route[position2-1];
        after_y  = R2->route[position2+length2+1];
        u = R1->route[position1];
        v = R1->route[position1+length1];
        x = R2->route[position2];
        y = R2->route[position2+length2];
        up = problem->get_invert(u);
        vp = problem->get_invert(v);
        xp = problem->get_invert(x);
        yp = problem->get_invert(y);
        // cout<<before_u<<u<<v<<after_v<<endl;
        // cout<<before_x<<x<<y<<after_y<<endl;
        // cout<<task1<<task2<<task3<<task4<<endl;
     // }
      // else {
      //   R1->get_loads();
      //   R2->get_loads();
      //   over_load = R1->over_load + R2->over_load;
      //   // cout<<over_load<<"**";
      //   over_load_changed = 0;
      // }
    }
    DType get_load_before_u(){
      load_before_u = 0;
      for (int i=0; i<=position1-1; i++) {
        load_before_u += problem->get_demand(R1->route[i]);
      }
      return load_before_u;
    }
    DType get_load_u_v(){
      load_u_v = 0;
      for (int i=position1; i<=position1+length1; i++) {
        load_u_v += problem->get_demand(R1->route[i]);
      }
      return load_u_v;
    }
    DType get_load_after_v(){
      load_after_v = 0;
      for (int i=position1+length1+1; i<R1->route.size(); i++) {
        load_after_v += problem->get_demand(R1->route[i]);
      }
      return load_after_v;
    }
    DType get_load_before_x(){
      load_before_x = 0;
      for (int i=0; i<=position2-1; i++) {
        load_before_x += problem->get_demand(R2->route[i]);
      }
      return load_before_x;
    }
    DType get_load_x_y(){
      load_x_y = 0;
      for (int i=position2; i<=position2+length2; i++) {
        load_x_y += problem->get_demand(R2->route[i]);
      }
      return load_x_y;
    }
    DType get_load_after_y(){
      load_after_y = 0;
      for (int i=position2+length2+1; i<R2->route.size(); i++) {
        load_after_y += problem->get_demand(R2->route[i]);
      }
      return load_after_y;
    }
    DType get_over_load_new(){
      over_load = 0;
      DType load1 = 0;
      DType load2 = 0;
      load1 += get_load_before_u(); 
      load1 += get_load_x_y(); 
      load1 += get_load_after_v(); 
      // cout<<load1;
      load2 += get_load_before_x(); 
      load2 += get_load_u_v(); 
      load2 += get_load_after_y(); 
      // cout<<load2<<endl; 
      if (load1 > problem->Capacity) {
        feasible = 0;
        over_load += load1-problem->Capacity;
      }
      if (load2 > problem->Capacity) {
        feasible = 0;
        over_load += load2-problem->Capacity;
      }
      return over_load;
    }
    DType get_over_load(){
      R1->get_loads();
      R2->get_loads();
      over_load = R1->over_load + R2->over_load;
      over_load_changed = 0;
      if(!tasks_can_be_move())return 0;
      get_over_load_new();
      over_load_changed = over_load-R1->over_load-R2->over_load;
      return over_load;
    }
    bool tasks_can_be_move(){
      //can_be_move = 0;
      if(R1==R2)  return 0;
      if(position1<1)  return 0;
      if(position1+length1+1>=R1->route.size()) return 0;
      if(position2<1) return 0;
      if(position2+length2+1>=R2->route.size()) return 0;
      //can_be_move = 1;
      return 1;
    }
    bool is_legal_move() {
      is_legal = tasks_can_be_move();
      return is_legal;
    }
    bool is_feasible(){
      feasible = 1;
      if(!is_legal_move())  return 0;
      if(get_over_load()>0)  return 0;
      return 1;
    }

    VType get_swap_costs(int _change_mode){
      // bu,u,v,av   bx,x,y,ay    bu for before u, so on
      // unit_mode 0 > bu x y av,  bx u v ay
      // unit_mode 1 > bu x y av,  bx vp up ay
      // unit_mode 2 > bu yp xp av,  bx u v ay
      // unit_mode 3 > bu yp xp av,  bx vp up ay
      get_unit_mode(_change_mode);
      int task1 = unit_mode[0]?vp:u;
      int task2 = unit_mode[0]?up:v;
      int task3 = unit_mode[1]?yp:x;
      int task4 = unit_mode[1]?xp:y;
      VType cost = 0;
      cost += (*problem)(before_u,task3);
      cost += (*problem)(task4,after_v);
      cost += (*problem)(before_x,task1);
      cost += (*problem)(task2,after_y);
      cost -= (*problem)(before_u,u);
      cost -= (*problem)(v,after_v);
      cost -= (*problem)(before_x,x);
      cost -= (*problem)(y,after_y);
      return cost;
    }
    VType get_move_costs(){
      // cout<<"exchange"<<endl;
      costs = 0;
      over_load = 0;
      init_move();
      if(!is_legal)  return INF;
      VType cost;
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
    void construct_P1(vector<int>& P1){
      P1.insert(P1.end(),R1->route.begin(),R1->route.begin()+position1);
      P1.insert(P1.end(),R2->route.begin()+position2,
          R2->route.begin()+position2+length2+1);
      if(inv_task_j) {
        reverse_tasks(P1,position1,length2);
      } 
      P1.insert(P1.end(),R1->route.begin()+position1+length1+1,R1->route.end());
    }
    void construct_P2(vector<int>& P2){
      P2.insert(P2.end(),R2->route.begin(),R2->route.begin()+position2);
      P2.insert(P2.end(),R1->route.begin()+position1,
          R1->route.begin()+position1+length1+1);          ;
      if(inv_task_i) {
        reverse_tasks(P2,position2,length1);
      }
      P2.insert(P2.end(),R2->route.begin()+position2+length2+1,R2->route.end());
    }
    bool move(){
      if(!is_legal)  return 0;
      vector<int> P1;
      vector<int> P2;
      construct_P1(P1);
      construct_P2(P2);
      // cout<<"move\n"<<costs;
      // show(R1->route);
      // show(R2->route);
      R1->set_route(P1);
      R2->set_route(P2);
      R1->get_fitness();
      R2->get_fitness();
      // show(P1);
      // show(P2);
      return 1;
    }
    EXCHANGE(){
        length1 = 0;
        length2 = 0;
    }
    EXCHANGE(CARP_INDIVIDUAL *_R1,CARP_INDIVIDUAL *_R2,
        int _position1, int _position2,int _length1,int _length2):
      Move_base(_R1,_R2,_position1,_position2,_length1,_length2){
      }
    EXCHANGE(CARP_INDIVIDUAL *_R1,CARP_INDIVIDUAL *_R2,
        int _position1, int _position2):
      Move_base(_R1,_R2,_position1,_position2){
        length1 = 0;
        length2 = 0;
      }
    EXCHANGE(CARP_INDIVIDUAL *_R1,CARP_INDIVIDUAL *_R2):
      Move_base(_R1,_R2){
        length1 = 0;
        length2 = 0;
    }
};

std::ostream &  operator<<(std::ostream & out,EXCHANGE& m) {
  out<<"******* move ********"<<endl;
  show(m.R1->route);
  show(m.R2->route);
  out<<"costs : "<<m.costs<<endl;
  out<<"position "<<m.position1<<" add "<<m.length1<<endl;
  out<<"position "<<m.position2<<" add "<<m.length2<<endl;
  out<<"task inv"<<m.inv_task_i<<"  "<<m.inv_task_j<<endl;
  out<<"change_mode: "<<m.change_mode<<endl;
  out<<"over_load: "<<m.over_load<<endl;
  out<<"fitness: "<<m.fitness<<endl;
  out<<"*********************"<<endl;
  return out;
}
#endif
