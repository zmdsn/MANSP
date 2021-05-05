#ifndef MERGE_H_
#define  MERGE_H_

#include<iostream>
#include "../heuristic/cycle.hpp"

using namespace std;

class Merge{
 public:
 	CARP* problem;
 	Cycle* C1;
 	Cycle* C2;
 	VType saving_costs;
 	int merge_mode;
 	bool IsFeasible(){
 		// cout<<C1->demands<<C2->demands<<endl;
 		return C1->demands+C2->demands<=problem->Capacity;
	}
 	void getSavingCosts(){
 		VType savings = 0;
 		saving_costs = 0;
 		int u,v,x,y,yp,up;
 		u = C1->cycle.front();
 		v = C1->cycle.back();
 		x = C2->cycle.front();
 		y = C2->cycle.back();
 		up = problem->get_invert(u);
 		yp = problem->get_invert(y);
 		int e,s;
 		DType will_load;
 		for (int i = 0; i < 4; ++i)	{
 			e = i/2?v:up;
 			s = i%2?x:yp;
 			// 0 => up,yp
 			// 1 => up,x
 			// 2 => v,yp
 			// 3 => v,x
			savings = (*problem)(e,0) + (*problem)(0,s) - (*problem)(e,s);
			// cout<<savings<<" ";
 			will_load = C1->demands + C2->demands;
			savings+= 30*will_load/problem->Capacity;
			// cout<<savings<<endl;
			// cout<<e<<s<<" "<<savings<<endl;
	 		if (savings>saving_costs) {
	 			saving_costs = savings;
	 			merge_mode = i;
	 		}
 		}
 	}
 	void MergeTwoCycle() {
 		VType olds = C1->total_costs+C2->total_costs;
 		switch (merge_mode) {
 			case 0: // reverse then append reverse => insert to the first
 				C1->cycle.insert(C1->cycle.begin(),C2->cycle.begin(),C2->cycle.end());
 				break;
 			case 1: // reverse then append => insert reverse to the first
 				for (int i = 0; i<C2->cycle.size(); -++i) {
 					C1->cycle.insert(C1->cycle.begin(),problem->get_invert(C2->cycle[i]));
 				}
 				break;
 			case 2: // append reverse 
 				for (int i = C2->cycle.size()-1; i>=0; --i) {
 					C1->cycle.push_back(problem->get_invert(C2->cycle[i]));
 				}
 				break;
 			case 3: // insert directly
 				C1->cycle.insert(C1->cycle.end(),C2->cycle.begin(),C2->cycle.end());
 				break;
 		}
 		C1->get_cost_demand();
		// C1->demands += C2->demands;
		// C1->total_costs += C2->total_costs;
		// C1->total_costs -= saving_costs;
		// C1->total_costs -= saving_costs;
		// if(olds-saving_costs!=C1->total_costs){
	 // 		cout<<"merge_mode:"<<merge_mode<<" costs "<<C1->total_costs+C2->total_costs<<" => ";
		// 	cout<<C1->total_costs<<" saving_costs "<<saving_costs<<endl;
		// 	exit(0);
		// }
		C2->Clear();
 	}
 	void show() {
 		cout<<"***** merge: *****\n";
 		C1->show();
 		C2->show();
 		cout<<"merge_mode:"<<merge_mode<<" ";
 		cout<<"saving_costs :"<<saving_costs<<endl;
 	}
 	Merge(CARP* problem,Cycle* C1,Cycle* C2):
 	problem(problem),C1(C1),C2(C2){
 	}
 	Merge(CARP* problem):
 	problem(problem){
 	}
 	bool operator<(const Merge& C) {
	 return saving_costs>C.saving_costs;
	}
 	~Merge(){};
};

#endif
