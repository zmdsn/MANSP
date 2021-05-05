#ifndef TreeSplit_H_
#define  TreeSplit_H_

#include<iostream>
#include<algorithm>
#include<utility>
#include "../problem/CARP.hpp"
#include "../heuristic/heuristic.hpp"
using namespace std;

class TreeSplit:public Heuristic{
	public:
		//*************** path_scanning ******************
		int eq;
		void get_best_solution(vector<int>& task_list);
		void get_best_solution();
		void get_one_solution();
		void construct_one_route();
		vector<int> tree_tasks;
		vector<pair<int,int> > tree_link;
		int get_next(int e);
		int get_next();
		// int get_next();
		pair<int,int> get_next();
		void construct_one_tree();
		void show_all();
		TreeSplit(){};
		TreeSplit(CARP* P);
		TreeSplit(CARP* p, DType Capacity);
		TreeSplit(const TreeSplit&);
		TreeSplit& operator=(const TreeSplit&);
		~TreeSplit();
		VType dbar = INF;
		void add_node_nearest_tasks(int node){
			// VType dbar = INF;
			VType distance;
			for(auto u:candidate){
				if(loaded + (*problem)[u].demand <= Q){
					// distance = (*problem)(node,u);
					distance = problem->get_distance_node_e(node,u);
					if(distance<dbar){
						dbar = distance;
						near_tasks.clear();
						near_tasks.push_back(u);
					}else if(distance == dbar){
						near_tasks.push_back(u);
					}
				}
			}
			// show(near_tasks);
		}
};

TreeSplit::TreeSplit(CARP* p): Heuristic(p){
	problem = p;
	Q = problem->Capacity;
	construct_candidate();
	run_times = 100;
}

TreeSplit::TreeSplit(CARP* p, DType Capacity) {
	problem = p;
	Q = Capacity;
	construct_candidate();
	run_times = 100;
}

TreeSplit::~TreeSplit(){}

int TreeSplit::get_next(int u){
	// get the next e
	VType dbar = INF;
	near_tasks.clear();
	for (u in tree_nodes ) {
		add_node_nearest_tasks(u);
	}
	return ubar;
}

pair<int,int> TreeSplit::get_next(){
	// get the next e
	int ubar=0;
	VType dbar = INF;
	VType distance;
	vector<pair<int,int> > nexts;
	nexts.clear();
	for(auto e:tree_tasks){
		for(auto u:candidate){
			distance = (*problem)(e,u);
			if(distance<dbar){
				dbar = distance;
				nexts.clear();
				nexts.push_back(make_pair(e,u));
			}else{
				if(dbar==distance){
					nexts.push_back(make_pair(e,u));
				}
			}
		}
	}
	// cout<<nexts.size()<<endl;
	if(nexts.size()>0) {
		return nexts[rand()%nexts.size()];
	} else {
		return make_pair(-1,-1);
	}
}

void TreeSplit::get_best_solution(vector<int>& task_list){
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

void TreeSplit::get_best_solution(){
	best_costs = INF;
	vector<int> candidate_bak;
	construct_candidate();
	candidate_bak = candidate;
	get_best_solution(candidate_bak);
}

void TreeSplit::construct_one_route(){
	// (e ubar)
	construct_one_tree();
	one_route.clear();
	one_route.push_back(0);
	int e = 0;
	int ubar;
	loaded = 0;
	while(!tree_tasks.empty()){
		ubar = get_next(e);
		cout<<ubar<<" ";
		one_route.push_back(ubar);
	}
	one_route.push_back(0);
}

void TreeSplit::construct_one_tree(){
	// (e ubar)
	one_route.clear();
	one_route.push_back(0);
	tree_tasks.push_back(0);
	pair<int,int> next_e;
	int e = 0;
	int ubar;
	loaded = 0;
	while(!candidate.empty()){
		next_e = get_next();
		ubar = next_e.second;
		cout<<ubar<<endl;
		if (ubar>0) {
			tree_tasks.push_back(ubar);
			tree_link.push_back(next_e);
			delete_candidate(ubar);
		}  else {
			break;
		}
	}
	// show(tree_tasks);
	// one_route.push_back(0);
}

void TreeSplit::get_one_solution(){
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
