/*************************************************************************
 *  Name  : ../heuristic/partical_reconstruct.hpp
 *  Author: zmdsn
 *  Mail  : zmdsn@126.com 
 * Created Time: Sun 31 Jan 2021 05:19:04 PM CST
 * 
 ************************************************************************/
#ifndef  __PartialReconstruct__HPP__
#define  __PartialReconstruct__HPP__

#include<iostream>
#include "../problem/CARP.hpp"
#include "../heuristic/heuristic.hpp"
using namespace std;

class PartialReconstruct:public HEURISTIC {
	int kernel=1;
	void reconstruct();
	PartialReconstruct(CARP* P);
	~PartialReconstruct(){};
}

PartialReconstruct::PartialReconstruct(CARP* P):
	HEURISTIC(P) {

	}

void ByPS::reconstruct(vector<int>& old_routes,
		vector<int>& new_routes){
	Path_scanning_ellipse PS(&T);
	PS.get_best_solution(old_routes);
	new_routes = PS.best_result;
}

void ByAM::reconstruct(vector<int>& old_routes,
		vector<int>& new_routes){
	AugmentMerge AM(&T);
	AM.get_best_solution(old_routes);
	new_routes = AM.best_result;
}

void PartialReconstruct::reconstruct(vector<int>& old_routes){
	VType old_val,new_val;
	old_val = get_costs(old_routes);
	vector<int>& new_routes;
	switch(kernel) {
		case 1:
			ByAM();break;
		default:
			ByPS();break;
	}
	new_val = get_costs(new_routes);
	if (new_val<old_val) {
		old_routes = new_routes;
	}
}

#endif
