/*************************************************************************
 *  Name  : ../heuristic/PathScanningRandSP.hpp
 *  Author: zmdsn
 *  Mail  : zmdsn@126.com 
 * Created Time: Sat 30 Jan 2021 05:56:33 PM CST
 * 
 ************************************************************************/
#ifndef  __PathScanningRandSP__HPP__
#define  __PathScanningRandSP__HPP__

#include<iostream>
using namespace std;

class PathScanningRandSP:public PathScanningRand{
	public:
		//*************** path_scanning ******************
		// void get_best_solution(vector<int>& task_list);
		// void get_best_solution();
		double lambda = 1.0;
		virtual int get_next(int e);
		VType penalty_distance(int e,int u);
		void get_min_smooth_next(int e);
		void get_min_smooth_next_0(int e);
		PathScanningRandSP(){};
		PathScanningRandSP(CARP* P);
		PathScanningRandSP(CARP* p, DType Capacity);
		~PathScanningRandSP(){};
};

PathScanningRandSP::PathScanningRandSP(CARP* P):
	PathScanningRand(P){
		run_times = 100;
		P->get_average_demand(); 
	};

VType PathScanningRandSP::penalty_distance(int e,int u) {
	VType distance;
	distance = (*problem)(e,u);
	distance += lambda*(distance-
			problem->get_smooth_link_costs(e,u));
	return distance;
}

void PathScanningRandSP::get_min_smooth_next(int e){
	VType dbar = INF;
	VType distance;
	near_tasks.clear();
	for(auto u:candidate){
		if(loaded + (*problem)[u].demand <= Q){
			distance = penalty_distance(e,u);
			if(distance<dbar){
				dbar = distance;
				near_tasks.clear();
				near_tasks.push_back(u);
			} else if (distance == dbar){
				near_tasks.push_back(u);
			}
		}
	}
}

void PathScanningRandSP::get_min_smooth_next_0(int e) {
	VType dbar = INF;
	VType distance;
	near_tasks.clear();
	for(auto u:candidate){
		if(loaded + (*problem)[u].demand <= Q){
			distance = penalty_distance(e,u);
			distance = penalty_distance(u,0);
			if(distance<dbar){
				dbar = distance;
				near_tasks.clear();
				near_tasks.push_back(u);
			} else if (distance == dbar){
				near_tasks.push_back(u);
			}
		}
	}
}

int PathScanningRandSP::get_next(int e){
	// get_nearest_tasks(e);
	if (Q-loaded<=problem->average_demand) {
		get_min_smooth_next_0(e);
	} else {
		get_min_smooth_next(e);
	}
	if(near_tasks.size()>0) {
		return near_tasks[rand()%near_tasks.size()];
	} else {
		return -1;
	}
}
#endif
