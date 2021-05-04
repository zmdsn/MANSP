/*************************************************************************
 *  Name  : move.hpp
 *  Author: zmdsn
 *  Mail  : zmdsn@126.com 
 * Created Time: 2020年11月18日 星期三 13时47分27秒
 * 
 ************************************************************************/
#ifndef  __InvRelocate__HPP__
#define  __InvRelocate__HPP__
#include<iostream>
#include "../move/move_base.hpp"
// 
using namespace std;
class InvRelocate:public Move_base{
  // (x,y) <==>  (u,v) 
 public:
	vector<int> P;
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
    inv_task_i = 0;
    is_legal= is_legal_move();
    // get_pre_back_val(R1,R2);
		// show(pre_val);
		// show(back_val);
    //cout<<position1<<" ti "<<task_i<<endl;
    // return is_feasible();
    return is_legal;
  }
  inline DType get_over_load() {
    DType temp_load = 0;
    over_load_changed = 0;
    temp_load = R1->loads - loadi; 
    over_load = problem->get_over_load(temp_load);
    temp_load = R2->loads+loadi; 
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
    // cout<<"is"<<(task_i!=0)<<not_be_special()<<p2_not_be_zero() <<endl;
    return is_legal;
  }
  VType get_move_costs() {
    costs = 0;
    if(!is_legal)return 0;
		if (R1!=R2) {
			costs = get_inv_insert_costs(R2,position2,task_i,change_mode);
			// cout<<costs<<"cs\n";
			costs += get_delete_costs(R1,position1);
			// cout<<costs<<"cs\n";
			costs -= R2->costs;
			// cout<<costs<<"cs\n";
			// cout<<change_mode<<"s\n";
		} else {
			// suit for R1==R2, which can't get the best value by O(1)
			costs = get_delete_costs(R1,position1);
			costs+= get_insert_costs(R2,position2,task_i);
			// costs = get_delete_costs(R1,position1);
			// costs+= get_insert_costs(R2,position2,task_i);
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
  bool move() {
    if(!is_legal)  return 0;
		// cout<<"moved***********";
		if (R1==R2) {
			relocate_same_route_move();
			// R2->get_best_in_sort(R2);
		} else {
			delete_task(R1->route,position1);
			P = R2->route;   
			int e = (change_mode&2)?task_ip:task_i;
			insert_task(P,position2,e);
			// cout<<task_i<<endl;
			//   show(R2->route);
			// show(P);
			R2->route = P;
			R2->get_best_in_sort(R2);
			if (P.size()!=R2->route.size()) {
				cout<<P.size();
				cout<<R2->route.size()<<endl;
				show(P);
				exit(0);
			}
		}
		R1->get_fitness();
		R2->get_fitness();
		return 1;
	}
	InvRelocate() {}
	InvRelocate(CARP_INDIVIDUAL *_R1,CARP_INDIVIDUAL *_R2,
			int _position1, int _position2):
		Move_base(_R1,_R2,_position1,_position2) {}
	InvRelocate(CARP_INDIVIDUAL *_R1,CARP_INDIVIDUAL *_R2):Move_base(_R1,_R2) {}
};
#endif
