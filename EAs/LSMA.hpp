#ifndef LSMA_H_
#define LSMA_H_
#include "../bin/general.hpp"
#include "../EAs/EA.hpp"
#include "../heuristic/path_scanning.hpp"
#include "../heuristic/path_scanning_rand.hpp"
#include "../heuristic/path_scanning_ellipse.hpp"
#include "../heuristic/path_scanning_rand_SP.hpp"
#include "../heuristic/path_scanning_ellipse_sp.hpp"
#include "../heuristic/augment_merge.hpp"
#include "../problem/CARP.hpp"
#include "../bin/CARP_individual.hpp"
#include "../EAs/MA.hpp"
#include "../EAs/LSMA.hpp"
#include "../move/inv_swap_move.hpp"
#include "../move/between_2opt_merge.hpp"
#include "../move/between_2opt_reverse.hpp"
#include "../move/two_between_2opt_merge.hpp"
#include "../move/two_between_2opt_reverse.hpp"
// #include "../EAs/MANSP.hpp"
// #include "../local_search/first_feasible_of_MA.hpp"
// #include "../local_search/best_feasible_LS.hpp"
#include "../move/relocate.hpp"
#include "../move/inv_relocate.hpp"
// #include "../EAs/GA.hpp"
#include "../EAs/MAENS.hpp"
#include "../local_search/first_local_search.hpp"
#include "../local_search/first_feasible_LS.hpp"
#include "../local_search/first_local_search_penalty.hpp"
#include "../local_search/partial_best_feasible_LS.hpp"
#include "../local_search/partial_best_local_search_LSMA.hpp"
// #define Individual INDIVIDUAL<std::vector<int> >  
// #define Individual INDIVIDUAL<int[100] >  
// #define RELOCATE MOVE<Relocate>  
#define _SWAP MOVE<SWAP_MOVE>
#define _DRELOCATE MOVE<DOUBLE_RELOCATE>
#define _RELOCATE MOVE<Relocate>
#define _O2OPT MOVE<BETWEEN_2OPT>
#define _I2OPT MOVE<INNER_2OPT>
#define _BRE MOVE<InvRelocate>

using namespace std;
typedef pair<int,VType> mark;
class LSMA:public EA4CARP{
	public:
		CARP_INDIVIDUAL S;
		CARP_INDIVIDUAL Sp;
		int to_be_updated;
		int mni = 20000;
		int mnui = 20000;
		double pm;
		void main_loop();
		void run();
		clock_t start_run,end_run;
		// void get_fitness();
		VType get_fitness(CARP_INDIVIDUAL& I);
		void tournament_select();
		void local_search();
		bool local_search_phase();
		bool split_local_search_phase();
		bool partial_replacement();
		void get_best_crossover(CARP_INDIVIDUAL* P,int k);
		template<typename A>
		int get_LS(CARP_INDIVIDUAL* I) ;
		// LSMA();
		LSMA(CARP* P);
		LSMA(const LSMA& algorithm);
		LSMA(int popsize);
		LSMA(int _popsize,int _end_type,int _max_iteration,int _max_time);
		LSMA& operator=(const LSMA&);
		~LSMA();
}

;
// LSMA::LSMA():EA4CARP(){}
LSMA::LSMA(CARP* P):EA4CARP(P){
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

LSMA::LSMA(int _popsize=3):EA4CARP(_popsize){
	swarm.resize(popsize);
	children.resize(popsize);
}

LSMA::LSMA(int _popsize,int _end_type,int _max_iteration,int _max_time):
	EA4CARP(_popsize,_end_type,_max_iteration,_max_time){
	best_individual.problem = problem;
	best_individual.fitness = INF;
}

LSMA::~LSMA(){}


template<typename A>
int LSMA::get_LS(CARP_INDIVIDUAL* I) {
	A LS(I);
	LS.while_move_of_all_routes();
	I->merge_subroute();
	return LS.improved<0;
}

bool LSMA::split_local_search_phase() {
	S.split_to_subroute(); 
	S.rand_shuffle_sub();
	S.merge_subroute();
	S.split_to_subroute(); 
	int k = S.sub_route_num/5;
	S.split_to_subroute_partly(k);
	for (int i=0; i<S.sub_route_num; i++) {
		 // LSA(&S.sub_route[i]);
		// LocalSearch<PaticalBestFeasibleLocalSearch<_BRE> > LSB(&S.sub_route[i]);
		// LocalSearch<PaticalBestFeasibleLocalSearch<_RELOCATE> > LSC(&S.sub_route[i]);
		// LocalSearch<PaticalBestFeasibleLocalSearch<_O2OPT> > LSD(&S.sub_route[i]);
		// LocalSearch<PaticalBestFeasibleLocalSearch<_I2OPT> > LSE(&S.sub_route[i]);
		// LocalSearch<PaticalBestFeasibleLocalSearch<_DRELOCATE> > LSF(&S.sub_route[i]);
		switch (rand()%6) {
			case 0:
				return get_LS<LocalSearch<PaticalBestFeasibleLocalSearch<_RELOCATE> > >(&S.sub_route[i]) 
					||  get_LS<LocalSearch<PaticalBestFeasibleLocalSearch<MOVE<InvRelocate> > > >(&S.sub_route[i]);
				break;
			case 1:
				return get_LS<LocalSearch<PaticalBestFeasibleLocalSearch<MOVE<SWAP_MOVE> > > >(&S.sub_route[i]) 
					|| get_LS<LocalSearch<PaticalBestFeasibleLocalSearch<MOVE<InvSwap> > > >(&S.sub_route[i]);
				break;
			case 2:
				return get_LS<LocalSearch<PaticalBestFeasibleLocalSearch<MOVE<Between2OPTMerge> > > >(&S.sub_route[i])
				|| get_LS<LocalSearch<PaticalBestFeasibleLocalSearch<MOVE<Between2OPTReverse> > > >(&S.sub_route[i])
				|| get_LS<LocalSearch<PaticalBestFeasibleLocalSearch<MOVE<INNER_2OPT> > > >(&S.sub_route[i]);
			case 3:
				return get_LS<LocalSearch<PaticalBestFeasibleLocalSearch<MOVE<TwoBetween2OPTReverse> > > >(&S.sub_route[i])
				|| get_LS<LocalSearch<PaticalBestFeasibleLocalSearch<MOVE<TwoBetween2OPTReverse> > > >(&S.sub_route[i])
				|| get_LS<LocalSearch<PaticalBestFeasibleLocalSearch<MOVE<INNER_2OPT> > > >(&S.sub_route[i]);
				break;
			case 4:
				return get_LS<LocalSearch<PaticalBestFeasibleLocalSearch<MOVE<INNER_2OPT> > > >(&S.sub_route[i]);
				break;
			case 5:
				return get_LS<LocalSearch<FirstFeasibleLocalSearch<MOVE<TwoBetween2OPTReverse> > > >(&S.sub_route[i]);
				break;
		}
	}
	S.merge_subroute();
	return 0;
	// S.get_fitness();
	// if( improved<0 ) {
	//   S.merge_subroute();
	//   // cout<<improved;
	//   return 1;
	// }
}

bool LSMA::local_search_phase() {
	// S.split_to_subroute(); 
	/*
	int k = 5;
	S.split_to_subroute_partly(k);
	for (int i=0; i<S.sub_route_num; i++) {
		// LocalSearch<FirstFeasibleLocalSearch<_SWAP> > LSA(&S.sub_route[i]);
		// LocalSearch<FirstFeasibleLocalSearch<_BRE> > LSB(&S.sub_route[i]);
		// LocalSearch<FirstFeasibleLocalSearch<_RELOCATE> > LSC(&S.sub_route[i]);
		// LocalSearch<FirstFeasibleLocalSearch<_O2OPT> > LSD(&S.sub_route[i]);
		// LocalSearch<FirstFeasibleLocalSearch<_I2OPT> > LSE(&S.sub_route[i]);
		// LocalSearch<FirstFeasibleLocalSearch<_DRELOCATE> > LSF(&S.sub_route[i]);
		LocalSearch<PaticalBestFeasibleLocalSearch<_SWAP> > LSA(&S.sub_route[i]);
		LocalSearch<PaticalBestFeasibleLocalSearch<_BRE> > LSB(&S.sub_route[i]);
		LocalSearch<PaticalBestFeasibleLocalSearch<_RELOCATE> > LSC(&S.sub_route[i]);
		LocalSearch<PaticalBestFeasibleLocalSearch<_O2OPT> > LSD(&S.sub_route[i]);
		LocalSearch<PaticalBestFeasibleLocalSearch<_I2OPT> > LSE(&S.sub_route[i]);
		LocalSearch<PaticalBestFeasibleLocalSearch<_DRELOCATE> > LSF(&S.sub_route[i]);
		switch (rand()%5) {
			case 0:
				LSA.while_move_of_all_routes();
				break;
			case 1:
				LSB.while_move_of_all_routes();
				break;
			case 2:
				LSC.while_move_of_all_routes();
				break;
			case 3:
				LSD.while_move_of_all_routes();
				break;
			case 4:
				LSE.while_move_of_all_routes();
				break;
			case 5:
				LSF.while_move_of_all_routes();
				break;
		}
	}
	S.merge_subroute();
	*/
	// S.split_to_subroute(); 
	// // FIRST_LOCAL_SEARCH_MA LS(S);
	// LocalSearch<LSMALS> LS(&S);
	// // LocalSearch<FirstFeasibleLocalSearch<_RELOCATE> > LS(&S);
	// for (int i=0; i<S.sub_route_num; i++) {
	// }
	// // LS.improved = 0;
	// LS.move_of_all_routes();
	// // cout<<LS.improved<<endl;
	// // LS.find_wanted_move_of_all_routes();
	// if( LS.improved<0 ) {
	//   return 1;
	// }
	// if( LS.improved<0 ) {
	//   cout<<LS.improved<<endl;
	//   LS.move();
	//   S.merge_subroute();
	//   S.del_double_zero();
	//   return 1;
	// }
	// FIRST_LOCAL_SEARCH_MA LS(S);
	// LS.move_of_all_routes();
	// LS.find_wanted_move_of_all_routes();
	// if( LS.improved<0 ) {
	//   // LS.wanted_move->move();
	//   S.merge_subroute();
	//   S.del_double_zero();
	//   return 1;
	// }
	// S.split_to_subroute();
	// int k = S.sub_route_num/5;
	// // cout<<k<<endl;
	// S.split_to_subroute_partly(k);
	// int improved = 0;
	// for (int i=0; i<S.sub_route_num; i++) {
	//   // int i = 0;
	//   LocalSearch<LSMALS> LS(&S.sub_route[i]);
	//   // LocalSearch<FirstFeasibleLocalSearch<_RELOCATE> > LS(&S.sub_route[i]);
	//   // LS.move_of_all_routes();
	//   LS.while_move_of_all_routes();
	//   if( LS.improved<0 ) {
	//     improved += LS.improved;
	//   }
	// }
	// if( improved<0 ) {
	//   S.merge_subroute();
	//   // cout<<improved;
	//   return 1;
	// }
	return 0;
}

void LSMA::local_search() {
	S = child;
	int imrpoved = 1;
	// cout<<"LS"<<endl;
	while(imrpoved){
		// imrpoved = local_search_phase();
		imrpoved = split_local_search_phase();
		get_fitness(S);
		// if(rand()%3==0) break;
		// cout<<imrpoved<<"*";
		// cout<<S.fitness<<endl;
	}
	// exit(0);
	S.route_to_tour();
	Sp = S;
	S.Ulusoy_split();
	get_fitness(S);
}

void LSMA::tournament_select() {
	// to parent1,parent2
	// first half
	int candidate1,candidate2;	
	candidate1 = rand()% (popsize/2);
	rand_select(); 
	candidate2 = swarm[parent1]<swarm[parent2]?parent1:parent2;
	parent1 = candidate1;
	parent2 = candidate2;
}

void LSMA::main_loop() {
	int ni=0,nui=0;
	while (ni<mni&&nui<mnui&& best_individual.costs>LB) {
	// while (nui<mnui&& best_individual.costs>LB) {
		tournament_select();
		order_crossover();
		child.Ulusoy_split_improve();
		if (!child.is_feasible()) {
			cout<<"child new is_feasible "<<child.is_feasible()<<endl;
			cout<<"child new is_uniqe "<<child.is_unique()<<endl;
			cout<<"child new is_over_load "<<child.is_over_load()<<endl;
			show(child.route);
			exit(0);
		}
		get_fitness(child);
		to_be_updated = (popsize/2)+rand()%(popsize/2);
		if (rand()%100<=pm*100) {
			local_search();
			if (not_in_swarm_except_k(S,to_be_updated)) {
				child = S;
			}
		}
		// if (!not_in_swarm(child)) {
		// 	for (auto& s: swarm) {
		// 		if (s.fitness==child.fitness) {
		// 			s = child;
		// 			// cout<<"changed"<<endl;
		// 			break;
		// 		}
		// 	}
		// }
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
			end_run  = clock();
			run_time = (double)(end_run - start_run) / CLOCKS_PER_SEC;
			// cout<<run_time<<endl;
			if (run_time>1800) break;
		}
		cout<<ni<<" get solution : "<< child.fitness<<"\r";
		cout.flush();
	}
}

void LSMA::run(){
	LB = problem->LB;
	set_popsize(10);
	start_run = clock();
	iteration = 0;
	// init_swarm_three_and_rand();
	init_swarm_PSrand();	
	// init_swarm_PSrand();
	// show_swarm_fitness();
	// partial_replacement();
	// show_swarm_fitness();
	// exit(0);
	int unupdate = 1;
	int upt = 0;
	// first phase
	pm = 0.4;
	mni = 300000;
	mnui =200000;
	main_loop();
	end_run  = clock();
	run_time = (double)(end_run - start_run) / CLOCKS_PER_SEC;
	cout<<endl<<"final result : "<<best_individual.fitness<<endl;
}

VType LSMA::get_fitness(CARP_INDIVIDUAL& I) {
	I.get_all_value();
	I.fitness = I.costs;
	return I.fitness;
}

void LSMA::get_best_crossover(CARP_INDIVIDUAL* P,int k){
	S.fitness = INF;
	for (int i = 0; i < popsize; ++i)	{
		if (k==i) continue;
		order_crossover(P,&swarm[i]);
		if (child<S&&not_in_swarm(child)) {
			S = child;
		}
	}
	child = S;
}

bool LSMA::partial_replacement(){
	std::vector<CARP_INDIVIDUAL> OMIGA;
	int nrep = 8;
	// generate OMIGA
	init_swarm_rand(OMIGA);
	int done = 0;
	while(done<nrep) {
		for (int k = 0; k < nrep; ++k) {
			if (OMIGA[k]<swarm[k]) {
				swarm[k] = OMIGA[k];
				done++;
				sort(swarm.begin(),swarm.end());
			} else {
				get_best_crossover(&OMIGA[k],k);
				if (child<swarm[popsize-1]) {
					swarm[popsize-1] = child;
					done++;
					sort(swarm.begin(),swarm.end());
				}
			}
			if (done==nrep) break;
		}
	}
	return 1;
}

#endif
