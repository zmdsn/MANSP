/*************************************************************************
 *  Name  : ../heuristic/path_scanning_ellipse.hpp
 *  Author: zmdsn
 *  Mail  : zmdsn@126.com 
 * Created Time: Sat 30 Jan 2021 05:56:33 PM CST
 * 
 ************************************************************************/
#ifndef  __PATH_SCANNING_ELLIPSE__HPP__
#define  __PATH_SCANNING_ELLIPSE__HPP__

#include<iostream>
using namespace std;

class PathScanningEllipse:public PathScanningRand{
	public:
		//*************** path_scanning ******************
		double alpha = 1.5;
		double average_demand;
		int get_nearest_tasks_ellipse_rule(int e);
		virtual int get_next(int e);
		PathScanningEllipse(){};
		PathScanningEllipse(CARP* P);
		PathScanningEllipse(CARP* p, DType Capacity);
		~PathScanningEllipse(){};
};

PathScanningEllipse::PathScanningEllipse(CARP* P):
	PathScanningRand(P){
		run_times = 100;
		// cout<<alpha<<" ";
		average_demand = 
			problem->total_demand/problem->task_number;
	};

int PathScanningEllipse::get_nearest_tasks_ellipse_rule(int e){
	vector<int> satisfied;
	VType cost;
	double average_cost = 
		problem->total_service_cost/problem->task_number;
	VType AD = average_cost+(*problem)(e,0);
	get_nearest_tasks(e);
	for (auto ei: near_tasks) {
		cost = (*problem)(e,ei)+(*problem)(ei,0); 
		cost+= problem->get_cost(ei);
		if(cost<AD){
			satisfied.push_back(e);	
		}
	}
	near_tasks = satisfied;
	return near_tasks.size();
}

int PathScanningEllipse::get_next(int e){
	if(Q-loaded>alpha*average_demand){
		get_nearest_tasks(e);
	} else {
		get_nearest_tasks_ellipse_rule(e);
	}
	if(near_tasks.size()>0) {
		return near_tasks[rand()%near_tasks.size()];
	} else {
		return -1;
	}
}

// void PathScanningEllipse::get_best_solution(vector<int>& task_list){
//   best_costs = INF;
//   for (int run=0; run<run_times; run++) {
//     // compare_run = run;
//     construct_candidate(task_list);
//     result.clear();
//     get_one_solution();
//     get_costs(result);
//     // cout<<run<<" => "<<costs<<endl;
//     if(costs<best_costs) {
//       best_costs = costs;
//       best_result = result;
//       // show(best_result);
//     }
//   }
// }

// void PathScanningEllipse::get_best_solution(){
//   best_costs = INF;
//   vector<int> candidate_bak;
//   construct_candidate();
//   candidate_bak = candidate;
//   get_best_solution(candidate_bak);
// }
#endif
