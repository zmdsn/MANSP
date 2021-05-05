/*************************************************************************
 *  Name  : ../heuristic/path_scanning_ellipse_SP.hpp
 *  Author: zmdsn
 *  Mail  : zmdsn@126.com 
 * Created Time: Sat 30 Jan 2021 05:56:33 PM CST
 * 
 ************************************************************************/
#ifndef  __PATH_SCANNING_ELLIPSE_SP__HPP__
#define  __PATH_SCANNING_ELLIPSE_SP__HPP__

#include<iostream>
using namespace std;

class PathScanningEllipseSP:public PathScanningRandSP{
	public:
		//*************** path_scanning ******************
		double alpha = 1.5;
		double average_demand;
		int get_nearest_tasks_ellipse_SP_rule(int e);
		virtual int get_next(int e);
		PathScanningEllipseSP(){};
		PathScanningEllipseSP(CARP* P);
		PathScanningEllipseSP(CARP* p, DType Capacity);
		~PathScanningEllipseSP(){};
};

PathScanningEllipseSP::PathScanningEllipseSP(CARP* P):
	PathScanningRandSP(P){
		run_times = 100;
		// cout<<alpha<<" ";
		average_demand = 
			problem->total_demand/problem->task_number;
	};

int PathScanningEllipseSP::get_nearest_tasks_ellipse_SP_rule(int e){
	vector<int> satisfied;
	VType cost;
	double average_cost = 
		problem->total_service_cost/problem->task_number;
	VType AD = average_cost+(*problem)(e,0);
	get_nearest_tasks(e);
	for (auto ei: near_tasks) {
		cost = penalty_distance(e,ei);
		cost += penalty_distance(ei,0);
		// cost = (*problem)(e,ei)+(*problem)(ei,0); 
		// cost+= problem->get_cost(ei);
		if(cost<AD){
			satisfied.push_back(e);	
		}
	}
	near_tasks = satisfied;
	return 1;
}

int PathScanningEllipseSP::get_next(int e){
	if(Q-loaded>alpha*average_demand){
		// get_nearest_tasks(e);
		get_min_smooth_next(e);
	} else {
		get_nearest_tasks_ellipse_SP_rule(e);
	}
	if(near_tasks.size()>0) {
		return near_tasks[rand()%near_tasks.size()];
	} else {
		return -1;
	}
}
#endif
