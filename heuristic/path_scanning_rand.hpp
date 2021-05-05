/*************************************************************************
 *  Name  : ../heuristic/path_scanning_rand.hpp
 *  Author: zmdsn
 *  Mail  : zmdsn@126.com 
 * Created Time: Sat 30 Jan 2021 05:56:33 PM CST
 * 
 ************************************************************************/
#ifndef  __PATH_SCANNING_RAND__HPP__
#define  __PATH_SCANNING_RAND__HPP__

#include<iostream>
using namespace std;

class PathScanningRand:public PathScanning{
	public:
		//*************** path_scanning ******************
		void get_best_solution(vector<int>& task_list);
		void get_best_solution();
		virtual int get_next(int e);
		PathScanningRand(){};
		PathScanningRand(CARP* P);
		PathScanningRand(CARP* p, DType Capacity);
		~PathScanningRand(){};
};

PathScanningRand::PathScanningRand(CARP* P):
	PathScanning(P){
		run_times = 100;
	};

int PathScanningRand::get_next(int e){
	get_nearest_tasks(e);
	if(near_tasks.size()>0) {
		return near_tasks[rand()%near_tasks.size()];
	} else {
		return -1;
	}
}

void PathScanningRand::get_best_solution(vector<int>& task_list){
	best_costs = INF;
	for (int run=0; run<run_times; run++) {
		// compare_run = run;
		construct_candidate(task_list);
		result.clear();
		get_one_solution();
		get_costs(result);
		// cout<<run<<" => "<<costs<<endl;
		if(costs<best_costs) {
			best_costs = costs;
			best_result = result;
			// show(best_result);
		}
	}
}

void PathScanningRand::get_best_solution(){
	best_costs = INF;
	vector<int> candidate_bak;
	construct_candidate();
	candidate_bak = candidate;
	get_best_solution(candidate_bak);
}
#endif
