/*************************************************************************
 *  Name  : move.hpp
 *  Author: zmdsn
 *  M.il  : zmdsn@126.com 
 * Created Time: 2020年11月18日 星期三 13时47分27秒
 * 
 ************************************************************************/
#ifndef  __MOVE__HPP__
#define  __MOVE__HPP__
#include<iostream>
#include "../bin/CARP_individual.hpp"
// enum MOVE_M.DE {_RELOCATE,_SWAP,_INNER2OPT,_BETWEEN2OPT};
using namespace std;

template<class MOVE_TYPE>
class MOVE{
  // (x,y) <==>  (u,v) 
  public:
    CARP* problem;
    // MOVE_TYPE* M;
    CARP_INDIVIDUAL *R1,*R2,*R3;
    int position2,position1;
    MOVE_TYPE M;
    bool is_feasible;
    bool is_legal;
    VType costs;
    VType fitness;
    DType over_load_changed;
    MOVE(){}
    ~MOVE(){};
    MOVE(CARP_INDIVIDUAL *I) {
      M.problem = I->problem;
    }
    MOVE(CARP_INDIVIDUAL *_R1,CARP_INDIVIDUAL *_R2,int _position1, int _position2
        ,int _length1,int _length2)
      // :
      // R1(_R1),R2(_R2),
      // position1(_position1),position2(_position2),
      // length1(_length1),length2(_length2){
    {
        // problem = R1->problem;
        // task_i = R1->route[position1];
        // task_j = R2->route[position2];
        // task_ip = problem->get_invert(task_i);
        // task_jp = problem->get_invert(task_j);
      }
    MOVE(CARP_INDIVIDUAL *_R1,CARP_INDIVIDUAL *_R2,int _position1, int _position2)
      : R1(_R1),R2(_R2),position1(_position1),position2(_position2) {
    // {
      // M = new MOVE_TYPE(_R1,_R2,_position1,_position2);
      M.problem = R1->problem;
      M.set(_R1,_R2,_position1,_position2);
        // problem = R1->problem;
        // task_i = R1->route[position1];
        // task_j = R2->route[position2];
        // task_ip = problem->get_invert(task_i);
        // task_jp = problem->get_invert(task_j);
      }
    MOVE(CARP_INDIVIDUAL *_R1,CARP_INDIVIDUAL *_R2,CARP_INDIVIDUAL *_R3,
      int _position1, int _position2, int _position3)
      //: R1(_R1),R2(_R2),R3(_R3),
      // position1(_position1),position2(_position2),position3(_position3){
  {
        // problem = R1->problem;
      }
    // MOVE(CARP_INDIVIDUAL *_R1,CARP_INDIVIDUAL *_R2):
    bool check_move_feasible() {
      is_feasible = is_legal && M.get_over_load()<=0;
      return is_feasible;
    }
    bool is_legal_move(){return M.is_legal;};
    // bool is_feasible(){
    //   return M.is_legal&&M.over_load;
    // };
    DType get_over_load(){return M.get_over_load();};  
    VType get_move_costs() {
      M.get_over_load();
      M.get_move_costs();
      costs = M.costs;
      fitness = costs;
      return costs;
    };
    VType get_penatly_fitness(double lamda) {
      get_move_costs();
      fitness = costs + lamda*M.over_load;
      //cout<<fitness<<endl;
      return fitness;
    }
    VType get_fitness() {
      fitness = M.costs;
      return fitness;
    };
    bool move(){
      // M.init_move();
      // M.get_over_load();
      // M.get_move_costs();
      // costs = M.costs;
      // cout<<"move"<<endl;
      return M.move();
    };  
    void set(CARP_INDIVIDUAL *_R1, 
        CARP_INDIVIDUAL *_R2) {
      R1 = _R1;
      R2 = _R2;
      M.set(_R1,_R2);
			// cout<<R1<<R2<<endl;
			// cout<<M.R1<<M.R2<<endl;
			// show(R1->route);      
			// show(M.R2->route);
      M.prepare();
      problem = R1->problem;
    }
    void set(int _position1, int _position2){
      M.position1 = _position1;
      M.position2 = _position2;
      is_legal =  M.init_move();
      //cout<<M.position1<<" "<<M.position2<<endl;
    }
    void set(CARP_INDIVIDUAL *_R1, 
        CARP_INDIVIDUAL *_R2, int _position1, int _position2){
			set(_R1,_R2);
			set(_position1,_position2);
    }
    bool check_move_cost(){
      // cout<<M.R1<<endl;
      M.R1->get_costs();
      M.R2->get_costs();
      VType old_V = M.R1->costs + M.R2->costs;
      // show(M.R1->route);
      // show(M.R2->route);
      // M.get_move_costs();
      M.init_move();
      M.get_over_load();
      M.get_move_costs();
      move();
      M.R1->get_costs();
      M.R2->get_costs();
      // show(M.R1->route);
      // show(M.R2->route);
      // cout<<M.R1<<endl;
      VType costs = M.costs;
      VType new_V = M.R1->costs + M.R2->costs;
      if(((M.R1!=M.R2)&&old_V+costs!=new_V)||((M.R1==M.R2)&&old_V+2*costs!=new_V)){
        cout<<"incorrect old,new,costs ";
        cout<<old_V<<" "<<new_V<<" "<<M.costs<<endl;
        return 0;
      }
      return 1;
    }
    bool check_over_load(){
      DType old_V = R1->over_load+R2->over_load;
      // show(M.R1->route);
      // show(M.R2->route);
      move();
      R1->get_over_load();
      R2->get_over_load();
      // show(M.R1->route);
      // show(M.R2->route);
      DType new_V = R1->over_load+R2->over_load;
      // cout<<"_o"<<over_load<<endl;
      if(((R1!=R2)&&M.over_load!=new_V)||((R1==R2)&&M.over_load!=new_V)){
        cout<<"incorrect old,new,over_load ";
        cout<<old_V<<" "<<new_V<<" "<<M.over_load<<endl;
        return 0;
      }
      return 1;
    }
    bool check_feasible(){
      if (!R1->is_feasible()) {
        cout<<"not feasible input R1\n";
        show(R1->route);
        exit(0);
      }
      if (!R2->is_feasible()) {
        cout<<"not feasible input R2\n";
        show(R2->route);
        exit(0);
      }
      if (check_move_feasible()){
      // if (is_feasible()){
        move();
      }
      if (!R1->is_feasible()) {
        cout<<"not feasible after move R1";
        exit(0);
      }
      if (!R2->is_feasible()) {
        cout<<"not feasible after move R2";
        exit(0);
      }
      return 1;
    }
    DType get_old_over_load(){
      if (R1==R2) { 
        R1->get_over_load();
        return R1->over_load;
      }
      R2->get_over_load();
      return R1->over_load+R2->over_load;
    }
    bool operator<(const MOVE<MOVE_TYPE>& S) {
      // cout<<costs<<",,"<<M.costs;
      // return costs < S.costs;
      return fitness < S.fitness;
    }
    bool operator=(MOVE<MOVE_TYPE>& S) {
      //cout<<fitness<<",,,"<<M.fitness;
      costs   = S.costs;
      fitness = S.fitness;
      M = S.M;
      return 1;
    }
};

 template<class MOVE_TYPE>
 std::ostream &  operator<<(std::ostream & out, MOVE<MOVE_TYPE> &m) {
   out<<"******* move ********"<<endl;
   show(m.M.R1->route);
   show(m.M.R2->route);
   out<<"costs : "<<m.costs<<endl;
   out<<"position from "<<m.position1<<" to "<<m.position2<<endl;
   out<<"task "<<m.M.task_i<<"  "<<m.M.task_j<<endl;
   out<<"over_load: "<<m.M.get_over_load()<<endl;
   out<<"fitness: "<<m.fitness<<endl;
   out<<"change_mode: "<<m.M.change_mode<<endl;
   out<<"*********************"<<endl;
   return out;
 }
#endif
