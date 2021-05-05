#ifndef HEURISTIC_H_
#define  HEURISTIC_H_

#include<iostream>
#include "../heuristic/merge.hpp"
#include "../heuristic/cycle.hpp"
#include "heuristic.hpp"
#include<algorithm>
using namespace std;

class HEURISTIC{
	public:
		CARP* problem;
		vector<int> candidate;
		vector<int> result;
		vector<int> best_result;
		vector<int> has_mark;
		vector<int> near_tasks;
		int compare_rule;
		DType Q;
		int run_times;
		VType costs;
		VType best_costs;
		inline void set_problem(CARP* P){problem=P;Q=P->Capacity;};
		inline void set_capacity(VType Capacity){Q=Capacity;};
		void get_costs(vector<int>& route);
		void get_costs();
		//*************** path_scanning ******************
		void construct_candidate(vector<int>& task_list);
		void construct_candidate();
		void delete_candidate(int tag);
		void path_scanning(vector<int>& task_list);
		void path_scanning();
		void path_scanning_one();
		bool better_than(int u,int ubar);
		int get_next(int e);
		void show_all();
		void path_scanning_ellipses(vector<int>& task_list, int runs);
		void path_scanning_ellipses(vector<int>& task_list);
		void path_scanning_ellipses(int runs);
		void path_scanning_ellipses();
		void path_scanning_ellipses_one();
		void path_scanning_rand(vector<int>& task_list);
		void path_scanning_rand(vector<int>& task_list, int runs);
		void path_scanning_rand();
		void path_scanning_rand(int runs);
		void path_scanning_rand_one();
		void path_scanning_rand_criterion(vector<int>& task_list);
		void path_scanning_rand_criterion(vector<int>& task_list, int runs);
		void path_scanning_rand_criterion();
		void path_scanning_rand_criterion(int runs);
		void path_scanning_rand_criterion_one();
		void ellipse_rule(int e);
		void get_nearest_tasks(int e);
		//*************** augment merge ******************
		void construct_candidate_AM(vector<int>& task_list);
		void construct_candidate_AM();
		void augment_merge_rand();
		void augment_merge(vector<int>& task_list);
		void augment_merge();
		void augment_merge(vector<int>& task_list,int run_times);
		void augment_merge(int run_times);
		void init_cycles(vector<Cycle>& cycles);
		void augment(vector<Cycle>& cycles);
		void zero_augment(vector<Cycle>& cycles);
		void sort_and_resize(vector<Cycle>& cycles);
		void get_best_merge(vector<Cycle>& cycles,Merge& merge_best);
		void cycles_to_result(vector<Cycle>& cycles);
		void merge_cycles(vector<Cycle>& cycles);
		void get_best_merge_vector(vector<Cycle>& cycles,vector<Merge>& merge_vector);
		void merge_cycles_best(vector<Cycle>& cycles);
		DType loaded;
		HEURISTIC();
		HEURISTIC(CARP* P);
		HEURISTIC(CARP* p, DType Capacity);
		HEURISTIC(const HEURISTIC&);
		HEURISTIC& operator=(const HEURISTIC&);
		~HEURISTIC();
};

HEURISTIC::HEURISTIC() {
}

HEURISTIC::HEURISTIC(CARP* p) {
	problem = p;
	Q = problem->Capacity;
	construct_candidate();
	run_times = 100;
}

HEURISTIC::HEURISTIC(CARP* p, DType Capacity) {
	problem = p;
	Q = Capacity;
	construct_candidate();
	run_times = 100;
}

HEURISTIC::~HEURISTIC(){}

void HEURISTIC::show_all(){
	cout<<"candidate list:"<<endl;
	show(candidate);
	show(result);
}

void HEURISTIC::construct_candidate(vector<int>& task_list){
	candidate.clear();
	has_mark.clear();
	for (int i=0; i<=2*problem->task_number; i++) {
		has_mark.push_back(0);
	}
	for (auto e: task_list) {
		if(e==0) continue;
		if(has_mark[e] == 0){
			candidate.push_back(e);
			candidate.push_back(problem->get_invert(e));
			has_mark[e] = 1;
			has_mark[problem->get_invert(e)] = 1;
		}
	}
	// show(has_mark);
}

void HEURISTIC::construct_candidate(){
	candidate.clear();
	for (int i=0; i<problem->task_number*2; i++) {
		candidate.push_back(i+1);
	}
}

void HEURISTIC::delete_candidate(int e){
	candidate.erase(find(candidate.begin(), 
				candidate.end(),e));
	candidate.erase(find(candidate.begin(), 
				candidate.end(),(*problem)[e].invert));
}

bool HEURISTIC::better_than(int u,int ubar){
	// if u is better than ubar
	int is_end_depot = 1;
	bool res;
	VType cu,cubar,u0,ubar0;
	DType ru,rubar;
	u0       = (*problem)(u,0);
	ubar0    = (*problem)(ubar,0);
	// avoid come to depot
	if(u0==0)    is_end_depot = 0;
	if(ubar0==0) is_end_depot = 1;
	if(ubar0==0&&u0!=0) return 1;	
	switch(compare_rule){
		case 1:	
		    cu = (*problem)[u].cost   ,cubar = (*problem)[u].cost;
		    ru = (*problem)[u].demand ,rubar = (*problem)[u].demand;
				res = (double)cu/ru > (double)cubar/rubar;
				// cout<<res<<" *1\n";
			break;
		case 2: 
		    cu = (*problem)[u].cost   ,cubar = (*problem)[u].cost;
		    ru = (*problem)[u].demand ,rubar = (*problem)[u].demand;
		    res = (double)cu/ru < (double)cubar/rubar;
		    break;
		case 3: 
			res = u0 > ubar0;
			break;
		case 4: 
			res = u0 < ubar0;
			break;
		case 5: 
			res = (loaded < Q/2)?(u0 > ubar0) : (u0 < ubar0);
			break;
	}
	// cout<<res<<"*\n";
	return is_end_depot && res;
}

int HEURISTIC::get_next(int e){
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

void HEURISTIC::path_scanning(vector<int>& task_list){
	best_costs = INF;
	for (int rule=1; rule<6; rule++) {
		compare_rule = rule;
		construct_candidate(task_list);
		path_scanning_one();
		get_costs(result);
		// cout<<rule<<" => "<<costs<<endl;
		if(costs<best_costs) {
			best_costs = costs;
			best_result = result;
			// show(best_result);
		}
	}
}

void HEURISTIC::path_scanning(){
	best_costs = INF;
	for (int rule=1; rule<6; rule++) {
		compare_rule = rule;
		construct_candidate();
		path_scanning_one();
		get_costs(result);
		// cout<<rule<<" => "<<costs<<endl;
		if(costs<best_costs) {
			best_costs = costs;
			best_result = result;
			// show(best_result);
		}
	}
}

void HEURISTIC::path_scanning_one(){
	if(candidate.size()==0) {
		construct_candidate();
	}
	result.clear();
	result.push_back(0);
	int e = 0;
	int ubar;
	loaded = 0;
	while(!candidate.empty()){
		ubar = get_next(e);
		if(ubar>0){
			result.push_back(ubar);
			loaded += (*problem)[ubar].demand;
			e = ubar;
			delete_candidate(ubar);
		} else {
			result.push_back(0);
			e = 0;
			loaded = 0;
		}
	}
	result.push_back(0);
}

void HEURISTIC::get_nearest_tasks(int e){
	// get the next e
	// int ubar=0;
	VType dbar = INF;
	VType distance;
	near_tasks.clear();
	for(auto u:candidate){
		if(loaded + (*problem)[u].demand <= Q){
			distance = (*problem)(e,u);
			if(distance<dbar){
				dbar = distance;
				near_tasks.clear();
				near_tasks.push_back(u);
			}else if(distance == dbar){
				near_tasks.push_back(u);
			}
		}
	}
	// return ubar;
}

void HEURISTIC::ellipse_rule(int e){
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
}

void HEURISTIC::path_scanning_ellipses(vector<int>& task_list){
	path_scanning_ellipses(task_list,run_times);
}

void HEURISTIC::path_scanning_ellipses(vector<int>& task_list, int runs){
	best_costs = INF;
	for (int rule=0; rule<runs; rule++) {
		construct_candidate(task_list);
		path_scanning_ellipses_one();
		get_costs(result);
		// cout<<rule<<" => "<<costs<<endl;
		if(costs<best_costs) {
			best_costs = costs;
			best_result = result;
			// show(best_result);
		}
	}
}

void HEURISTIC::path_scanning_ellipses(int runs){
	best_costs = INF;
	for (int rule=0; rule<runs; rule++) {
		construct_candidate();
		path_scanning_ellipses_one();
		get_costs(result);
		// cout<<rule<<" => "<<costs<<endl;
		if(costs<best_costs) {
			best_costs = costs;
			best_result = result;
			// show(best_result);
		}
	}
}

void HEURISTIC::path_scanning_ellipses(){
	path_scanning_ellipses(run_times);
}

void HEURISTIC::path_scanning_ellipses_one(){
	// [1]L. Santos, J. Coutinho-Rodrigues, and J. R. Current, “An improved heuristic for the capacitated arc routing problem,” Computers & Operations Research, vol. 36, no. 9, pp. 2632–2637, Sep. 2009.
	if(candidate.size()==0) {
		construct_candidate();
	}
	result.clear();
	result.push_back(0);
	int e = 0;
	int ubar;
	loaded = 0;
	DType remaining_vehicle_capacity;
	remaining_vehicle_capacity = Q;
	double alpha = 0.5;
	double average_demand = problem->total_demand/problem->task_number;
	while(!candidate.empty()){
		if(remaining_vehicle_capacity>alpha*average_demand){
			get_nearest_tasks(e);
		} else {
			ellipse_rule(e);
		}
		if(near_tasks.size()>0){
			if(near_tasks.size()==1){
				ubar = near_tasks[0];
			} else{
				ubar = near_tasks[rand()%near_tasks.size()];
			}
			result.push_back(ubar);
			loaded += problem->get_demand(ubar);
			e = ubar;
			delete_candidate(ubar);
		} else {
			result.push_back(0);
			e = 0;
			loaded = 0;
		}
	}
	result.push_back(0);
}

void HEURISTIC::get_costs(vector<int> &route){
	costs = 0;
	int i;
	for (i=0; i<route.size()-1; i++) {
		costs += (*problem)(route[i],route[i+1]);
		costs += problem->get_cost(route[i]);
	}
	costs += problem->get_cost(route[i]);
}

void HEURISTIC::get_costs(){
	get_costs(result);
}

void HEURISTIC::path_scanning_rand(vector<int>& task_list){
	path_scanning_rand(task_list, run_times);
}

void HEURISTIC::path_scanning_rand(vector<int>& task_list, int runs){
	best_costs = INF;
	for (int run=0; run<runs; run++) {
		construct_candidate(task_list);
		path_scanning_rand_one();
		get_costs(result);
		// cout<<run<<" => "<<(costs<best_costs)<<costs<<endl;
		if(costs<best_costs) {
			best_costs = costs;
			best_result = result;
			// show(best_result);
		}
	}
}

void HEURISTIC::path_scanning_rand(int runs){
	best_costs = INF;
	for (int rule=0; rule<runs; rule++) {
		construct_candidate();
		path_scanning_rand_one();
		get_costs(result);
		// cout<<rule<<" => "<<costs<<endl;
		if(costs<best_costs) {
			best_costs = costs;
			best_result = result;
			// show(best_result);
		}
	}
}

void HEURISTIC::path_scanning_rand(){
	path_scanning_rand(run_times);
}

void HEURISTIC::path_scanning_rand_one(){
	if(candidate.size()==0) {
		construct_candidate();
	}
	result.clear();
	result.push_back(0);
	int e = 0;
	int ubar;
	loaded = 0;
	DType remaining_vehicle_capacity;
	remaining_vehicle_capacity = Q;
	while(!candidate.empty()){
		get_nearest_tasks(e);
		if(near_tasks.size()>0){
			if(near_tasks.size()==1){
				ubar = near_tasks[0];
			} else{
				ubar = near_tasks[rand()%near_tasks.size()];
			}
			result.push_back(ubar);
			loaded += problem->get_demand(ubar);
			e = ubar;
			delete_candidate(ubar);
		} else {
			result.push_back(0);
			e = 0;
			loaded = 0;
		}
	}
	result.push_back(0);
}

void HEURISTIC::path_scanning_rand_criterion(vector<int>& task_list){
	path_scanning_rand_criterion(task_list, run_times);
}

void HEURISTIC::path_scanning_rand_criterion(vector<int>& task_list, int runs){
	best_costs = INF;
	for (int rule=0; rule<runs; rule++) {
		construct_candidate(task_list);
		path_scanning_rand_criterion_one();
		get_costs(result);
		// cout<<rule<<" => "<<costs<<endl;
		if(costs<best_costs) {
			best_costs = costs;
			best_result = result;
			// show(best_result);
		}
	}
}

void HEURISTIC::path_scanning_rand_criterion(int runs){
	best_costs = INF;
	for (int rule=0; rule<runs; rule++) {
		construct_candidate();
		path_scanning_rand_criterion_one();
		get_costs(result);
		// cout<<rule<<" => "<<costs<<endl;
		if(costs<best_costs) {
			best_costs = costs;
			best_result = result;
			// show(best_result);
		}
	}
}

void HEURISTIC::path_scanning_rand_criterion(){
	path_scanning_rand_criterion(run_times);
}

void HEURISTIC::path_scanning_rand_criterion_one(){
	if(candidate.size()==0) {
		construct_candidate();
	}
	result.clear();
	result.push_back(0);
	int e = 0;
	int ubar;
	loaded = 0;
	while(!candidate.empty()){
		compare_rule = rand()%5+1;
		ubar = get_next(e);
		if(ubar>0){
			result.push_back(ubar);
			loaded += (*problem)[ubar].demand;
			e = ubar;
			delete_candidate(ubar);
		} else {
			result.push_back(0);
			e = 0;
			loaded = 0;
		}
	}
	result.push_back(0);
}


void HEURISTIC::construct_candidate_AM(vector<int>& task_list){
	candidate.clear();
	has_mark.clear();
	for (int i=0; i<=2*problem->task_number; i++) {
		has_mark.push_back(0);
	}
	for (auto e: task_list) {
		if(e==0) continue;
		if(has_mark[e] == 0){
			candidate.push_back(e);
			has_mark[e] = 1;
			has_mark[problem->get_invert(e)] = 1;
		}
	}
}

void HEURISTIC::construct_candidate_AM(){
	candidate.clear();
	has_mark.clear();
	for (int i=0; i<=2*problem->task_number; i++) {
		has_mark.push_back(0);
	}
	int e;
	for (int i=0; i<problem->task_number*2; i++) {
		e = i+1;
		if(e==0) continue;
		if(has_mark[e] == 0){
			candidate.push_back(e);
			has_mark[e] = 1;
			has_mark[problem->get_invert(e)] = 1;
		}
	}
}

void HEURISTIC::augment_merge(){
	best_costs = INF;
	// for (int i = 0; i < 1; ++i) {
	construct_candidate_AM();
	augment_merge_rand();
	get_costs(result);
	if(costs<best_costs) {
		best_costs = costs;
		best_result = result;
	}
	// }
}

void HEURISTIC::augment_merge(int run_times){
	best_costs = INF;
	for (int i = 0; i < run_times; ++i) {
		construct_candidate_AM();
		augment_merge_rand();
		get_costs(result);
		if(costs<best_costs) {
			best_costs = costs;
			best_result = result;
		}
	}
}


void HEURISTIC::augment_merge(vector<int>& task_list){
	best_costs = INF;
	construct_candidate_AM(task_list);
	augment_merge_rand();
	get_costs(result);
	// cout<<costs<<endl;
	if(costs<best_costs) {
		best_costs = costs;
		best_result = result;
	}
}

void HEURISTIC::augment_merge(vector<int>& task_list,int run_times){
	best_costs = INF;
	for (int i = 0; i < run_times; ++i) {
		construct_candidate_AM(task_list);
		augment_merge_rand();
		get_costs(result);
		cout<<costs<<endl;
		if(costs<best_costs) {
			best_costs = costs;
			best_result = result;
		}
	}
}

void HEURISTIC::sort_and_resize(vector<Cycle>& cycles){
	sort(cycles.begin(),cycles.end());
	for (int i = 0; i < cycles.size(); ++i) {
		if(cycles[i].cycle.size()==0) {
			cycles.resize(i);
			break;
		}
	}
}

void HEURISTIC::init_cycles(vector<Cycle>& cycles){
	vector<int> path;
	path.resize(1);
	for (auto e:candidate) {
		path[0] = e;
		Cycle	C(problem,path);
		C.init();
		cycles.push_back(C);
	}
	sort(cycles.begin(),cycles.end());
	// cout<<"init"<<endl; for(auto C:cycles) C.show();
}

void HEURISTIC::augment(vector<Cycle>& cycles){
	// augment 
	for (int i = 0; i < cycles.size(); ++i) {
		if (cycles[i].cycle.size()==0) continue;
		for (int j = i+1; j < cycles.size(); ++j) {
			if (cycles[i].Have(cycles[j])) {
				cycles[i].Insert();
				cycles[j].Clear();
			}
		}
	}
	sort_and_resize(cycles);
	// cout<<"augment"<<endl; for(auto C:cycles) C.show();
}

void HEURISTIC::zero_augment(vector<Cycle>& cycles){
	// augment 
	for (int i = 0; i < cycles.size(); ++i) {
		if (cycles[i].cycle.size()==0) continue;
		for (int j = i+1; j < cycles.size(); ++j) {
			if (cycles[i].zero_connected(cycles[j])) {
				cycles[i].Insert();
				cycles[j].Clear();
			}
		}
	}
	sort_and_resize(cycles);
	// cout<<"augment"<<endl; for(auto C:cycles) C.show();
}

void HEURISTIC::get_best_merge(vector<Cycle>& cycles, Merge& merge_best){
	vector<Merge> merge_vector;
	merge_best.saving_costs = 0;
	get_best_merge_vector(cycles,merge_vector);
	if (merge_vector.size()>=1) {
		int sel = rand()%merge_vector.size();
		// cout<<"have*****>"<<merge_vector.size()<<"del" <<sel;
		// for(auto s:merge_vector) s.show();
		merge_best = merge_vector[sel];
	}
}

void HEURISTIC::get_best_merge_vector(vector<Cycle>& cycles,vector<Merge>& merge_vector){
	merge_vector.clear();
	Merge merge_best(problem);
	merge_best.saving_costs = 0;
	for (int i = 0; i < cycles.size(); ++i) {
		if (cycles[i].cycle.size()==0) continue;
		for (int j = i+1; j < cycles.size(); ++j) {
			if (cycles[j].cycle.size()==0) continue;
			Merge merge(problem,&cycles[i],&cycles[j]);
			if (merge.IsFeasible()) {
				merge.getSavingCosts();
				// merge_vector.push_back(merge);
				if (merge.saving_costs>merge_best.saving_costs) {
					merge_best = merge;
					merge_vector.clear();
					merge_vector.push_back(merge);
				} else if(merge.saving_costs==merge_best.saving_costs) {
					merge_vector.push_back(merge);
				}
			}
		}
	}
	sort(merge_vector.begin(),merge_vector.end());
}

void HEURISTIC::merge_cycles_best(vector<Cycle>& cycles){
	vector<Merge> merge_vector;
	int sel;
	vector<Cycle*> used;
	vector<int> selected;
	// has_mark.clear();
	// for (int i = 0; i < cycles.size(); ++i) {
	// 	has_mark.push_back(0);
	// }
	while (1) {
		get_best_merge_vector(cycles,merge_vector);
		if (merge_vector.size()==0||merge_vector[0].saving_costs<=0) {
			break;
		} else {
			// cout<<"size "<<merge_vector.size()<<" remain "<<cycles.size()<<endl;
			if (merge_vector.size()>=1) {
				used.clear();
				// cout<<"*"<<endl;
				int k = 1;
				// if(merge_vector.size()>k){
				// 	RandPermute(selected,k);
				// }else{
				// 	RandPermute(selected,merge_vector.size());
				// }
				RandPermute(selected,merge_vector.size());
				// show(selected);
				for (int i = 0; i < selected.size(); ++i) {
					sel = selected[i]-1;
					bool is_used = find(used.begin(),used.end(),merge_vector[sel].C1)!=used.end()
						||find(used.begin(),used.end(),merge_vector[sel].C2)!=used.end();
					if(!is_used) {
						merge_vector[sel].MergeTwoCycle();
						// cout<<merge_vector[sel].C1-&cycles<<endl;
						used.push_back(merge_vector[sel].C1);
						used.push_back(merge_vector[sel].C2);
						//exit(0);
					}
				}
			}
		}
		sort_and_resize(cycles);
		// cout<<"*** augment run ***"<<endl; for(auto C:cycles) C.show();
	}
}

void HEURISTIC::merge_cycles(vector<Cycle>& cycles){
	Merge merge_best(problem);
	while (1) {
		get_best_merge(cycles,merge_best);
		if (merge_best.saving_costs<=0) {
			break;
		} else {
			// cout<<" remain "<<cycles.size()<<" "<<merge_best.saving_costs<<endl;
			// cout<<merge_best.C1->total_costs<<" + ";
			// cout<<merge_best.C2->total_costs<<" - ";
			// cout<<merge_best.saving_costs<<" ->- ";
			// merge_best.show();
			merge_best.MergeTwoCycle();
			// cout<<merge_best.C1->total_costs<<endl;
			// merge_best.C1->show();
			// merge_best.C2->show();
		}
		sort_and_resize(cycles);
		// cout<<"*** augment run ***"<<endl; for(auto C:cycles) C.show();
	}
}

void HEURISTIC::cycles_to_result(vector<Cycle>& cycles){
	result.clear();
	result.push_back(0);
	for (int i = 0; i < cycles.size(); ++i) {
		if (cycles[i].cycle.size()==0) continue;
		result.insert(result.end(),cycles[i].cycle.begin(),cycles[i].cycle.end());
		result.push_back(0);
	}
}

void HEURISTIC::augment_merge_rand(){
	if(candidate.size()==0) {
		construct_candidate_AM();
	}
	vector<Cycle > cycles;
	init_cycles(cycles);
	// zero_augment(cycles);
	augment(cycles);
	merge_cycles(cycles);
	// merge_cycles_best(cycles);
	cycles_to_result(cycles);
}
#endif
