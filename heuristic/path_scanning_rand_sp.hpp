/*************************************************************************
 *  Name  : ../heuristic/Path_scanning_rand_SP.hpp
 *  Author: zmdsn
 *  Mail  : zmdsn@126.com 
 * Created Time: Sat 30 Jan 2021 05:56:33 PM CST
 * 
 ************************************************************************/
#ifndef  __Path_scanning_rand_SP__HPP__
#define  __Path_scanning_rand_SP__HPP__

#include<iostream>
using namespace std;

class Path_scanning_rand_SP:public Path_scanning_rand{
	public:
		//*************** path_scanning ******************
		// void get_best_solution(vector<int>& task_list);
		// void get_best_solution();
		double lambda = 1.0;
		virtual int get_next(int e);
		VType penalty_distance(int e,int u);
		void get_min_smooth_next(int e);
		void get_min_smooth_next_0(int e);
		Path_scanning_rand_SP(){};
		Path_scanning_rand_SP(CARP* P);
		Path_scanning_rand_SP(CARP* p, DType Capacity);
		~Path_scanning_rand_SP(){};
};

Path_scanning_rand_SP::Path_scanning_rand_SP(CARP* P):
	Path_scanning_rand(P){
		run_times = 100;
		P->get_average_demand(); 
	};

VType Path_scanning_rand_SP::penalty_distance(int e,int u) {
	VType distance;
	distance = (*problem)(e,u);
	distance += lambda*problem->get_smooth_link_costs(e,u);
	return distance;
}

void Path_scanning_rand_SP::get_min_smooth_next(int e){
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

void Path_scanning_rand_SP::get_min_smooth_next_0(int e){
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

int Path_scanning_rand_SP::get_next(int e){
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
