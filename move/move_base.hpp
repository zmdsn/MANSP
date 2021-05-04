/*************************************************************************
 *  Name  : move.hpp
 *  Author: zmdsn
 *  Mail  : zmdsn@126.com 
 * Created Time: 2020年11月18日 星期三 13时47分27秒
 * 
 ************************************************************************/
#ifndef  __Move_base__HPP__
#define  __Move_base__HPP__
#include<iostream>
#include "../bin/CARP_individual.hpp"
// enum Move_base_MODE {_RELOCATE,_SWAP,_INNER2OPT,_BETWEEN2OPT};
#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))
using namespace std;
class Move_base{
  // (x,y) <==>  (u,v) 
  public:
    clock_t start_time,end_time;
    CARP* problem;
    CARP_INDIVIDUAL *R1,*R2,*R3; // for the route
    VType costs;
    double fitness;
    int task_i,task_ip;
    int task_j,task_jp;
    int position1,position2,position3;  // for the position
    int change_mode;
    vector<int> unit_mode;
    bool feasible;
    bool is_legal;
    bool inv_task_i,inv_task_j;
    DType loadi,loadj; // for the task
    DType old_over_load;
    DType over_load;
    DType over_load_changed;
    int length1,length2;
    int u,v,up,vp; // for R1
    int before_u,after_v;
    int x,y,xp,yp; // for R2
    int before_x,after_y;
    int to_insert_task;
    int insert_position;
    bool is_prepared = 0;
    vector<VType> pre_val;
    vector<VType> back_val;
    bool operator<(const Move_base& M) {
      // cout<<costs<<",,"<<M.costs;
      return costs < M.costs;
    }
    DType get_old_over_load(){
      if (R1==R2) { 
        return R1->over_load;
      }
      return R1->over_load + R2->over_load;
    }
    // DType get_over_load_same_route() {
    //   R1->get_over_load();
    //   return R1->over_load;
    //   return 0;
    // }
    // VType get_penatly_fitness(double lamda){
    //   get_move_costs();
    //   get_over_load();
    //   fitness = costs + lamda*over_load;
    //   return fitness;
    // }
    // VType get_fitness(double lamda){
    //   get_move_costs();
    //   get_over_load();
    //   fitness = costs + lamda*over_load;
    //   return fitness;
    // }
    // VType get_fitness(){
    //   get_move_costs();
    //   fitness = costs + R1->lamda*over_load;
    //   return fitness;
    // }
  inline bool prepare() {
    //R1->get_loads();
    //R2->get_loads();
    // R1->get_over_load();
    // R2->get_over_load();
    // R1->get_costs();
    // R2->get_costs();
    is_prepared = 1;
    return 1;
  }
    VType get_invert_costs(CARP_INDIVIDUAL* R,int p){
      // insert t to R[p]
      VType cost = 0;
      int t  = R->route[p];
      int tp = problem->get_invert(t);
      cost += (*problem)(R->route[p-1],tp);
      cost += (*problem)(tp,R->route[p+1]);
      cost -= (*problem)(R->route[p-1],t);
      cost -= (*problem)(t,R->route[p+1]);
      return cost;
    }
    VType get_invert_costs(){
      costs = get_invert_costs(R1,position1);
      return costs;
    }
    void reverse_tasks(vector<int>& V,int p1,int length){
        reverse(V.begin()+p1, V.begin()+p1+length+1);
        for (int i=0; i<=length; i++) {
          invert_task(V,p1+i);
        }
    }
    void invert_task(vector<int>& V,int p){
      V[p] = problem->get_invert(V[p]);
    }
    void invert_task(){
      invert_task(R1->route,position1);
    }
    VType get_insert_k_costs(CARP_INDIVIDUAL* R,int p,vector<int> t){
      // insert t to R[p]
      VType cost = 0;
      cost  = (*problem)(R->route[p-1],t[0]);
      cost += (*problem)(t.back(),R->route[p]);
      cost -= (*problem)(R->route[p-1],R->route[p]);
      for (int i = 0; i < t.size(); ++i) {
        cost += problem->get_cost(t[i]);
        if (i+1<t.size()) {
          cost += (*problem)(t[i],t[i+1]);
        }
      }
      return cost;
    }
    VType get_insert_costs(CARP_INDIVIDUAL* R,int p,int t){
      // insert t to R[p]
      VType cost = 0;
      cost  = (*problem)(R->route[p-1],t);
      cost -= (*problem)(R->route[p-1],R->route[p]);
      cost += (*problem)(t,R->route[p]);
      cost += problem->get_cost(t);
      return cost;
    }
		inline VType get_pre_back_val(int u,int x,int v){
			// u x v
			VType cost = pre_val[u]+(*problem)(u,x);
			cost += (*problem)(x,v)+back_val[v];
			cost += problem->get_cost(x);
			return cost;
		}
    VType get_inv_insert_costs(CARP_INDIVIDUAL* R,int p,int x,int& mode){
      // insert t to R[p]
      VType min_cost = INF,cost;
			int u = R->route[p-1];
			int v = R->route[p];
			int up= problem->get_invert(u);
			int vp= problem->get_invert(v);
			int xp= problem->get_invert(x);
			int a,b,c;
			// for (int i=0; i<8; i++) {
			//   cout<< (i&1) <<" ";
			//   cout<< (i&2) <<" ";
			//   cout<< (i&4) <<endl;
			// }
			for (int i=0; i<8; i++) {
				a = (i&1)?up:u;
				b = (i&2)?xp:x;
				c = (i&4)?vp:v;
				// cout<< (i&1) <<" ";
				// cout<< (i&2) <<" ";
				// cout<< (i&4) <<endl;
				// cout<<a<<" "<<b<<" "<<c<<endl;
				cost = get_pre_back_val(a,b,c);
				if (cost<min_cost) {
					min_cost = cost;
					mode = i;
				}
			}
			// cost += problem->get_cost(x);
      return min_cost;
    }
    VType get_insert_costs(){
      // insert task1 to position2
      costs = get_insert_costs(R2,position2,R1->route[position1]);
      return costs;
    }
    void insert_task(vector<int>& V,int p,int e) {
      // insert e to p
      V.insert(V.begin()+p,e);
    }
    void insert_k_task(vector<int>& V,int p,vector<int> es) {
      // insert e to p
      V.insert(V.begin()+p,es.begin(),es.end());
    }
    void insert_task(){
      // from R1 delete position1
      int e = inv_task_i?task_ip:task_i;
      insert_task(R2->route,position2,e);
    }
    inline VType get_delete_costs(CARP_INDIVIDUAL* R,int p){
      // delete R[p]
      // show(R->route);
      VType cost = 0;
      int t = R->route[p];
      cost  = (*problem)(R->route[p-1],R->route[p+1]);
      // cout<<cost<<">"<<p<<R->route[p-1]<<R->route[p+1];
      cost -= (*problem)(R->route[p-1],R->route[p]);
      // cout<<cost<<">";
      cost -= (*problem)(R->route[p],R->route[p+1]);
      // cout<<cost<<">";
      cost -= problem->get_cost(t);
      // cout<<cost<<">";
      return cost;
    }
    VType get_delete_k_costs(CARP_INDIVIDUAL* R,int p,int k){
      // delete R[p] -> R[p+k] [)
      VType cost = 0;
      int t = R->route[p];
      cost  = (*problem)(R->route[p-1],R->route[p+k]);
      // cout<<k<<"==>"<<cost<<"*" ;
      cost -= (*problem)(R->route[p-1],R->route[p]);
      // cout<<cost<<"*" ;
      cost -= (*problem)(R->route[p+k-1],R->route[p+k]);
      // cout<<cost<<"*" ;
      for (int i = 0; i < k; ++i) {
        cost -= problem->get_cost(R->route[p+i]);
        // cout<<cost<<"*"<<endl;
        if (i!=k-1) cost -= (*problem)(R->route[p+i],R->route[p+i+1]);
        // cout<<cost<<"*"<<endl;
      }
      // cout<<cost<<"*"<<endl;
      return cost;
    }
    void delete_task(vector<int> &V,int p){
      // from V delete p
      V.erase(V.begin()+p); // 删除第p+1个元素
    }
    void delete_k_task(vector<int> &V,int p,int k){
      // from V delete [p,p+k) 
      V.erase(V.begin()+p,V.begin()+p+k);
    }
    void delete_task(){
      // from R1 delete position1
      // show(R1->route);
      R1->route.erase(R1->route.begin()+position1);
      // show(R1->route);
    }
    VType get_replace_costs(CARP_INDIVIDUAL* R,int p,int task){
      // chage R[p] as task
      VType cost = 0;
      cost  = (*problem)(R->route[p-1],task);
      cost += (*problem)(task,R->route[p+1]);
      cost += problem->get_cost(task);
      cost -= (*problem)(R->route[p-1],R->route[p]);
      cost -= (*problem)(R->route[p],R->route[p+1]);
      cost -= problem->get_cost(R->route[p]);
      return cost;
    }
    VType get_replace_costs(){
      costs = get_replace_costs(R2,position2,R1->route[position1]);
      return costs;
    }
    void replace_task(vector<int>& V,int p,int t){
      // replace p by task t
      V[p] = t;
    }
    void replace_task(){
      // from R1 delete position1
      int e = inv_task_i?task_ip:task_i;
      replace_task(R2->route,position2,e);
    }
    bool not_be_zero(){
      if(R1->route[position1] == 0) {
        feasible = 0;
        // R1->get_loads();
        // R2->get_loads();
        over_load = R1->over_load+R2->over_load; 
        // cout<<"ov_"<<over_load<<endl;
        // over_load = 0; 
        return 0;
      }
      return 1;
    }
    bool is_same_route(){
      if(R1==R2) {
        feasible = 1;
        over_load = 0; 
        return 1;
      }
      return 0;
    }
    bool copy(Move_base& m) {
      task_ip = m.task_ip;
      task_jp = m.task_jp;
      task_i  = m.task_i;
      task_j  = m.task_j;
      costs = m.costs;
      position1 = m.position1;
      position2 = m.position2;
      inv_task_i = m.inv_task_i;
      inv_task_j = m.inv_task_j;
      feasible = m.feasible;
      change_mode = m.change_mode;
      over_load = m.over_load;
      fitness = m.fitness;
      return 1;
    }
    void set(CARP_INDIVIDUAL *_R1, 
        CARP_INDIVIDUAL *_R2){
      R1 = _R1;
      R2 = _R2;
      problem = R1->problem;
			// cout<<R1<<R2<<endl;
			// cout<<"SS"<<endl;
			// show(R1->route);      
			// show(R2->route);
    }
    void set(CARP_INDIVIDUAL *_R1, 
        CARP_INDIVIDUAL *_R2,
        int _position1, int _position2){
      R1 = _R1;
      R2 = _R2;
      position1 = _position1;
      position2 = _position2;
      problem = R1->problem;
      task_i = R1->route[position1];
      task_j = R2->route[position2];
      task_ip = problem->get_invert(task_i);
      task_jp = problem->get_invert(task_j);
    }
    void set(CARP_INDIVIDUAL *_R1, 
        CARP_INDIVIDUAL *_R2,
        int _position1, int _position2,
        int _length1,int _length2){
      set(_R1,_R2,_position1,_position2);
      // R1 = _R1;
      // R2 = _R2;
      // position1 = _position1;
      // position2 = _position2;
      problem = R1->problem;
      length1 = _length1;
      length2 = _length2;
      // if(is_legal_move()){
      //   init_move();
      //   // cout<<"**** u,v x,y******\n";
      //   // cout<<before_u<<endl;
      //   // cout<<u<<endl;
      //   // cout<<v<<endl;
      //   // cout<<after_v<<endl;
      //   // cout<<before_x<<endl;
      //   // cout<<x<<endl;
      //   // cout<<y<<endl;
      //   // cout<<after_y<<endl;
      //   // cout<<R1->route.size()<<endl;
      //   // cout<<position1+length1-1<<endl;
      //   // cout<<R2->route.size()<<endl;
      //   // cout<<position2+length2-1<<endl;
      //   // cout<<"**** ******\n";
      // }
    }
    void set(CARP_INDIVIDUAL *_R1, 
        CARP_INDIVIDUAL *_R2,
        CARP_INDIVIDUAL *_R3,
        int _position1, int _position2, int _position3){
      R1 = _R1;
      R2 = _R2;
      R3 = _R3;
      position1 = _position1;
      position2 = _position2;
      position3 = _position3;
      problem= R1->problem;
    }
    Move_base(){}
    ~Move_base(){};
    Move_base(CARP_INDIVIDUAL *_R1,CARP_INDIVIDUAL *_R2,int _position1, int _position2
        ,int _length1,int _length2):
      R1(_R1),R2(_R2),
      position1(_position1),position2(_position2),
      length1(_length1),length2(_length2){
        problem = R1->problem;
        // task_i = R1->route[position1];
        // task_j = R2->route[position2];
        // task_ip = problem->get_invert(task_i);
        // task_jp = problem->get_invert(task_j);
      }
    Move_base(CARP_INDIVIDUAL *_R1,CARP_INDIVIDUAL *_R2,int _position1, int _position2):
      R1(_R1),R2(_R2),
      position1(_position1),position2(_position2){
        problem = R1->problem;
        task_i = R1->route[position1];
        task_j = R2->route[position2];
        task_ip = problem->get_invert(task_i);
        task_jp = problem->get_invert(task_j);
      }
    Move_base(CARP_INDIVIDUAL *_R1,CARP_INDIVIDUAL *_R2,CARP_INDIVIDUAL *_R3,
      int _position1, int _position2, int _position3):
      R1(_R1),R2(_R2),R3(_R3),
      position1(_position1),position2(_position2),position3(_position3){
        problem = R1->problem;
      }
    Move_base(CARP_INDIVIDUAL *_R1,CARP_INDIVIDUAL *_R2):
      R1(_R1),R2(_R2){
        problem = R1->problem;
        task_i = R1->route[position1];
        task_j = R2->route[position2];
        task_ip = problem->get_invert(task_i);
        task_jp = problem->get_invert(task_j);
      }
    // bool check_over_load(){
    //   R1->get_over_load();
    //   R2->get_over_load();
    //   DType old_V = R1->over_load+R2->over_load;
    //   get_over_load();
    //   move();
    //   R1->get_over_load();
    //   R2->get_over_load();
    //   DType new_V = R1->over_load+R2->over_load;
    //   // cout<<"_o"<<over_load<<endl;
    //   if(((R1!=R2)&&over_load!=new_V)||((R1==R2)&&over_load!=new_V)){
    //     cout<<"incorrect old,new,over_load ";
    //     cout<<old_V<<" "<<new_V<<" "<<over_load<<endl;
    //     return 0;
    //   }
    //   return 1;
    // }
    // bool check_feasible(){
    //   if (!R1->is_feasible()) {
    //     cout<<"not feasible input R1\n";
    //     show(R1->route);
    //     exit(0);
    //   }
    //   if (!R2->is_feasible()) {
    //     cout<<"not feasible input R2\n";
    //     show(R2->route);
    //     exit(0);
    //   }
    //   if (is_feasible()){
    //     move();
    //   }
    //   if (!R1->is_feasible()) {
    //     cout<<"not feasible after move R1";
    //     exit(0);
    //   }
    //   if (!R2->is_feasible()) {
    //     cout<<"not feasible after move R2";
    //     exit(0);
    //   }
    //   return 1;
    // }
    // bool check_move_cost(){
    //   R1->get_costs();
    //   R2->get_costs();
    //   VType old_V = R1->costs+R2->costs;
    //   // show(R1->route);
    //   // show(R2->route);
    //   move();
    //   R1->get_costs();
    //   R2->get_costs();
    //   VType new_V = R1->costs+R2->costs;
    //   if(((R1!=R2)&&old_V+costs!=new_V)||((R1==R2)&&old_V+2*costs!=new_V)){
    //     cout<<"incorrect old,new,costs ";
    //     cout<<old_V<<" "<<new_V<<" "<<costs<<endl;
    //     return 0;
    //   }
    //   return 1;
    // }
    // bool check_move_cost3(){
    //   // show(R1->route);
    //   // show(R2->route);
    //   // show(R3->route);
    //   R1->get_costs();
    //   R2->get_costs();
    //   R3->get_costs();
    //   VType old_V = R1->costs+R2->costs+R3->costs;
    //   move();
    //   R1->get_costs();
    //   R2->get_costs();
    //   R3->get_costs();
    //   VType new_V = R1->costs+R2->costs+R3->costs;
    //   if((R1!=R2)&&old_V+costs!=new_V){
    //     // show(R1->route);
    //     // show(R2->route);
    //     // show(R3->route);
    //     // cout<<"position "<<position1<<","<<position2<<","<<position3<<endl;
    //     cout<<"incorrect old,new,costs ";
    //     cout<<old_V<<" "<<new_V<<" "<<costs<<endl;
    //     // exit(0);
    //     return 0;
    //   }
    //   return 1;
    // }
    void get_unit_mode(int k){
      int max_ = 4;
      unit_mode.clear();
      unit_mode.resize(max_);
      for (int i=0; i<max_; i++) {
        unit_mode[i] = k%2;
        k>>=1;
      }
    }
    void change_mode_to_task_inv(int k){
      get_unit_mode(k);
      inv_task_i = unit_mode[0];
      inv_task_j = unit_mode[1];
    }
    // virtual bool is_feasible(){return 1;};
    // virtual bool is_legal_move(){return 1;};
    // virtual DType get_over_load(){cout<<"get over load"<<endl;return 0;};  
    // virtual VType get_move_costs(){return INF;};
    // virtual bool move(){return 0;};  
    // virtual void init_move(){return ;};  
    void value_vector_back(int e,int pre,int prep,vector<VType>& V){
      if(pre!=prep) {
        VType value_pre = (*problem)(e, pre )+V[pre]; 
        VType value_prep= (*problem)(e, prep)+V[prep];
        V[e] = value_pre<value_prep?value_pre:value_prep;
      } else {
        V[e] = (*problem)(e,pre)+V[pre];
      }
      V[e] += problem->get_cost(e);
      // cout<<e<<"=>tag,mark "<<tag_values[e]<<mark[e]<<endl;
    }
    void value_vector(int e,int pre,int prep,vector<VType>& V){
      if(pre!=prep) {
        VType value_pre = (*problem)(pre ,e)+V[pre]; 
        VType value_prep= (*problem)(prep,e)+V[prep];
        V[e] = value_pre<value_prep?value_pre:value_prep;
      } else {
        V[e] = (*problem)(pre,e)+V[pre];
      }
      V[e] += problem->get_cost(e);
      // cout<<e<<"=>tag,mark "<<tag_values[e]<<mark[e]<<endl;
    }
    void get_pre_back_val(CARP_INDIVIDUAL* P){
      int ep,pre,prep;
      pre_val.resize(problem->task_number*2+1);
      back_val.resize(problem->task_number*2+1);
      for (auto e: P->route) {
        ep = problem->get_invert(e);
        pre_val[e]  = 0;
        back_val[e] = 0;
      }
      pre = -1;
      for (auto e: P->route) {
        if (e==0) {
          if (pre<=0){
            prep = 0;
            pre = 0;
            continue;
          } else {
            // value_vector(e,pre,prep,pre_val);
            pre_val[0] = 0;
            prep = 0;
            pre = 0;
          }
        } else {
          ep = problem->get_invert(e);
          value_vector(e,pre,prep,pre_val);
          value_vector(ep,pre,prep,pre_val);
          pre  = e;
          prep = ep;
        }
      }
      int e;
      pre = -1;
      // for (auto e: P->route) {
      for (int i=P->route.size()-1;i>=0; i--) {
        e = P->route[i];
        if (e==0) {
          if (pre<=0){
            prep = 0;
            pre = 0;
            continue;
          } else {
            // value_vector_back(e,pre,prep,back_val);
            // this maybe cause (... x 0) error
            back_val[0] = 0;
            prep = 0;
            pre = 0;
          }
        } else {
          ep = problem->get_invert(e);
          value_vector_back(e ,pre,prep,back_val);
          value_vector_back(ep,pre,prep,back_val);
          pre  = e;
          prep = ep;
        }
      }
    }
    void get_pre_back_val(CARP_INDIVIDUAL* P,CARP_INDIVIDUAL* S){
      get_pre_back_val(P);
      get_pre_back_val(S);
    } 
    inline void get_val(CARP_INDIVIDUAL *R) {
      int e=0,ev=0,inv;
      for (auto v: R->route) {
        if (v==0) continue;
        inv = problem->get_invert(v);
        pre_val[v] = problem->get_cost(v)+min(pre_val[e]+(*problem)(e,v),
            pre_val[ev]+(*problem)(ev,v));
        pre_val[inv] = problem->get_cost(v)+min(pre_val[e]
            +(*problem)(e,inv),pre_val[ev]+(*problem)(ev,inv));
        e = v;
        ev = inv;
      }
    }
    inline void get_val_back(CARP_INDIVIDUAL *R) {
      int e=0,ev=0,inv;
      for(auto it = R->route.rbegin(); it!= R->route.rend(); ++it) {
        v = *it;
        if (v==0) continue;
        inv = problem->get_invert(v);
        back_val[v] = problem->get_cost(v)+ min(back_val[e]
            +(*problem)(v,e),back_val[ev]+(*problem)(v,ev));
        back_val[inv] = problem->get_cost(v)+min(back_val[e]
            +(*problem)(inv,e),back_val[ev]+(*problem)(inv,ev));
        e = v;
        ev = inv;
      }
    }
    // VType get_part_pre_val(vector<int> R,int i,int j) {
		//   int u = R[i];
		//   int v = R[j];
		//   int duv = pre_val[v]-pre_val[u]+problem->get_cost(u);
    // } 
};

std::ostream &  operator<<(std::ostream & out, Move_base &m) {
  out<<"******* move ********"<<endl;
  show(m.R1->route);
  show(m.R2->route);
  out<<"costs : "<<m.costs<<endl;
  out<<"position from "<<m.position1<<" to "<<m.position2<<endl;
  out<<"task "<<m.task_i<<"  "<<m.task_j<<endl;
  out<<"over_load: "<<m.over_load<<endl;
  out<<"fitness: "<<m.fitness<<endl;
  out<<"change_mode: "<<m.change_mode<<endl;
  out<<"*********************"<<endl;
  return out;
}
#endif
