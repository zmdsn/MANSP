/*************************************************************************
 *  Name  : ../heuristic/augment_merge.hpp
 *  Author: zmdsn
 *  Mail  : zmdsn@126.com 
 * Created Time: Sun 31 Jan 2021 04:10:01 PM CST
 * 
 ************************************************************************/
#ifndef  __AM__HPP__
#define  __AM__HPP__
#include<iostream>
#include<algorithm>
#include "../problem/CARP.hpp"
#include "../heuristic/heuristic.hpp"
using namespace std;
class AugmentMerge:public Heuristic{
	public:
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
		AugmentMerge(){};
		AugmentMerge(CARP* P);
		AugmentMerge(CARP* p, DType Capacity);
		~AugmentMerge(){};
}
;
AugmentMerge::AugmentMerge(CARP* p): Heuristic(p){
}

void AugmentMerge::construct_candidate_AM(vector<int>& task_list){
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

void AugmentMerge::construct_candidate_AM(){
	vector<int> task_list;
	for (int i=1; i<=problem->task_number*2; i++) {
		task_list.push_back(i);
	}
	construct_candidate_AM(task_list);
}

void AugmentMerge::augment_merge(){
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

void AugmentMerge::augment_merge(int run_times){
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

void AugmentMerge::augment_merge(vector<int>& task_list){
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

void AugmentMerge::augment_merge(vector<int>& task_list,int run_times){
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

void AugmentMerge::sort_and_resize(vector<Cycle>& cycles){
	sort(cycles.begin(),cycles.end());
	for (int i = 0; i < cycles.size(); ++i) {
		if(cycles[i].cycle.size()==0) {
			cycles.resize(i);
			break;
		}
	}
}

void AugmentMerge::init_cycles(vector<Cycle>& cycles){
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

void AugmentMerge::augment(vector<Cycle>& cycles){
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

void AugmentMerge::zero_augment(vector<Cycle>& cycles){
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

void AugmentMerge::get_best_merge(vector<Cycle>& cycles, Merge& merge_best){
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

void AugmentMerge::get_best_merge_vector(vector<Cycle>& cycles,vector<Merge>& merge_vector){
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

void AugmentMerge::merge_cycles_best(vector<Cycle>& cycles){
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

void AugmentMerge::merge_cycles(vector<Cycle>& cycles){
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

void AugmentMerge::cycles_to_result(vector<Cycle>& cycles){
	result.clear();
	result.push_back(0);
	for (int i = 0; i < cycles.size(); ++i) {
		if (cycles[i].cycle.size()==0) continue;
		result.insert(result.end(),cycles[i].cycle.begin(),cycles[i].cycle.end());
		result.push_back(0);
	}
}

void AugmentMerge::augment_merge_rand(){
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
