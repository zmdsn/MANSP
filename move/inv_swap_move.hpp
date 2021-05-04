/*************************************************************************
 *  Name  : move.hpp
 *  Author: zmdsn
 *  Mail  : zmdsn@126.com 
 * Created Time: 2020年11月18日 星期三 13时47分27秒
 * 
 ************************************************************************/
#ifndef  __InvSwap__HPP__
#define  __InvSwap__HPP__
#include<iostream>
#include "../move/move_base.hpp"
// 
using namespace std;
class InvSwap:public Move_base{
  // (x,y) <==>  (u,v) 
 public:
	vector<int> P;
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
  inline bool prepare() {
		pre_val.resize(problem->task_number*2+1);
		back_val.resize(problem->task_number*2+1);
		get_val(R1);
		get_val_back(R1);
		if (R1!=R2) {
			get_val(R2);
			get_val_back(R2);
		}
    // R1->get_costs();
    // R2->get_costs();
    is_prepared = 1;
    return 1;
  }
	inline bool init_move() {
		if (!is_prepared) prepare();
		old_over_load = get_old_over_load();
		task_i  = R1->route[position1];
		loadi   = problem->get_demand(task_i);
		task_ip = problem->get_invert(task_i);
		// inv_task_i = 0;
		task_j  = R2->route[position2];
		loadj   = problem->get_demand(task_j);
		task_jp = problem->get_invert(task_j);
		// inv_task_j = 0;
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
	inline bool p2_not_be_zero() {
		return position2!=0;
	}
	inline bool not_be_special() {
		return (R1!=R2||!(R1==R2&&(position1==position2||position1==position2-1)));
		// return (R1!=R2);
  }
  inline bool is_legal_move() {
    is_legal = task_i!=0&&not_be_special()&&p2_not_be_zero();
		is_legal &= position2<R2->route.size()-1;
		is_legal &= R1!=R2;
    // cout<<"is"<<(task_i!=0)<<not_be_special()<<p2_not_be_zero() <<endl;
    return is_legal;
  }
	VType get_swap_costs(int _change_mode){
		int u = _change_mode&1?problem->get_invert(R1->route[position1-1]):R1->route[position1-1];
		int a = _change_mode&2?task_ip:task_i;
		int v = _change_mode&4?problem->get_invert(R1->route[position1+1]):R1->route[position1+1];
		int x = _change_mode&8?problem->get_invert(R2->route[position2-1]):R2->route[position2-1];
		int b = _change_mode&16?task_jp:task_j;
		int y = _change_mode&32?problem->get_invert(R2->route[position2+1]):R2->route[position2+1];
		VType cost = 0;
		cost = (*problem)(u,b)+(*problem)(b,v);
		cost+= pre_val[u]+back_val[v];
		cost+= problem->get_cost(b);
		// cout<<cost<<",";
		int as = cost;
		cost+= (*problem)(x,a)+(*problem)(a,y);
		cost+= pre_val[x]+back_val[y];
		cost+= problem->get_cost(a);
		// cout<<cost-as<<endl;
		return cost-R1->costs-R2->costs;
	}
	VType get_move_costs(){
		// cout<<"swap"<<endl;
		VType cost;
		costs = 0;
		if(!is_legal)  return 0;
		costs = INF;
		if (R1!=R2) {
			for (int i=1; i<=64; i++) {
				cost = get_swap_costs(i);
				if(cost<costs){
					costs = cost;
					change_mode = i;
					change_mode_to_task_inv(change_mode);
				}
			}
		} else {
			for (int i=1; i<=64; i++) {
				cost = get_swap_costs(i);
				if(cost<costs){
					costs = cost;
					change_mode = i;
					change_mode_to_task_inv(change_mode);
				}
			}
		}
		return costs;
	}
  bool relocate_same_route_move() {
		int e = task_i;
    // int e = inv_task_i?task_ip:task_i;
    //cout<<"m "<<e<<position1<<" "<<position2<<endl;
    if(position1>=position2) {
      delete_task(R1->route,position1);
      insert_task(R2->route,position2,e);
    } else{
      insert_task(R2->route,position2,e);
    //  show(R2->route);
      delete_task(R1->route,position1);
      //show(R1->route);
    }
		// cout<<"R2"<<endl;
		// show(R2->route);
		// R2->get_best_in_sort(R2);
		// show(R2->route);
    return 1;
  }
	inline void flip(vector<int> &R,int pos) {
		R[pos] = problem->get_invert(R[pos]);
	}
	bool move(){
		if(!is_legal)  return 0;
		// if(R1==R2&&position1==position2) {
		//   invert_task();
		//   return 1;
		// }
		// int task1 = inv_task_i?task_ip:task_i;
		// int task2 = inv_task_j?task_jp:task_j;
		replace_task(R2->route,position2,task_i);
		replace_task(R1->route,position1,task_j);
		R2->get_best_in_sort(R2);
		R1->get_best_in_sort(R1);
		R1->get_fitness();
		// cout<<"R1 "<<R1->costs<<endl;
		// cout<<"R2 "<<R2->costs<<endl;
		R2->get_fitness();
		return 1;
	}
	InvSwap() {}
	InvSwap(CARP_INDIVIDUAL *_R1,CARP_INDIVIDUAL *_R2,
			int _position1, int _position2):
		Move_base(_R1,_R2,_position1,_position2) {}
	InvSwap(CARP_INDIVIDUAL *_R1,CARP_INDIVIDUAL *_R2):Move_base(_R1,_R2) {}
};
#endif
