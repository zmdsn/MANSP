#ifndef LMA_H_
#define LMA_H_
#include "../bin/general.hpp"
#include "../EAs/EA.hpp"
using namespace std;
typedef pair<int,VType> mark;
class LMA:public EA4CARP{
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
		// LMA();
		LMA(CARP* P);
		LMA(const LMA& algorithm);
		LMA(int popsize);
		LMA(int _popsize,int _end_type,int _max_iteration,int _max_time);
		LMA& operator=(const LMA&);
		~LMA();
}

;
// LMA::LMA():EA4CARP(){}
LMA::LMA(CARP* P):EA4CARP(P){
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

LMA::LMA(int _popsize=3):EA4CARP(_popsize){
	swarm.resize(popsize);
	children.resize(popsize);
}

LMA::LMA(int _popsize,int _end_type,int _max_iteration,int _max_time):
	EA4CARP(_popsize,_end_type,_max_iteration,_max_time){
	best_individual.problem = problem;
	best_individual.fitness = INF;
}

LMA::~LMA(){}
bool LMA::local_search_phase() {
	S.split_to_subroute();
	FIRST_LOCAL_SEARCH_MA LS(S);
	LS.move_of_all_routes();
	// LS.find_wanted_move_of_all_routes();
	if( LS.improved<0 ) {
		// LS.wanted_move->move();
		S.merge_subroute();
		S.del_double_zero();
		return 1;
	}
	return 0;
}

void LMA::local_search() {
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

void LMA::tournament_select() {
	// to parent1,parent2
	// first half
	int candidate1,candidate2;	
	candidate1 = rand()% (popsize/2);
	rand_select(); 
	candidate2 = swarm[parent1]<swarm[parent2]?parent1:parent2;
	parent1 = candidate1;
	parent2 = candidate2;
}

void LMA::main_loop() {
	int ni=0,nui=0;
	while (ni<mni&&nui<mnui&& best_individual.costs>LB){
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
		}
		cout<<ni<<" get solution : "<< child.fitness<<"\r";
		cout.flush();
	}
}

void LMA::run(){
	LB = problem->LB;
	set_popsize(30);
	clock_t start_run,end_run;
	start_run = clock();
	iteration = 0;
	init_swarm_three_and_rand();
	// show_swarm_fitness();
	// partial_replacement();
	// show_swarm_fitness();
	// exit(0);
	int unupdate = 1;
	int upt = 0;
	// first phase
	pm = 0.1;
	mni = 20000;
	mnui = 6000;
	main_loop();
	// first phase
	int restarts = 20;
	for (int i = 0; i < restarts; ++i) {
		if (best_individual.costs<=LB) break;
		// init_swarm_three_and_rand();
		show_swarm_fitness();
		partial_replacement();
		show_swarm_fitness();
		pm = 0.2;
		mnui = 2000;
		main_loop();
	}
	end_run  = clock();
	run_time = (double)(end_run - start_run) / CLOCKS_PER_SEC;
	cout<<endl<<"final result : "<<best_individual.fitness<<endl;
}

VType LMA::get_fitness(CARP_INDIVIDUAL& I) {
	I.get_all_value();
	I.fitness = I.costs;
	return I.fitness;
}

void LMA::get_best_crossover(CARP_INDIVIDUAL* P,int k){
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

bool LMA::partial_replacement(){
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
