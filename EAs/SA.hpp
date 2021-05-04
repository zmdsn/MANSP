#ifndef SA_H_
#define SA_H_
#include "../bin/general.hpp"
#include "../EAs/EA.hpp"
using namespace std;
typedef pair<int,VType> mark;
class SA:public EA4CARP{
	public:
		CARP_INDIVIDUAL S;
		CARP_INDIVIDUAL Sp;
		int to_be_updated;
		int mni = 100;
		int mnui = 6000;
		double pm;
		void main_loop();
		void run();
		// void get_fitness();
		VType get_fitness(CARP_INDIVIDUAL& I);
		void tournament_select();
		void local_search();
		bool local_search_phase();
		bool partial_replacement();
		void get_best_crossover(CARP_INDIVIDUAL* P,int k);
		// SA();
		SA(CARP* P);
		SA(const SA& algorithm);
		SA(int popsize);
		SA(int _popsize,int _end_type,int _max_iteration,int _max_time);
		SA& operator=(const SA&);
		~SA();
}

;
// SA::SA():EA4CARP(){}
SA::SA(CARP* P):EA4CARP(P){
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

SA::SA(int _popsize=3):EA4CARP(_popsize){
	swarm.resize(popsize);
	children.resize(popsize);
}

SA::SA(int _popsize,int _end_type,int _max_iteration,int _max_time):
	EA4CARP(_popsize,_end_type,_max_iteration,_max_time){
	best_individual.problem = problem;
	best_individual.fitness = INF;
}

SA::~SA(){}
bool SA::local_search_phase() {
	S.split_to_subroute();
	// do nothing
  // S.merge_subroute();
  // S.del_double_zero();
	// FIRST_RELOCATE_FLS RE(S);
	// RE.find_wanted_move_of_all_routes();
	// if( RE.wanted_move->costs<0 ) {
	// 	// cout<<"cyc "<<RE.wanted_move->costs<<endl;
	//   RE.wanted_move->move();
	//   S.merge_subroute();
	//   S.del_double_zero();
	//   return 1;
	// }

	// FIRST_SWAP_FLS RI(S);
	// FIRST_RELOCATE_FLS RI(S);
	// FIRST_REPLACE_INSERT_FLS RI(S);
	// RI.find_wanted_move_of_all_routes();
	// if( RI.wanted_move->costs<0 ) {
	// 	// cout<<"cyc "<<CYC.wanted_move->costs<<endl;
	// 	RI.wanted_move->move();
	// 	S.merge_subroute();
	// 	S.del_double_zero();
	// 	return 1;
	// }

	// FIRST_LOCAL_SEARCH_MA LS(S);
	// LS.find_wanted_move_of_all_routes();
	// if( LS.wanted_move->costs<0 ) {
	// 	LS.wanted_move->move();
	// 	S.merge_subroute();
	// 	S.del_double_zero();
	// 	return 1;
	// }

	// FIRST_RELOCATE_BEST_FLS RI(S);
	// RI.find_wanted_move_of_all_routes();
	// if( RI.wanted_move->costs<0 ) {
 // 		// cout<<"cyc "<<RI.wanted_move->costs<<endl;
	// 	RI.wanted_move->move();
	// 	S.merge_subroute();
	// 	S.del_double_zero();
	// 	return 1;
	// }

	FIRST_BETWEEN_2OPT_BEST_FLS RI(S);
	// FIRST_BETWEEN_2OPT_FLS RI(S);
	// FIRST_INNER_2OPT_FLS RI(S);
	RI.find_wanted_move_of_all_routes();
	if( RI.wanted_move->costs<0 ) {
 		// cout<<RI.wanted_move->costs<<"\t";
		RI.wanted_move->move();
		S.merge_subroute();
		S.del_double_zero();
		return 1;
	}

	// FIRST_BETWEEN_3CYCLE_FLS CYC(S);
	// CYC.find_wanted_move_of_all_routes();
	// if( CYC.wanted_move->costs<0 ) {
	// 	// cout<<"cyc "<<CYC.wanted_move->costs<<endl;
	// 	CYC.wanted_move->move();
	// 	S.merge_subroute();
	// 	S.del_double_zero();
	// 	return 1;
	// }
	return 0;
}

void SA::local_search() {
	// cout<<"LS "<<endl;
	S = child;
	int imrpoved = 1;
	while(imrpoved){
		imrpoved = local_search_phase();
	}
	S.route_to_tour();
	Sp = S;
	S.Ulusoy_split();
	get_fitness(S);
}

void SA::tournament_select() {
	// to parent1,parent2
	// first half
	int candidate1,candidate2;	
	candidate1 = rand()% (popsize/2);
	rand_select(); 
	candidate2 = swarm[parent1]<swarm[parent2]?parent1:parent2;
	parent1 = candidate1;
	parent2 = candidate2;
}

void rand_select_one(int sel,int total) {
	for
}

void SA::main_loop() {
	int ite = 0;
	while (ite<max_iteration && best_individual.costs>LB){
		child = swarm[0];
		local_search();
		get_fitness(S);
		if (S<child) {
			child = S;
		} else {
			if (rand()%100<100*pm) {
				child = S;
			}
		}
		swarm[0] = child;
		update_best();
		if(is_record(ite)) {
			record_best(ite);
		}
		cout<<ite<<" get solution : "<< child.fitness<<"\r";
		cout.flush();
		ite++;
	}
}

void SA::run(){
	LB = problem->LB;
	set_popsize(30);
	clock_t start_run,end_run;
	start_run = clock();
	iteration = 0;
	init_swarm_rand();
	// init_swarm_three_and_rand();
	// show_swarm_fitness();
	// partial_replacement();
	// show_swarm_fitness();
	// exit(0);
	// first phase
	pm = 0.1;
	main_loop();
	end_run  = clock();
	run_time = (double)(end_run - start_run) / CLOCKS_PER_SEC;
	cout<<endl<<"final result : "<<best_individual.fitness<<endl;
}

VType SA::get_fitness(CARP_INDIVIDUAL& I) {
	I.get_all_value();
	I.fitness = I.costs;
	return I.fitness;
}
#endif
