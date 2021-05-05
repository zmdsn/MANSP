#ifndef CYCLE_H_
#define  CYCLE_H_

#include<iostream>
using namespace std;

class Cycle{
 public:
	CARP* problem;
	vector<int> cycle;
	VType total_costs;
	VType serve_costs;
	//VType costs;
	DType demands;
	int position;
	bool is_invert;
	int ei;
	void get_cost_demand(){
		total_costs = 0;
		demands=0;
		serve_costs = 0;
		int pre = 0;
		for (auto e:cycle){			
			serve_costs  += problem->get_cost(e);
			total_costs  += (*problem)(pre,e);
			pre = e;
			demands+= problem->get_demand(e);
		}
		total_costs += (*problem)(cycle.back(),0);
		total_costs += serve_costs;
	}
	void init(){
		get_cost_demand();
	}
	bool Clear() {
		cycle.clear();
		total_costs = 0;
	  demands = 0;
	  return 1;
	}
	bool Insert() {
		cycle.insert(cycle.begin()+position,ei);
		// cout<<demands;
		demands += problem->get_demand(ei);
				// cout<<demands;

		return 1;
	}
	bool In_path(int e,int i){
		if (i==0) {
			return (*problem)(0,cycle[0]) == (*problem)(0,e)+(*problem)(e,cycle[0])+problem->get_cost(e);
		}
		if (i==cycle.size()) {
			return (*problem)(cycle[i-1],0) == (*problem)(cycle[i-1],e)+(*problem)(e,0)+problem->get_cost(e);
		}
		return (*problem)(cycle[i-1],cycle[i]) == (*problem)(cycle[i-1],e)+(*problem)(e,cycle[i])+problem->get_cost(e);
	}
	bool Have(const Cycle& C) {
		if (C.cycle.size()!=1) {
			return 0;
		}	 
		if (C.demands+demands>problem->Capacity) {
			return 0;
		}	
		int e = C.cycle[0];
		for (int i = 0; i <= cycle.size(); ++i)
		{
			if (In_path(e,i) ){
				position = i;
				is_invert = 0;
				ei = e;
				return 1;
			}
			if (In_path(problem->get_invert(e),i)){
				position = i;
				is_invert = 1;
				ei = problem->get_invert(e);
				return 1;
			}
		}
		return 0;
	}
	bool Have1(const Cycle& C) {
		if (C.cycle.size()!=1) {
			return 0;
		}	 
		if (C.demands+demands>problem->Capacity) {
			return 0;
		}	
		int e = C.cycle[0];
		if (In_path(e,0) ){
			position = 0;
			is_invert = 0;
			ei = e;
			return 1;
		}
		if (In_path(problem->get_invert(e),0)){
			position = 0;
			is_invert = 1;
			ei = problem->get_invert(e);
			return 1;
		}
		if (In_path(e,cycle.size()) ){
			position = cycle.size();
			is_invert = 0;
			ei = e;
			return 1;
		}
		if (In_path(problem->get_invert(e),cycle.size())){
			position = cycle.size();
			is_invert = 1;
			ei = problem->get_invert(e);
			return 1;
		}
		return 0;
	}
	bool zero_connected(const Cycle& C) { 
		if (C.demands+demands>problem->Capacity) {
			return 0;
		}
		if (C.cycle.size()!=1) {
			return 0;
		}	
		int u,v,x,y,yp,xp;
		u = cycle[0];
		v = cycle.back();
		x = C.cycle[0];
		y = C.cycle.back();
		xp = problem->get_invert(x);
		yp = problem->get_invert(y);
		int e = C.cycle[0];
		int ep= problem->get_invert(e);
		if ((*problem)(v,x) == 0) {
			position  = cycle.size();
			is_invert = 0;
			ei = e;
			return 1;
		}
		if ((*problem)(y,u) == 0) {
			position  = 0;
			is_invert = 0;
			ei = e;
			return 1;
		}
		if ((*problem)(v,yp) == 0) {
			position  = cycle.size();
			is_invert = 1;
			ei = ep;
			return 1;
		}
		if ((*problem)(xp,u) == 0) {
			position  = 0;
			is_invert = 1;
			ei = ep;
			return 1;
		}
		return 0;
	}
	bool operator<(const Cycle& C) {
	 return total_costs-serve_costs > C.total_costs-C.serve_costs;
	}
	void show() {
		for(auto c:cycle) {
			cout<<c<<" ";
		}
		cout<<endl<<"costs :"<<total_costs<<" ";
		cout<<"Capacity:"<<demands<<endl;
	}
	Cycle(CARP* P,vector<int>& _cycle):
		problem(P),cycle(_cycle){
			init();
	}
	Cycle(){};
	~Cycle(){};
};
#endif
