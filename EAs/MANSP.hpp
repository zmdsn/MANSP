#ifndef MANSP_H_
#define MANSP_H_
#include "../bin/general.hpp"
#include "../EAs/EA.hpp"
#include "../local_search/first_feasible_of_MA.hpp"
#include "../local_search/best_local_search.hpp"
#include "../local_search/best_local_search_penalty.hpp"
using namespace std;
typedef pair<int,VType> mark;
class MANSP:public EA4CARP{
	public:
		CARP_INDIVIDUAL S;
		int to_be_updated;
		int mni = 100;
		int mnui = 6000;
		double pm;
		// double lamda=10.3;
		double lamda=1;
		void main_loop();
		void main_loop_each_one_swarm();
		void run();
		// void get_fitness();
		VType get_fitness(CARP_INDIVIDUAL& I);
		void local_search();
		bool local_search_phase();
		bool update_best();
		void show_best();
		// MANSP();
		MANSP(CARP* P);
		MANSP(const MANSP& algorithm);
		MANSP(int popsize);
		MANSP(int _popsize,int _end_type,int _max_iteration,int _max_time);
		MANSP& operator=(const MANSP&);
		~MANSP();
}
;
// MANSP::MANSP():EA4CARP(){}
MANSP::MANSP(CARP* P):EA4CARP(P){
	problem = P;
	best_solution.fitness = INF;
	child.problem = P;
	popsize = 10;
	end_type = 2;
	max_iteration = 1000;
	max_time = 600;
	swarm.resize(popsize);
	children.resize(popsize);
	best_individual.problem = problem;
	best_individual.fitness = INF;
}

MANSP::MANSP(int _popsize=3):EA4CARP(_popsize){
	swarm.resize(popsize);
	children.resize(popsize);
}

MANSP::MANSP(int _popsize,int _end_type,int _max_iteration,int _max_time):
	EA4CARP(_popsize,_end_type,_max_iteration,_max_time){
	best_individual.problem = problem;
	best_individual.fitness = INF;
}

MANSP::~MANSP(){}

bool MANSP::local_search_phase() {
	S.rand_shuffle_sub();
	// FirstLocalSearchMA MLS(S);
	BestLocalSearch<RELOCATE> MLS(S);
	// BestLocalSearchPenalty<RELOCATE> MLS(S);
	// MLS.find_wanted_move_of_all_routes();
	// MLS.move_of_all_routes();
	// show(S.route);
	if (MLS.move_of_all_routes() ){
		// if (MLS.wanted_move->over_load>0) {
			// cout<<MLS;
		//   S.route_to_tour();
			// get_fitness(S);
		//   show(S.route);
		//   show((MLS.wanted_move->R1)->route);
		//   show((MLS.wanted_move->R2)->route);
			// cout<<S.costs<<" "<<S.fitness<<" e "<<S.over_load<<endl;
		//   exit(0);
		// }
		return 1;
	}
	return 0;
}

void MANSP::local_search() {
	S = child;
	int imrpoved = 1;
	// show(S.route);
	// while(imrpoved){
	//   imrpoved = local_search_phase();
	while (local_search_phase()) {
		S.route_to_tour();
		// Sp = S;
		// S.Ulusoy_split();
		get_fitness(S);
		// cout<<S.fitness<<" e "<<S.over_load<<endl;
		// show(S.route);
	}
	// exit(0);
	S.route_to_tour();
	// Sp = S;
	S.Ulusoy_split();
	get_fitness(S);
}

void MANSP::main_loop() {
	int ni=0,nui=0;
	while (ni<mni&&nui<mnui&& best_individual.costs>LB){
		tournament_select();
		order_crossover();
		//dchild.route_to_tour();
		child.Ulusoy_split();
		get_fitness(child);
		to_be_updated = (popsize/2)+rand()%(popsize/2);
		if (rand()%100<=pm*100) {
			local_search();
			if (not_in_swarm_except_k(S,to_be_updated)) {
				child = S;
			}
		}
		if (not_in_swarm_except_k(child,to_be_updated)) {
			ni++;
			if (child<swarm[0]) {
				nui = 0;
			} else {
				nui++;
			}
			swarm[to_be_updated] = child;
			sort(swarm.begin(),swarm.end());
			update_best();
			if(is_record(ni)) {
				record_best(ni);
			}
		}
		cout<<ni<<" get solution : "<< child.fitness<<"\r";
		cout.flush();
	}
}

void MANSP::main_loop_each_one_swarm() {
	int ni=0,nui=0;
	children.clear();
	for (int i=0; i<popsize; i++) {
		tournament_select();
		order_crossover();
		child.Ulusoy_split();
		get_fitness(child);
		children.push_back(child);
	// child.Ulusoy_split();
	// get_fitness(child);
	// to_be_updated = (popsize/2)+rand()%(popsize/2);
	//   if (rand()%100<=pm*100) {
	//     local_search();
	//     if (not_in_swarm_except_k(S,to_be_updated)) {
	//       child = S;
	//     }
	//   }
	//   if (not_in_swarm_except_k(child,to_be_updated)) {
	//     ni++;
	//     if (child<swarm[0]) {
	//       nui = 0;
	//     } else {
	//       nui++;
	//     }
	//     swarm[to_be_updated] = child;
	//   }
	//   cout<<ni<<" get solution : "<< child.fitness<<"\r";
	//   cout.flush();
	// }
	}
}

void MANSP::run(){
	// alp = 0.75;
	// HEURISTIC H(problem);
	PathScanning H(problem);
	LB = problem->LB;
	set_popsize(30);
	// H.path_scanning();
	H.get_best_solution();
	candidate_individual.problem = problem;
	candidate_individual.set_route(H.best_result);
	get_fitness(candidate_individual);
	cout<<"candidate_individual : "<<candidate_individual.fitness<<endl;
	clock_t start_run,end_run;
	run_time;
	start_run = clock();
	iteration = 0;
	achive_size = 0;
	// init_swarm();
	// init_swarm_with_LS();
	init_swarm_three_and_rand();
	unimproved = 0;
	int unupdate = 1;
	int upt = 0;
	// first phase
	pm = 0.1;
	mni = 20000;
	mnui = 6000;
	main_loop();
	// second phase
	for (int i = 0; i < 10; ++i) {
		if (best_individual.costs>LB) {
			init_swarm_three_and_rand();
			pm = 0.2;
			mnui = 2000;
			mni = mnui;
			main_loop();
		}
	}
	end_run  = clock();
	run_time = (double)(end_run - start_run) / CLOCKS_PER_SEC;
	cout<<endl<<"final result : "<<best_individual.fitness<<endl;
}

VType MANSP::get_fitness(CARP_INDIVIDUAL& I) {
	// I.get_all_value();
	// I.fitness = I.costs;
	I.get_nonsmooth_penatly_fitness(lamda);
	// I.get_penatly_fitness(lamda);
	// I.get_penalty_fitness(lamda);
	// I.get_over_load();
	// show(I.route);
	// I.show_all();
	// cout<<I.costs<<" c,f "<<lamda<<" "<<I.fitness<<"\t over_load "<<I.over_load<<endl;
	return I.fitness;
}

bool MANSP::update_best() {
	for (auto& I: swarm) {
		if(I.is_feasible() && I < best_individual){
			best_individual = I;
			show_best();
			return 1;
		}
	}
	return 0;
}

void MANSP::show_best() {
	cout<<"\n************ best *********************";
	cout<<endl<<"new best "<<best_individual.fitness<<endl;
	cout<<"costs "<<best_individual.costs<<endl;
	cout<<"fitness "<<best_individual.fitness<<endl;
	cout<<"over load "<<best_individual.over_load<<endl;
	//best_individual.show_all();
	cout<<"****************************************\n";
}

#endif
