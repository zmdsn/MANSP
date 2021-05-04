#ifndef MA_H_
#define MA_H_
#include "../bin/general.hpp"
#include "../EAs/EA.hpp"
//#include "../local_search/first_feasible_of_MA.hpp"
#include "../local_search/first_feasible_LS.hpp"

using namespace std;
typedef pair<int,VType> mark;


class MA:public EA4CARP{
	public:
		CARP_INDIVIDUAL S;
		int to_be_updated;
		int mni = 100;
		int mnui = 6000;
		double pm;
		void main_loop();
		void main_loop_each_one_swarm();
		void run();
		// void get_fitness();
		VType get_fitness(CARP_INDIVIDUAL& I);
		void local_search();
		bool local_search_phase();
		// MA();
		MA(CARP* P);
		MA(const MA& algorithm);
		MA(int popsize);
		MA(int _popsize,int _end_type,int _max_iteration,int _max_time);
		MA& operator=(const MA&);
		~MA();
}
;
// MA::MA():EA4CARP(){}
MA::MA(CARP* P):EA4CARP(P){
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

MA::MA(int _popsize=3):EA4CARP(_popsize){
	swarm.resize(popsize);
	children.resize(popsize);
}

MA::MA(int _popsize,int _end_type,int _max_iteration,int _max_time):
	EA4CARP(_popsize,_end_type,_max_iteration,_max_time){
	best_individual.problem = problem;
	best_individual.fitness = INF;
}

MA::~MA(){}

bool MA::local_search_phase() {
	S.rand_shuffle_sub();
	LocalSearch<FirstFeasibleLocalSearch<RELOCATE> > MLS(&S);
	return MLS.while_move_of_all_routes();
	/*MLS.find_wanted_move_of_all_routes();
	if( MLS.wanted_move->costs<0 ) {
		MLS.wanted_move->move();
		S.merge_subroute();
		S.del_double_zero();
		return 1;
	}*/
	 //return 0;
}

void MA::local_search() {
	S = child;
	bool improved = 1;
	//	S.get_fitness();
	//	cout<<improved<<" "<<S.fitness<<S.is_feasible()<<endl;
	while(improved){
		improved = local_search_phase();
	//	S.get_fitness();
	//	cout<<improved<<" "<<S.fitness<<S.is_feasible()<<endl;
		//if (improved) exit(0);
	}
	S.route_to_tour();
	// Sp = S;
	S.Ulusoy_split();
	get_fitness(S);
}

void MA::main_loop() {
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
		cout<<ni<<" "<<swarm_distance()<<endl;
		//cout<<ni<<" get solution : "<< child.fitness<<"\r";
		cout.flush();
	}
}

void MA::main_loop_each_one_swarm() {
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

void MA::run(){
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

VType MA::get_fitness(CARP_INDIVIDUAL& I) {
	I.get_all_value();
	I.fitness = I.costs;
	return I.fitness;
}
#endif
