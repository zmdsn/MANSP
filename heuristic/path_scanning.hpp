#ifndef PathScanning_H_
#define  PathScanning_H_

#include<iostream>
#include<algorithm>
#include "../problem/CARP.hpp"
#include "../heuristic/heuristic.hpp"
using namespace std;

class PathScanning:public Heuristic{
	public:
		//*************** path_scanning ******************
		int eq;
		void get_best_solution(vector<int>& task_list);
		void get_best_solution();
		void get_one_solution();
		void construct_one_route();
		template<typename T>
			inline bool less_than(T a,T b);
		inline bool rule_1(int u,int ubar);
		inline bool rule_2(int u,int ubar);
		inline bool rule_3(int u,int ubar);
		inline bool rule_4(int u,int ubar);
		inline bool rule_5(int u,int ubar,DType loaded);
		bool better_than(int u,int ubar);
		virtual int get_next(int e);
		void show_all();
		PathScanning(){};
		PathScanning(CARP* P);
		PathScanning(CARP* p, DType Capacity);
		PathScanning(const PathScanning&);
		PathScanning& operator=(const PathScanning&);
		~PathScanning();
};

PathScanning::PathScanning(CARP* p): Heuristic(p){
	problem = p;
	Q = problem->Capacity;
	construct_candidate();
	run_times = 100;
}

PathScanning::PathScanning(CARP* p, DType Capacity) {
	problem = p;
	Q = Capacity;
	construct_candidate();
	run_times = 100;
}

PathScanning::~PathScanning(){}

template<typename T>
inline bool PathScanning::less_than(T a,T b){
	// cout<<eq<<" ";
	// if (a==b) {
	//   eq++;
	//   return rand()%eq==1;
	// } 
	// eq = 2;
	return a<b;
}

inline bool PathScanning::rule_1(int u,int ubar){
	VType cu,cubar,u0,ubar0;
	DType ru,rubar;
	cu = (*problem)[u].cost,cubar = (*problem)[u].cost;
	ru = (*problem)[u].demand,rubar = (*problem)[u].demand;
	return less_than(cubar*ru,cu*rubar);
}

inline bool PathScanning::rule_2(int u,int ubar){
	VType cu,cubar,u0,ubar0;
	DType ru,rubar;
	cu = (*problem)[u].cost,cubar = (*problem)[u].cost;
	ru = (*problem)[u].demand,rubar = (*problem)[u].demand;
	return less_than(cu*rubar,cubar*ru);
}

inline bool PathScanning::rule_3(int u,int ubar){
	VType u0       = (*problem)(u,0);
	VType ubar0    = (*problem)(ubar,0);
	// if (u0==ubar0) return rand()%2;
	// return  u0 > ubar0;
	return less_than(ubar0,u0);
}

inline bool PathScanning::rule_4(int u,int ubar){
	VType u0       = (*problem)(u,0);
	VType ubar0    = (*problem)(ubar,0);
	if (u0==ubar0) return rand()%2;
	// return  u0 < ubar0;
	return less_than(u0,ubar0);
}

inline bool PathScanning::rule_5(int u,int ubar,DType loaded){
	return (loaded < Q/2)?rule_3(u,ubar):rule_4(u,ubar);
}

bool PathScanning::better_than(int u,int ubar){
	// if u is better than ubar
	int is_end_depot = 1;
	bool res;
	switch(compare_rule){
		case 1:	
			res = rule_1(u,ubar);
			break;
		case 2: 
			res = rule_2(u,ubar);
			break;
		case 3: 
			res = rule_3(u,ubar);
			break;
		case 4: 
			res = rule_4(u,ubar);
			break;
		case 5: 
			res = rule_5(u,ubar,loaded);
			break;
	}
	return is_end_depot && res;
}

int PathScanning::get_next(int e){
	// get the next e
	int ubar=0;
	VType dbar = INF;
	VType distance;
	for(auto u:candidate){
		if(loaded + (*problem)[u].demand <= Q){
			distance = (*problem)(e,u);
			if(distance<dbar){
				dbar = distance;
				ubar = u;
			}else{
				if(dbar==distance && better_than(u,ubar) ){
					ubar = u;
				}
			}
		}
	}
	return ubar;
}

void PathScanning::get_best_solution(vector<int>& task_list){
	best_costs = INF;
	for (int rule=1; rule<6; rule++) {
		compare_rule = rule;
		construct_candidate(task_list);
		get_one_solution();
		get_costs(result);
		// cout<<rule<<" => "<<costs<<endl;
		if(costs<best_costs) {
			best_costs = costs;
			best_result = result;
			// show(best_result);
		}
	}
}

void PathScanning::get_best_solution(){
	best_costs = INF;
	vector<int> candidate_bak;
	construct_candidate();
	candidate_bak = candidate;
	get_best_solution(candidate_bak);
}

void PathScanning::construct_one_route(){
	// (e ubar)
	one_route.clear();
	one_route.push_back(0);
	int e = 0;
	int ubar;
	loaded = 0;
	while(!candidate.empty()){
		ubar = get_next(e);
		if (ubar>0) {
			one_route.push_back(ubar);
			loaded += (*problem)[ubar].demand;
			e = ubar;
			delete_candidate(ubar);
		} else {
			one_route.push_back(0);
			return;
			 // break;
		}
	}
	one_route.push_back(0);
}

void PathScanning::get_one_solution(){
	// eq = 2;
	// cout<<"sss"<<endl;
	// show(candidate);
	shuffle_candidate();
	// show(candidate);
	result.clear();
	while(!candidate.empty()){
		construct_one_route();
		result.insert(result.end(),one_route.begin(),one_route.end()-1);
	}
	result.push_back(0);
	// show(result);
}

#endif
