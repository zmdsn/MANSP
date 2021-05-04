#ifndef MAENS_H_
#define MAENS_H_
#include<algorithm>
#include<iostream>
#include <dirent.h>
#include <fstream>
#include "../local_search/best_LS_of_MAENS.hpp"
// #include "../local_search/first_local_search.hpp"
// #include "../local_search/first_feasible_LS.hpp"
// #include "../local_search/first_local_search_penalty.hpp"
// #include "../local_search/best_local_search_penalty.hpp"
// #include "../move/swap_move.hpp"
// #include "../move/between_2opt.hpp"
// #include "../move/inner_2opt.hpp"
using namespace std;
typedef pair<int,VType> mark;
// typedef pair<int,int> insert_pos;

class MAENS:public EA4CARP{
	public:
		int unupdate;
		int unupdated_time;
		double lamda;
		double pm;
		// CARP *problem;
		// int popsize;      // the size of population
		int ubntrial;
		// vector<CARP_INDIVIDUAL> swarm;  // main swarm
		// vector<CARP_INDIVIDUAL> children; // children
		// CARP_INDIVIDUAL child;
		CARP_INDIVIDUAL Sx,Sl,Slp;
		// CARP_INDIVIDUAL best_individual;
		// CARP_INDIVIDUAL best_solution;
		// CARP_INDIVIDUAL candidate_individual;
		// vector<mark> record;//iterative optimal value
		// int max_ite;
		// int max_fun_cal;
		// int iteration ;
		// double run_time;
		// void init_swarm();
		void init_swarm_PSrand();
		// void update_best();
		void update_swarm_fitness();
		void update_lamda(bool is_updated);
		void update_lamda(CARP_INDIVIDUAL& I,bool is_updated);
		// void init_swarm_with_LS();
		// void init_swarm_rand();
		// void test_distance_to_best();
		void run();
		// void set_popsize(int popsize);
		void main_loop();
		// int end_type;     // the end type of algorithm, 0 for max_iteration,1 fo    r max_time, 2 for the min of them
		// int max_iteration;// the max iteration
		// int max_time;     // the max time
		// int parent1,parent2; // for Crossover
		// void record_best(int iterator);
		// bool is_record(int iterator);
		// bool is_stop(int iterator);
		// bool not_in();
		bool child_not_in();
		bool individual_not_in(CARP_INDIVIDUAL& I);
		// bool update_best();
		// void show_all();
		void show_swarm_costs();
		// void show_best();
		// void show_swarm_fitness();
		// void check();
		// void rand_second_half_change();
		bool add_to_swarm(CARP_INDIVIDUAL& I);
		// bool rand_second_half_change(CARP_INDIVIDUAL& I);
		// void second_half_change_min_distance();	
		// void show_swarm();
		void init_lamda();
		void init_lamda(CARP_INDIVIDUAL& I);
		// void get_fitness();
		// void get_fitness(CARP_INDIVIDUAL& I);
		VType get_fitness(CARP_INDIVIDUAL& I);
		void get_init_fitness(CARP_INDIVIDUAL& I);
		// double solution_distance(CARP_INDIVIDUAL& I,CARP_INDIVIDUAL& S);
		// double Jaccard_distance(CARP_INDIVIDUAL& I,CARP_INDIVIDUAL& S);
		// double min_swarm_distance();
		// double max_swarm_distance();
		// int intersection_number_set(CARP_INDIVIDUAL& I,CARP_INDIVIDUAL& S);
		// void order_crossover_subroute_PS(CARP_INDIVIDUAL *p1,CARP_INDIVIDUAL *p2);       //order crossover
		// void order_crossover_subroute_PS();       //order crossover
		// void order_crossover(CARP_INDIVIDUAL *p1,CARP_INDIVIDUAL *p2);       //order crossover
		void order_crossover();       //order crossover
		void stochastic_ranking_swarm();
		// void SBX(CARP_INDIVIDUAL *p1,CARP_INDIVIDUAL *p2);       //order crossover
		// void SBX();       //order crossover
		// void order_crossover_subroute_based(CARP_INDIVIDUAL *p1,CARP_INDIVIDUAL *p2);
		// void order_crossover_subroute_based();
		// void order_crossover_subroute_PSE(CARP_INDIVIDUAL *p1,CARP_INDIVIDUAL *p2);
		// void order_crossover_subroute_PSE();
		// void order_crossover_subroute_PSR(CARP_INDIVIDUAL *p1,CARP_INDIVIDUAL *p2);
		// void order_crossover_subroute_PSR();
		// void rand_select();
		// void tournament_select();
		// void save_date();
		void local_search(CARP_INDIVIDUAL& Sl) ;
		bool local_search_phase(CARP_INDIVIDUAL& Sl) ;
		// MAENS();
		MAENS(CARP* P);
		MAENS(const MAENS& algorithm);
		MAENS(int popsize);
		MAENS(int _popsize,int _end_type,int _max_iteration,int _max_time);
		MAENS& operator=(const MAENS&);
		~MAENS();
}

;
// MAENS::MAENS():EA4CARP(){}
MAENS::MAENS(CARP* P) : EA4CARP(P){
	// problem = P;
	best_solution.fitness = INF;
	best_solution.costs   = INF;
	child.problem = P;
	popsize = 30;
	end_type = 2;
	max_iteration = 1000;
	max_time = 600;
	swarm.resize(popsize);
	children.resize(popsize);
	lamda = 10;
	init_lamda();
	pm = 0.2;
}

MAENS::MAENS(int _popsize=3):EA4CARP(_popsize){
	swarm.resize(popsize);
	children.resize(popsize);
}

MAENS::MAENS(int _popsize,int _end_type,int _max_iteration,int _max_time) 
	:EA4CARP(_popsize,_end_type,_max_iteration,_max_time){
		// popsize(_popsize),end_type(_end_type),max_iteration(_max_iteration),max_time(_max_time) {
}

MAENS::~MAENS(){}

bool MAENS::child_not_in() {
	for(auto	I:swarm) {
		if(I.fitness==child.fitness)
			return 0;
	}
	return 1;
}

bool MAENS::individual_not_in(CARP_INDIVIDUAL& I) {
	for(auto	S:swarm) {
		if(I.over_load==S.over_load && I.costs==S.costs)
			return 0;
		// if(I.fitness==S.fitness)
		//   return 0;
	}
	return 1;
}

// void MAENS::check() {
//   for (int i=0; i<popsize; i++) {
//     if(swarm[i].is_unique()==0){
//       cout<<"\nNot unique swarm "<<i<<endl;
//       swarm[i].show_all();
//       exit(0);
//     }
//   }
//   if(child.is_unique()==0){
//     cout<<"not unique child"<<endl;
//     child.show_all();
//     exit(0);
//   }
// }

void MAENS::init_swarm_PSrand() {
	for (int i=0; i<popsize; i++) {
		swarm[i].problem = problem;
		swarm[i].fitness = INF;
	}	
	child.problem = problem;
	ubntrial = 50;
	int ntrial;
	PathScanningRand PS(problem);
	for (int i=0; i<popsize; i++) {
		for (ntrial=0; ntrial<ubntrial; ntrial++) {
			PS.get_best_solution();
			child.route = PS.best_result;
			get_init_fitness(child);
			// cout<<child.fitness<<endl;
			if(child_not_in()){
				swarm[i] = child;
				break;
			}
		}
		if(ntrial>=ubntrial){
			set_popsize(i);
			break;
		}
	}
	sort(swarm.begin(),swarm.end());
	best_individual.problem= problem;
	best_individual.fitness = INF;
	best_individual.costs   = INF;
	update_best();
}

bool MAENS::local_search_phase(CARP_INDIVIDUAL& I) {


  /*
	int rule = 1;
	VType min_fitness;
	// FirstLocalSearchPenalty<RELOCATE> RE(I);
	BestLocalSearchPenalty<RELOCATE> RE(I);
	RE.set_lamda(I.lamda);
	RE.find_wanted_move_of_all_routes();
	min_fitness = RE.get_fitness();

	BestLocalSearchPenalty<DOUBLE_RELOCATE> DRE(I);
	DRE.set_lamda(I.lamda);
	// DRE.find_wanted_move_of_all_routes();
	// if (min_fitness>DRE.wanted_move->fitness) {
	//   min_fitness = DRE.get_fitness();
	//   rule = 2;
	// }
	
	BestLocalSearchPenalty<SWAP_MOVE> SPM(I);
	SPM.set_lamda(I.lamda);
	SPM.find_wanted_move_of_all_routes();
	if (min_fitness>SPM.wanted_move->fitness) {
		min_fitness = SPM.get_fitness();
		rule = 3;
	}

	BestLocalSearchPenalty<BETWEEN_2OPT> B2P(I);
	B2P.set_lamda(I.lamda);
	B2P.find_wanted_move_of_all_routes();
	if (min_fitness>B2P.wanted_move->fitness) {
		min_fitness = B2P.get_fitness();
		rule = 4;
	}

	BestLocalSearchPenalty<BETWEEN_2OPT> I2P(I);
	I2P.set_lamda(I.lamda);
	I2P.find_wanted_move_of_all_routes();
	if (min_fitness>I2P.wanted_move->fitness) {
		min_fitness = I2P.get_fitness();
		rule = 5;
	}
	
	rule = 1;
	switch (rule) {
		case 1:
			return RE.move();
			// return RE.move_of_all_routes();
		// case 2:
		//   DRE.wanted_move->move();
			// return DRE.move_of_all_routes();
			// return 1;
		case 3:
			SPM.move();
			// return SPM.move_of_all_routes();
			// return 1;
		case 4:
			B2P.move();
			// return B2P.move_of_all_routes();
			return 1;
		case 5:
			I2P.move();
			return 1;
			// return I2P.move_of_all_routes();
	}
  */
	return 0;
}

void MAENS::local_search(CARP_INDIVIDUAL& Sl) {
	bool imp = 1;
	Slp = Sl;
	init_lamda(Sl);
	int count = 0,const_fsb=0,const_infsb=0;
  LocalSearch<BestLocalSearchMAENS> LS(&Sl);
  LS.LS->set_best_costs(best_solution.costs);
  LS.LS->init_lamda();
	while (imp){
		imp = 0;
		count++;
		if (Sl.is_feasible()) {
			const_fsb ++;
		} else {
			const_infsb ++;
		}
		if (count%5==0) {
			if (const_fsb == 5) {
				Sl.lamda /= 2;
			} else if(const_infsb == 5) {
				Sl.lamda *= 2;
			}
			get_fitness(Sl);
			const_fsb = 0;
			const_infsb = 0;
		} 
		Slp = Sl;
		// imp = local_search_phase(Sl);
    // LS.LS.find_wanted_move_of_all_routes();
    // LS.move();
    LS.LS->costs = INF;
    LS.LS->fitness = INF;
    // move_of_all_routes();
    LS.set_lamda(Sl.lamda);
    LS.move_of_all_routes();
		get_fitness(Sl);
		// cout<<count<<" => "<<const_infsb<<" "<<const_fsb<<" "<<Sl.lamda<<endl;
    // cout<<count<<" => "<<Sl.fitness<<endl;
		if (Sl.fitness<Slp.fitness) {
			imp = 1;
		}
	}
	Sl = Slp; ///biaozhu 
}

void MAENS::stochastic_ranking_swarm() {
	int Pf = 45;
	for (int i=0; i<swarm.size(); i++) {
		// int r = rand()%100;
		for (int j=0; j<i; j++) {
			if (swarm[i].over_load==0&&swarm[j].over_load==0&&rand()%100<Pf) {
				if (swarm[j].costs>swarm[j+1].costs) {
					Slp = swarm[j];
					swarm[j] = swarm[j+1];
					swarm[j+1] = Slp;
				}
			} else {
				if (swarm[j].over_load>swarm[j+1].over_load) {
					Slp = swarm[j];
					swarm[j] = swarm[j+1];
					swarm[j+1] = Slp;
				}
			}
		}
	}
}

void MAENS::main_loop() {
	// for (int i=0; i<popsize; i++) {
	int ite = 1;
	while (ite<6*popsize) {
		SBX();
		get_fitness(child);
		Sx = child;
		Sl = child;
		if(rand()%100<100*pm){
			local_search(Sl);
			if (individual_not_in(Sl) ){
				child = Sl;
			}
		} else {
			child = Sx;
		}
		if(swarm[parent1]==child) {
			swarm[parent1] = child;
		}else if(swarm[parent2]==child) {
			swarm[parent2] = child;
		} else {
			add_to_swarm(child);
			ite++;
		}
	}

	// update_swarm_fitness();
	sort(swarm.begin(),swarm.end());
	// stochastic_ranking_swarm();
	// show_swarm_fitness();
	swarm.resize(popsize);
	if(update_best()) {
		// cout<<"min_distance "<< min_swarm_distance()<<endl;
		// cout<<"max_distance "<< max_swarm_distance()<<endl;
		// exit(0);
	}
}

void MAENS::show_swarm_costs(){
	cout<<"*****************"<<endl;
	for (auto I: swarm) {
		cout<<I.costs<<" ";
	}
	cout<<endl<<"*****************"<<endl;

}
// double MAENS::max_swarm_distance() {
//   double distance,max_distance;
//   max_distance = 0;
//   for (int i=0; i<popsize; i++) {
//     for (int j=0; j<popsize; j++) {
//       if(i==j) continue;
//       distance = Jaccard_distance(swarm[i],swarm[j]);
//       if(distance>max_distance) {
//         max_distance = distance;
//       }
//     }
//   }
//   return max_distance;
// }

// double MAENS::min_swarm_distance() {
//   double distance,min_distance;
//   min_distance = 1;
//   for (int i=0; i<popsize; i++) {
//     for (int j=0; j<popsize; j++) {
//       if(i==j) continue;
//       distance = Jaccard_distance(swarm[i],swarm[j]);
//       if(distance<min_distance) {
//         min_distance = distance;
//       }
//     }
//   }
//   return min_distance;
// }

// void MAENS::second_half_change_min_distance() {
//   int k;
//   double distance,min_distance;
//   min_distance = 1;
//   k = popsize/2;
//   for (int i=popsize/2; i<popsize; i++) {
//   // for (int i=0; i<popsize; i++) {
//     distance = Jaccard_distance(swarm[i],child);
//     if(distance<min_distance) {
//       k = i;
//       min_distance = distance;
//     }
//   }
//   if(child<swarm[k] && not_in()) {
//     swarm[k] = child;
//     test_distance_to_best();
//     show_swarm_fitness();
//   }
//   sort(swarm.begin(),swarm.end());
// }

bool MAENS::add_to_swarm(CARP_INDIVIDUAL& I) {
	if(individual_not_in(I)) {
		if (child.fitness != child.costs+child.lamda*child.over_load){
		// cout<<child.costs<<" "<<child.over_load<<" "<<child.fitness<<endl;
		get_fitness(child);
		// cout<<child.lamda<<"\t"<<child.costs<<" "<<child.over_load<<" "<<child.fitness<<endl;
    // exit(0);
		}
		swarm.push_back(I);
		return 1;
	}
	return 0;
}

// bool MAENS::rand_second_half_change(CARP_INDIVIDUAL& I) {
//   int k = rand()%(popsize/2) + popsize/2;
//   if(I<swarm[k]&&not_in()) {
//     swarm[k] = I;
//     test_distance_to_best();
//     show_swarm_fitness();
//     sort(swarm.begin(),swarm.end());
//     return 1;
//   }
//   return 0;
// }

// void MAENS::rand_second_half_change() {
//   int k = rand()%(popsize/2) + popsize/2;
//   if(child<swarm[k]&&not_in()) {
//     swarm[k] = child;
//     test_distance_to_best();
//     show_swarm_fitness();
//   }
//   sort(swarm.begin(),swarm.end());
// }

// void MAENS::get_fitness() {
//   for (auto& I: swarm) {
//     get_fitness(I);
//   }
// }
	
void MAENS::init_lamda() {
	// DType Q = problem->Capacity;
	// lamda = costs/Q*(best_individual.costs/costs+over_load/Q+1);
	lamda = 2.1;
}

void MAENS::init_lamda(CARP_INDIVIDUAL& I) {
	DType Q = I.problem->Capacity;
	I.lamda = I.costs/Q*(best_individual.costs/I.costs+I.over_load/Q+1);
}

VType MAENS::get_fitness(CARP_INDIVIDUAL& I) {
	I.get_all_value();
	I.fitness = I.costs + I.lamda*I.over_load;
	return I.fitness;
}

void MAENS::get_init_fitness(CARP_INDIVIDUAL& I) {
	I.get_all_value();
	init_lamda(I);
	get_fitness(I);
}

// void MAENS::test_distance_to_best(){
//   // cout<<"distance : " <<solution_distance(best_individual,child)<<endl;
//   cout<<"distance : " <<Jaccard_distance(best_individual,child)<<endl;
//   if(Jaccard_distance(best_individual,child)==1){
//     show(best_individual.route);
//     cout<<endl;
//     show(child.route);
//     exit(0);
//   }
// }

// bool MAENS::update_best() {
//   // cout<<"update_best"<<swarm[0].costs<<"  "<<swarm[0].over_load<<endl;
//   // cout<<best_individual.costs;
//   if(swarm[0].costs < best_individual.costs){
//     // cout<<swarm[0].costs;
//     if(swarm[0].over_load<=0){
//       // cout<<swarm[0].costs;
//       best_individual = swarm[0];
//       show_best();
//       return 1;
//     }
//   }
//   return 0;
// }

// void MAENS::show_swarm() {
//   cout<<"\n************ swarm ***********************";
//   for(auto I:swarm) {
//     I.show_all();
//   }
//   cout<<"****************************************\n";
// }

// void MAENS::show_best() {
//   // cout<<"\n************ best *********************"<<endl;
//   cout<<fixed<<"new best "<<best_individual.fitness<<endl;
//   //best_individual.show_all();
//   // cout<<"****************************************\n";
// }

// void MAENS::show_swarm_fitness() {
//   cout<<"\n************ swarm fitness ***********\n";
//   for (auto I: swarm) {
//     cout<<I.fitness<<" ";
//   }
//   cout<<endl;
//   cout<<"****************************************\n";
// }

// void MAENS::show_all() {
//   show_swarm();
//   show_best();
//   for (auto r:record) {
//     cout<<r.first<<"\t"<<r.second<<endl;
//   }
// }

// bool MAENS::is_stop(int iterator) {
//   switch (this->end_type) {
//     case 0 :
//       return (iterator>=max_iteration);
//       break;
//     case 1 :
//       return (run_time>=max_time);
//       break;
//     case 2 :
//       return ((iterator>=max_iteration || run_time>=max_time) );
//       break;
//     default : return 0;
//   }
// }

// double MAENS::Jaccard_distance(CARP_INDIVIDUAL& I,CARP_INDIVIDUAL& S){
//   //Jaccard distance the close
//   I.del_double_zero();
//   S.del_double_zero();
//   I.split_to_subroute();
//   S.split_to_subroute();
//   double min_i,all_min,ij_iter;
//   int total_inter_num;
//   total_inter_num = 0;
//   all_min = 1;
//   for (auto& IS:I.sub_route) {
//     min_i = 1;
//     for (auto& SS:S.sub_route) {
//       ij_iter = 1.0-(double)count_AnB(IS.route,SS.route)/count_AUB(IS.route,SS.route);
//       // show(SS.route);
//       // show(IS.route);
//       // cout<<count_AnB(IS.route,SS.route)<<"/"<<count_AUB(IS.route,SS.route)<<"\t"<<ij_iter<<"\t"<<min_i<<"\t"<<all_min<<endl;
//       // cout<<ij_iter<<endl;
//       if(ij_iter<min_i) {
//         min_i = ij_iter;
//       }
//     }
//     if(min_i<all_min) {
//       all_min = min_i;
//     }
//   }
//   // cout<<total_inter_num<<endl;
//   // average
//   // return (double)total_inter_num/problem->task_number;
//   return all_min;
// }

// double MAENS::solution_distance(CARP_INDIVIDUAL& I,CARP_INDIVIDUAL& S){
//   if(I.sub_route_num==0) {
//     I.split_to_subroute();
//   }
//   if(S.sub_route_num==0) {
//     S.split_to_subroute();
//   }
//   int max_i,ij_iter,total_inter_num;
//   total_inter_num = 0;
//   for (int i=0; i<I.sub_route_num; i++) {
//     max_i = 0;
//     for (int j=0; j<S.sub_route_num; j++) {
//       ij_iter = intersection_number_set(I.sub_route[i],S.sub_route[j]);
//       if(ij_iter>max_i) {
//         max_i = ij_iter;
//       }
//     }
//     total_inter_num += max_i;
//   }
//   // cout<<total_inter_num<<endl;
//   return (double)total_inter_num/problem->task_number;
// }

// int MAENS::intersection_number_set(CARP_INDIVIDUAL& I,CARP_INDIVIDUAL& S){
//   int equal_number=0;
//   int ip;
//   for(auto i :I.route) {
//     if(i==0) continue;
//     ip=problem->get_invert(i);
//     for (auto j: S.route) {
//       if(j==0) continue;
//       if(ip==j) equal_number++;
//       if(i==j) equal_number++;
//     }
//   }
//   return equal_number;
// }

void MAENS::update_swarm_fitness() {
	for (auto &I: swarm) {
		I.lamda = lamda;
		get_fitness(I);
	}
}

void MAENS::update_lamda(CARP_INDIVIDUAL& I,bool is_updated){
	if(is_updated){
		unupdate = 1;
		if(unupdated_time>5){
			I.lamda /= 2;
			get_fitness(I);
			// update_swarm_fitness();
		}
	} else {
		unupdated_time = 0;
		unupdate++;
		if(unupdate>5){
			I.lamda *= 2;
			get_fitness(I);
			// update_swarm_fitness();
		}
	}
}

void MAENS::update_lamda(bool is_updated){
	for (auto &I: swarm) {
		update_lamda(I,is_updated);
	}
	// if(is_updated){
	//   unupdate = 1;
	//   if(unupdated_time>5){
	//     lamda /= 2;
	//     update_swarm_fitness();
	//   }
	// } else {
	//   unupdated_time = 0;
	//   unupdate++;
	//   if(unupdate>5){
	//     lamda *= 2;
	//     update_swarm_fitness();
	//   }
	// }
}

void MAENS::run(){
	clock_t start_run,end_run;
	double run_time;
	start_run = clock();
	iteration = 0;
	init_swarm_PSrand();
	unupdate = 1;
	unupdated_time = 0;
	while(iteration<max_iteration) {
		main_loop(); 
		cout<<iteration<<"=> \n";
		show_swarm_costs();
		// cout<<"*"<<iteration<<endl;
		end_run  = clock();
		run_time = (double)(end_run - start_run) / CLOCKS_PER_SEC;
		update_lamda(update_best());
		// update_best();
		if(is_record(iteration)) {
			record_best(iteration);
		}
		if(is_stop(iteration)) {
			record_best(iteration);
			break;
		}
		iteration++;
	}
}

#endif
