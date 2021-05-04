#ifndef EA_H_
#define EA_H_
#include <algorithm>
#include <iostream>
#include <dirent.h>
#include <fstream>
#include "../bin/general.hpp"
#include "../problem/problem.hpp"
#include "../problem/CARP.hpp"
#include "../heuristic/heuristic.hpp"
#include "../bin/CARP_individual.hpp"
#include "../move/move.hpp"
#include "../local_search/local_search.hpp"
//#include "../local_search/best_feasible_LS.hpp"
#include "../move/relocate.hpp"
// #include "../others/partial_reconstruct.hpp"

using namespace std;
typedef pair<int,VType> mark;


class EA4CARP{
	public:
void init_lamda(CARP_INDIVIDUAL& I);
		CARP *problem;
		int popsize;      // the size of population
		vector<CARP_INDIVIDUAL> swarm;  // main swarm
		int achive_size;      // the size of population
		vector<CARP_INDIVIDUAL> diversity_achive;// main swarm
		vector<CARP_INDIVIDUAL> children; // children
		CARP_INDIVIDUAL child;
		CARP_INDIVIDUAL best_individual;
		CARP_INDIVIDUAL best_solution;
		CARP_INDIVIDUAL candidate_individual;
	//	LocalSearch * LS;
		int LB;
		vector<mark> record;//iterative optimal value
		double alp;
		int fea_count;
		int max_ite;
		int max_fun_cal;
		int iteration ;
		double run_time;
		void init_swarm();
		void init_swarm_rand_second_half();
		void init_swarm_three_and_rand();
		void init_swarm_with_LS();
		void init_swarm_rand();
		void init_swarm_rand(vector<CARP_INDIVIDUAL>& swarm);
		void init_swarm_PSrand(vector<CARP_INDIVIDUAL>& POP);
		void init_swarm_PSrand();
		void test_distance_to_best();
		void set_popsize(int popsize);
		int unimproved;
		void main_loop_merge_halved();
		int end_type;     // the end type of algorithm, 0 for max_iteration,1 fo    r max_time, 2 for the min of them
		int max_iteration;// the max iteration
		int max_time;     // the max time
		int parent1,parent2; // for Crossover
		void record_best(int iterator);
		bool is_record(int iterator);
		bool is_stop(int iterator);
		bool not_in();
		bool not_in_swarm(CARP_INDIVIDUAL& P);
		bool not_in_swarm_except_k(CARP_INDIVIDUAL& P,int k);
		bool not_in_achive(CARP_INDIVIDUAL& P);
		bool update_achive(CARP_INDIVIDUAL& P);
		bool achive_to_swarm();
		bool update_best();
		void show_all();
		void show_best();
		void show_swarm_fitness();
		void check();
		void rand_second_half_change();
		void add_to_swarm();
	  void second_half_change_min_distance();	
		void show_swarm();
		void child_get_fitness();
		double solution_distance(CARP_INDIVIDUAL& I,CARP_INDIVIDUAL& S);
		double Jaccard_distance(CARP_INDIVIDUAL& I,CARP_INDIVIDUAL& S);
		double min_swarm_distance();
		double max_swarm_distance();
		int intersection_number_set(CARP_INDIVIDUAL& I,CARP_INDIVIDUAL& S);
		void SBX();
		void SBX(CARP_INDIVIDUAL *p1,CARP_INDIVIDUAL *p2);
		void order_crossover_subroute_PS(CARP_INDIVIDUAL *p1,CARP_INDIVIDUAL *p2);       //order crossover
		void order_crossover_subroute_PS();       //order crossover
		void order_crossover(CARP_INDIVIDUAL *p1,CARP_INDIVIDUAL *p2);       //order crossover
		void order_crossover();       //order crossover
		void order_crossover_subroute_based(CARP_INDIVIDUAL *p1,CARP_INDIVIDUAL *p2);
		void order_crossover_subroute_based();
		void order_crossover_subroute_PSE(CARP_INDIVIDUAL *p1,CARP_INDIVIDUAL *p2);
		void order_crossover_subroute_PSE();
		void order_crossover_subroute_PSR(CARP_INDIVIDUAL *p1,CARP_INDIVIDUAL *p2);
		void order_crossover_subroute_PSR();
		void rand_select();
		void tournament_select();
		void save_date();
		void get_fitness_for_swarm();
		int union_num(vector<int>& A,vector<int>& B);
		int intersection_num(vector<int>& A,vector<int>& B);
		double Jaccard_sim(vector<int>& A,vector<int>& B);
		double I_sim(CARP_INDIVIDUAL& I,CARP_INDIVIDUAL& S);
		double d_sim(CARP_INDIVIDUAL& I,CARP_INDIVIDUAL& S);
		double swarm_distance();
		virtual void local_search();
		virtual void main_loop();
		virtual void run();
		// virtual void get_fitness();
		virtual VType get_fitness(CARP_INDIVIDUAL& I);
		EA4CARP();
		EA4CARP(CARP* P);
		EA4CARP(const EA4CARP& algorithm);
		EA4CARP(int popsize);
		EA4CARP(int _popsize,int _end_type,int _max_iteration,int _max_time);
		EA4CARP& operator=(const EA4CARP&);
		~EA4CARP();
}
;
EA4CARP::EA4CARP(){}
EA4CARP::EA4CARP(CARP* P){
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

EA4CARP::EA4CARP(int _popsize=3):popsize(_popsize){
	swarm.resize(popsize);
	children.resize(popsize);
}

EA4CARP::EA4CARP(int _popsize,int _end_type,int _max_iteration,int _max_time) : popsize(_popsize),end_type(_end_type),max_iteration(_max_iteration),max_time(_max_time) {
	best_individual.problem = problem;
	best_individual.fitness = INF;
}

EA4CARP::~EA4CARP(){}

void EA4CARP::tournament_select() {
	// to parent1,parent2
	int candidate1,candidate2;	
	rand_select();
	candidate1 = swarm[parent1]<swarm[parent2]?parent1:parent2;
	rand_select(); 
	candidate2 = swarm[parent1]<swarm[parent2]?parent1:parent2;
	parent1 = candidate1;
	parent2 = candidate2;
}

void EA4CARP::save_date() {
	// to parent1,parent2
	DIR *dir;
	problem->set_save_file_name();
	cout<<problem->save_filename<<endl;
	string dir_name;
	int pos = problem->save_filename.rfind("/");
	dir_name = problem->save_filename;
	dir_name.resize(pos+1);
	cout<<dir_name;
	string cmd = "mkdir -p " + dir_name;
	if ((dir=opendir(dir_name.c_str())) == NULL) {
		// system(cmd.c_str());      //system( s.c_str() );
		cout<<"error to open save file \n";
	}
	fstream _file;
	string filename = problem->save_filename;
	int k;
	k = time(0);
	filename.insert(filename.rfind("."),"_"+to_string(k));
	_file.open(filename.c_str(),ios::out);
	if(!_file) {
		cout<< "Cannot find file:"<<problem->save_filename<<endl;
		exit(-1);
	}
	_file<<"best value: ";
	_file<<best_individual.fitness<<endl;
	_file<<"over_load: ";
	_file<<best_individual.over_load<<endl;
	_file<<"best solution : "<<endl;
	for (int i=0; i<best_individual.route.size(); i++) {
		_file<<best_individual.route[i]<<" ";
	}
	_file<<endl;
	_file<<"record: "<<endl;
	for (int i=0; i<record.size(); i++) {
		_file<<record[i].first;
		_file<<" ";
		_file<<record[i].second;
		_file<<endl;
	}
	_file<<"time used : ";
	_file<<run_time<<endl;
	_file.close();
}

void EA4CARP::set_popsize(int ps) {
	popsize = ps;
	swarm.resize(popsize);
	children.resize(popsize);
}

void EA4CARP::rand_select() {
	// to parent1,parent2
	parent1 = rand()%popsize;
	parent2 = rand()%(popsize-1);
	if (parent2 >= parent1) 
		parent2 += 1;
}

void rand_select_sort_2(int& min,int& max,int n){
	min = rand()%(n);
	max = rand()%(n-1);
	if(max>=min) {
		max += 1;
	}else{
		int s = min;
		min = max;
		max = s;	 
	}
}

void EA4CARP::order_crossover_subroute_PSR(){
	rand_select(); 
	CARP_INDIVIDUAL *p1 = &swarm[parent1];
	CARP_INDIVIDUAL *p2 = &swarm[parent2];
	order_crossover_subroute_PSR(p1,p2);
}

void EA4CARP::order_crossover_subroute_PSR(CARP_INDIVIDUAL *p1,CARP_INDIVIDUAL *p2){
	p1->route_to_tour();
	p2->route_to_tour();
	vector<int> C,C2;
	// rand select k subroute from route
	int k = rand()%p1->sub_route_num+1;
	p1->rand_get_part_route(k);
	C = p1->part_route;
	CARP_INDIVIDUAL I(problem);
	I.set_route(C);
	child.sub_route_num = 1;
	child.sub_route.clear();
	child.sub_route.push_back(I);
	p2->rand_shuffle_sub();
	p2->part_route_not_in_edges(C);
	C2 = p2->part_route;
	PathScanningRand PSR(problem);
	if(C2.size()>0){
		PathScanning H(problem);
		H.get_best_solution(C2);
		H.get_costs();
		I.set_route(H.best_result);
		get_fitness(I);
		for (int i=0; i<100; i++) {
			// H.path_scanning_rand(C2);
			PSR.get_best_solution(C2);
			PSR.get_costs();
			if(PSR.costs<I.fitness) {
				I.set_route(PSR.best_result);
				get_fitness(I);
			}
			// PSR.get_costs();
			// if(H.costs<I.fitness) {
			//   I.set_route(H.best_result);
			//   get_fitness(I);
			// }
		}
		PSR.get_best_solution(C2);
		I.set_route(H.best_result);
		// I._1();
		child.sub_route.push_back(I);
		child.sub_route_num++;
	}	
	child.merge_subroute();
	child.del_double_zero();
}

void EA4CARP::order_crossover_subroute_PSE(){
	rand_select(); 
	CARP_INDIVIDUAL *p1 = &swarm[parent1];
	CARP_INDIVIDUAL *p2 = &swarm[parent2];
	order_crossover_subroute_PSE(p1,p2);
}

void EA4CARP::order_crossover_subroute_PSE(CARP_INDIVIDUAL *p1,CARP_INDIVIDUAL *p2){
	// rand k in p1 and PSE with other
	p1->route_to_tour();
	p1->split_to_subroute();
	p2->route_to_tour();
	vector<int> C;
	vector<int> C2;
	int k = rand()%p1->sub_route_num+1;
	p1->rand_get_part_route(k);
	C = p1->part_route;
	CARP_INDIVIDUAL I(problem);
	I.set_route(C);
	child.sub_route_num = 1;
	child.sub_route.clear();
	child.sub_route.push_back(I);
	p2->part_route_not_in_edges(C);
	C2 = p2->part_route;
	if(C2.size()>0){
		// HEURISTIC H(problem);
		PathScanningEllipse H(problem);
		// H.path_scanning_ellipses(C2);
		H.get_best_solution();
		H.get_costs();
		I.set_route(H.best_result);
		get_fitness(I);
		for (int i=0; i<100; i++) {
			H.get_best_solution();
			// H.path_scanning_ellipses(C2);
			H.get_costs();
			if(H.costs<I.fitness) {
				I.set_route(H.best_result);
				get_fitness(I);
			}
		}
		// H.path_scanning_rand(C2);
		H.get_best_solution();
		I.set_route(H.best_result);
		child.sub_route.push_back(I);
		child.sub_route_num++;
	}	
	child.merge_subroute();
	child.del_double_zero();
	get_fitness(child);
}

void EA4CARP::order_crossover_subroute_PS(){
	rand_select(); 
	CARP_INDIVIDUAL *p1 = &swarm[parent1];
	CARP_INDIVIDUAL *p2 = &swarm[parent2];
	order_crossover_subroute_PS(p1,p2);
}

void EA4CARP::order_crossover_subroute_PS(CARP_INDIVIDUAL *p1,CARP_INDIVIDUAL *p2){
	p1->route_to_tour();
	p2->route_to_tour();
	vector<int> C,C2;
	// rand select k subroute from route
	// construct two subroute merge to one
	// first 
	int k = rand()%p1->sub_route_num+1;
	p1->rand_get_part_route(k);
	C = p1->part_route;
	CARP_INDIVIDUAL I(problem);
	I.set_route(C);
	child.sub_route_num = 1;
	child.sub_route.clear();
	child.sub_route.push_back(I);
	// second
	p2->rand_shuffle_sub();
	p2->part_route_not_in_edges(C);
	C2 = p2->part_route;
	if(C2.size()>0){
		PathScanning H(problem);
		// H.path_scanning(C2);
		H.get_best_solution(C2);
		I.set_route(H.best_result);
		child.sub_route.push_back(I);
		child.sub_route_num++;
	}	
	// merge
	child.merge_subroute();
	child.del_double_zero();
	get_fitness(child);
}

void EA4CARP::order_crossover_subroute_based(){
	rand_select(); 
	CARP_INDIVIDUAL *p1 = &swarm[parent1];
	CARP_INDIVIDUAL *p2 = &swarm[parent2];
	order_crossover_subroute_based(p1,p2);
}

void EA4CARP::order_crossover_subroute_based(CARP_INDIVIDUAL *p1,CARP_INDIVIDUAL *p2){
	p1->route_to_tour();
	p2->route_to_tour();
	vector<int> C,C2;
	// rand select k subroute from route
	// construct two subroute merge to one
	// first 
	int k = rand()%p1->sub_route_num+1;
	p1->rand_get_part_route(k);
	C = p1->part_route;
	CARP_INDIVIDUAL I(problem);
	I.set_route(C);
	child.sub_route_num = 1;
	child.sub_route.clear();
	child.sub_route.push_back(I);
	// second
	p2->rand_shuffle_sub();
	p2->part_route_not_in_edges(C);
	C2 = p2->part_route;
	if(C2.size()>0){
		I.set_tour(C2);
		I.Ulusoy_split();
		child.sub_route.push_back(I);
		child.sub_route_num++;
	}	
	child.is_sub = 0;
	// merge
	child.merge_subroute();
	child.del_double_zero();
	get_fitness(child);
}

void EA4CARP::order_crossover(){
	order_crossover(&swarm[parent1],&swarm[parent2]);
}

void EA4CARP::order_crossover(CARP_INDIVIDUAL *p1,CARP_INDIVIDUAL *p2){
	p1->route_to_tour();
	p2->route_to_tour();
	vector<int> C(p2->tour);
	int p,q;
	int n = p1->tour.size();
	// n-1 for the do-while 
	rand_select_sort_2(p,q,n-1);
	int j = q+1,ip;
	if (n==0) cout<<"may error tour.size()==0"<<endl;
	//for (int i=q+1;i%n!=q; i++) {
	int i=q+1;
	do{	
		ip = i%n;
		bool in = 0;
		for (int k=p; k<=q; k++) {
			in |= (C[k] == p1->tour[ip]);
			in |= ((*problem)[C[k]].invert== p1->tour[ip]);
		}
		if (in==0){
			//cout<<p1->tour[ip]<<endl;
			C[j%n] = p1->tour[ip];
			j++;
			if(j%n==p) break;
		}
		i++;
	}while(i%n!=(q+1)%n);
	//}while(((p!=n)&&(i%n!=q+1))||((i%n==0)&&(p==n)));
	child.set_tour(C);
	// if(child.is_unique()==0){
	// 	cout<<"\nchild not unique"<<endl;
	// 	show(p1->tour);
	// 	show(p2->tour);
	// 	show(C);
	// 	cout<<C[p]<<"**"<<C[q]<<endl;
	// 	cout<<p<<"**"<<q<<endl;
	// 	exit(0);
	// }
	child.Ulusoy_split();
	get_fitness(child);
}

void EA4CARP::record_best(int iterator) {
	record.push_back(make_pair(iterator,best_individual.fitness));
}

bool EA4CARP::not_in_achive(CARP_INDIVIDUAL& P) {
	for(auto	I:diversity_achive) {
		if(I.fitness==P.fitness){
			return 0;
		}
	}
	return 1;
}

bool EA4CARP::not_in_swarm(CARP_INDIVIDUAL& P) {
	for(auto	I:swarm) {
		if(I.fitness==P.fitness){
			return 0;
		}
	}
	return 1;
}

bool EA4CARP::not_in_swarm_except_k(CARP_INDIVIDUAL& P,int k) {
	for (int i = 0; i < popsize; ++i) {
		if(i!=k&&swarm[i].fitness==P.fitness){
			return 0;
		}
	}
	return 1;
}

bool EA4CARP::achive_to_swarm() {
	vector<int> selected;
	RandPermute(selected,achive_size);
	for (int i=popsize/2; i<popsize; i++) {
		swarm[i].problem = problem;
		swarm[i] = diversity_achive[i]; 
	}	
	sort(swarm.begin(),swarm.end());
	return 0;
}

bool EA4CARP::update_achive(CARP_INDIVIDUAL& P) {
	if(not_in_achive(P)) {
		if(achive_size<3*popsize){
			diversity_achive.push_back(P);
		} else {
			sort(diversity_achive.begin(),diversity_achive.end());
			if(diversity_achive[achive_size-1] < P){
				diversity_achive[achive_size-1] = P;
				return 1;
			}
		}
	}
	return 0;
}

bool EA4CARP::not_in() {
	for(auto	I:swarm) {
		// if(I.fitness==child.fitness)
		//   return 0;
		// if(Jaccard_distance(I,child)<0.1){
		//   return 0;
		// }
		if(Jaccard_distance(I,child)==0
				&&(I.fitness==child.fitness)
				){
			return 0;
		}
	}
	return 1;
}

bool EA4CARP::is_record(int iterator) {
	if(iterator%30==0) 
		return 1;
	return 0;
}

void EA4CARP::check() {
	for (int i=0; i<popsize; i++) {
		if(swarm[i].is_unique()==0){
			cout<<"\nNot unique swarm "<<i<<endl;
			swarm[i].show_all();
			exit(0);
		}
	}
	if(child.is_unique()==0){
		cout<<"not unique child"<<endl;
		child.show_all();
		exit(0);
	}
}

void EA4CARP::init_swarm_three_and_rand() {
	for (int i=0; i<popsize; i++) {
		swarm[i].problem = problem;
		swarm[i].rand_init();
		get_fitness(swarm[i]);
		// update_achive(swarm[i]);
	}	
	swarm[0].PS_init();
	get_fitness(swarm[0]);
	swarm[1].AM_init();
	get_fitness(swarm[1]);
	// swarm[2].Ulusoy_PS_init();
	get_fitness(swarm[2]);
	sort(swarm.begin(),swarm.end());
	update_best();
	// show_swarm_fitness();
	// cout<<"best v "<<swarm[0].fitness<<endl;
}

void EA4CARP::init_swarm_rand_second_half() {
/*	for (int i=popsize/2; i<popsize; i++) {
		swarm[i].problem = problem;
		swarm[i].rand_init();
		LS = new BestFeasibleLocalSearch<RELOCATE>(swarm[i]);
		LS->while_move_of_all_routes();
			// swarm[i].merge_split();
			// swarm[i].while_relocate();
			// swarm[i].while_swap();
			// swarm[i].while_between_2opt();
			// swarm[i].inner_2opt();
		get_fitness(swarm[i]);
		update_achive(swarm[i]);
	}	*/
	sort(swarm.begin(),swarm.end());
	update_best();
}

void EA4CARP::init_swarm_rand() {
	init_swarm_rand(swarm);
}

void EA4CARP::init_swarm_rand(vector<CARP_INDIVIDUAL>& POP) {
	if (POP.size() == 0){
		POP.resize(popsize);
	}
	for (int i=0; i<popsize; i++) {
		POP[i].problem = problem;
		POP[i].rand_init();
		get_fitness(POP[i]);
		// update_achive(POP[i]);
	}	
	sort(POP.begin(),POP.end());
	update_best();
}

void EA4CARP::init_swarm_PSrand(vector<CARP_INDIVIDUAL>& POP) {
	if (POP.size() == 0){
		POP.resize(popsize);
	}
	for (int i=0; i<popsize; i++) {
		POP[i].problem = problem;
		POP[i].PS_rand_init();
		get_fitness(POP[i]);
		cout<<POP[i].fitness<<endl;
		// update_achive(POP[i]);
	}	
	sort(POP.begin(),POP.end());
	update_best();
}

void EA4CARP::init_swarm_PSrand() {
	init_swarm_PSrand(swarm);
}
void EA4CARP::init_swarm() {
	init_swarm_rand();
	PathScanning H(problem);
	// H.path_scanning();
	H.get_best_solution();
	CARP_INDIVIDUAL C(problem);
	C.set_route(H.best_result);
	C.get_all_value();
	swarm[0] = C;
	get_fitness_for_swarm();
	sort(swarm.begin(),swarm.end());
	check();
	update_best();
}

void EA4CARP::get_fitness_for_swarm(){
	for (auto &I:swarm) {
		get_fitness(I);
	}
}

void EA4CARP::init_swarm_with_LS() {
	init_swarm_rand();
	// HEURISTIC H(problem);
	// H.path_scanning();
	// CARP_INDIVIDUAL C(problem);
	// C.set_route(H.best_result);
	// C.get_all_value();
	// swarm[0] = C;
/*
	for (int i=0; i<popsize; i++) {
		// LS = new BEST_RELOCATE_FLS(swarm[i]);
		LS = new BestFeasibleLocalSearch<RELOCATE>(swarm[i]);
		int it=1;
		while(++it<10){
			LS->while_move_of_all_routes();
			// LS.while_relocate();
			// swarm[i].merge_split();
			// swarm[i].while_relocate();
			// swarm[i].while_swap();
			// swarm[i].while_between_2opt();
			// swarm[i].inner_2opt();
		}
		// (swarm[i]._21());
		// while(swarm[i]._123()){}
		// swarm[i].local_search();
		get_fitness_for_swarm();
		cout<<swarm[i].fitness<<endl;
		// swarm[i].get_best_in_sort();
	}*/
	get_fitness_for_swarm();
	sort(swarm.begin(),swarm.end());
	check();
	update_best();
}

void EA4CARP::SBX(){
	// sequence based crossover
	rand_select(); 
	SBX(&swarm[parent1],&swarm[parent2]);
}

void EA4CARP::SBX(CARP_INDIVIDUAL *p1,CARP_INDIVIDUAL *p2){
	// sequence based crossover
	p1->del_double_zero();
	p1->split_to_subroute();
	p2->del_double_zero();
	p2->split_to_subroute();
	vector<int> C1(p2->tour);
	vector<int> C;
	child = *p1;
	// child.show_all();
	int k1 = rand()%p1->sub_route_num;
	vector<int>* route_p1 = 
		&p1->sub_route[k1].route;
	vector<int>* route_p2 = 
		&p2->sub_route[rand()%p2->sub_route_num].route;
	int p,q;
	// cout<<route_p2->size()<<endl;
	p = rand()%(route_p1->size()-2)+1;
	q = rand()%(route_p2->size()-2)+1;
	vector<int> R11;
	vector<int> R12;
	vector<int> R21;
	vector<int> R22;
	R11.insert(R11.end(),
			route_p1->begin(), route_p1->begin()+p); 
	R12.insert(R12.end(),
			route_p1->begin()+p, route_p1->end()); 
	R21.insert(R21.end(),route_p2->begin(), route_p2->begin()+q); 
	R22.insert(R22.end(),route_p2->begin()+q, route_p2->end()); 

	// R11-R22
	// show(R11);
	// show(R22);
	A_B(R11,R22,problem);
	// show(R11);
	C.insert(C.end(),R11.begin(), R11.end()); 
	C.insert(C.end(),R22.begin(), R22.end()); 
	// copy(R22.begin(), R22.end(),C.end()); 
	// show(C);
	// child.sub_route[k1].set_route(C);
	// R12-R22
	A_B(R12,R22,problem);
	// cout<<"*******\n";
	// show(R11);
	// show(R12);
	// show(R22);
	// show(C);

	for (auto &I: child.sub_route) {
		// show(R22);
		// show(I.route);
		A_B(I.route,R22,problem);
		// show(I.route);
	}
	child.sub_route[k1].set_route(C);

	// best_insert
	for (auto e: R12) {
		if(e==0) continue;
		// cout<<e<<" ";
		child.get_best_inserts(e);
		if(child.best_inserts.size()==1){
			child.insert(child.best_inserts[0]);
		} else {
			if(child.best_inserts.size()>1){
				child.insert(child.best_inserts[rand()%child.best_inserts.size()]);
			}
		}
	}
	child.merge_subroute();
	child.get_all_value();
	// copy(v1.begin(), v1.end(), v2.begin()); 
	// child.set_tour(C1);
	// if(child.is_unique()==0){
	//   cout<<"\nchild not unique"<<endl;
	//   show(p1->tour);
	//   show(p2->tour);
	//   show(C1);
	//   cout<<C1[p]<<"**"<<C1[q]<<endl;
	//   cout<<p<<"**"<<q<<endl;
	//   exit(0);
	// }
	// child.Ulusoy_split();
	// child.get_fitness();
}

void EA4CARP::main_loop_merge_halved() {
	// LocalSearch child_LS(child);
	// PART_RECONSTRUCT PR(child);
/*	BestFeasibleLocalSearch<RELOCATE> child_LS(child);
	for (int i=0; i<popsize; i++) {
		// order_crossover();
		SBX();
		if (rand()%100<20){
			// BEST_RELOCATE_FLS child_LS(child);
			// BestFeasibleLocalSearch<RELOCATE> child_LS(child);
			// BestFeasibleLocalSearch<RELOCATE> LS(*I);
			child_LS.while_move_of_all_routes();
			// while(child_LS.while_relocate()){};
			// while(child_LS.while_swap_improve()){};
			// // while(child_LS.while_between_2opt()){};
			// while(child_LS.while_move_best_improve(2)){};
			// while(child_LS.while_move_best_improve(6)){};
			// PR.byPSR();
			// PR.byPS();
			// PR.byPSE(100);
			// child_LS.while_relocate();
			// child_LS.while_swap_improve();
			// child_LS.while_between_2opt();
			// child_LS.while_move_best_improve(2);
			// while(child_LS.while_move_best_improve(6)){};
		}
		child.get_best_in_sort();
		// child.get_best_in_sort();
		child_get_fitness();
		update_achive(child);
		// cout<<child.fitness<<endl;
		add_to_swarm();
	}*/

	sort(swarm.begin(),swarm.end());
	swarm.resize(popsize);
	unimproved++;
	if (unimproved>10){
		// child.lamda = 
		// cout<<child.lamda<<endl;
		// alp /=2;
	}
	if (unimproved>100){
		unimproved = 0;
		// achive_to_swarm();
		// init_swarm_with_LS();
		init_swarm();
		// init_swarm_rand_second_half();
	}
	// if(update_best()) {
	//   update_achive(best_individual);
	//   unimproved=0;
	//   // cout<<"min_distance "<< min_swarm_distance()<<endl;
	//   // exit(0);
	// }
}

void EA4CARP::main_loop() {
	// LocalSearch child_LS(child);
	// PART_RECONSTRUCT PR(child);
	// order_crossover_subroute_based();
	// order_crossover_subroute_PS();
	// order_crossover_subroute_PSR();
	// order_crossover_subroute_PSE();
	// order_crossover();
	SBX();
	// child_LS.while_relocate();
	// child_LS.while_between_2opt();
	// child_LS.while_swap_improve();
	// child_LS.while_move_best_improve(2);
	// PR.byPS();
	// cout<<PR.improved<<endl;
	// child_LS.while_relocate();
	child_get_fitness();
	rand_second_half_change();
	if(update_best()) {
		cout<<"min_distance "<< min_swarm_distance()<<endl;
		cout<<"max_distance "<< max_swarm_distance()<<endl;
		// exit(0);
	}
}

double EA4CARP::max_swarm_distance() {
	double distance,max_distance;
	max_distance = 0;
	for (int i=0; i<popsize; i++) {
		for (int j=0; j<popsize; j++) {
			if(i==j) continue;
			distance = Jaccard_distance(swarm[i],swarm[j]);
			if(distance>max_distance) {
				max_distance = distance;
			}
		}
	}
	return max_distance;
}

double EA4CARP::min_swarm_distance() {
	double distance,min_distance;
	min_distance = 1;
	for (int i=0; i<popsize; i++) {
		for (int j=0; j<popsize; j++) {
			if(i==j) continue;
			distance = Jaccard_distance(swarm[i],swarm[j]);
			if(distance<min_distance) {
				min_distance = distance;
			}
		}
	}
	return min_distance;
}

void EA4CARP::second_half_change_min_distance() {
	int k;
	double distance,min_distance;
	min_distance = 1;
	k = popsize/2;
	for (int i=popsize/2; i<popsize; i++) {
	// for (int i=0; i<popsize; i++) {
		distance = Jaccard_distance(swarm[i],child);
		if(distance<min_distance) {
			k = i;
			min_distance = distance;
		}
	}
	if(child<swarm[k] && not_in()) {
		swarm[k] = child;
		test_distance_to_best();
		show_swarm_fitness();
	}
	sort(swarm.begin(),swarm.end());
}

void EA4CARP::add_to_swarm() {
	swarm.push_back(child);
}

void EA4CARP::rand_second_half_change() {
	int k = rand()%(popsize/2) + popsize/2;
	if(child<swarm[k]&&not_in()) {
		swarm[k] = child;
		// test_distance_to_best();
		// show_swarm_fitness();
	}
	sort(swarm.begin(),swarm.end());
}

void EA4CARP::child_get_fitness() {
	// child.lamda =3.2;
	child.get_all_value();
	child.fitness = child.costs + alp*child.over_load;
	// cout<<"**"<<child.costs<<child.over_load<<child.lamda<<child.fitness<<endl;
}

// void EA4CARP::get_fitness() {
// 	for (int i=0; i<popsize; i++) {
// 		swarm[i].get_fitness();
// 	}
// }

void EA4CARP::test_distance_to_best(){
	// cout<<"distance : " <<solution_distance(best_individual,child)<<endl;
	cout<<"distance : " <<Jaccard_distance(best_individual,child)<<endl;
	if(Jaccard_distance(best_individual,child)==1){
		show(best_individual.route);
		cout<<endl;
		show(child.route);
		exit(0);
	}
}

bool EA4CARP::update_best() {
	for (auto& I: swarm) {
		if(I.is_feasible() && I < best_individual){
			best_individual = I;
			show_best();
			return 1;
		}
	}
	return 0;
}

void EA4CARP::show_swarm() {
	cout<<"\n************ swarm ***********************";
	for(auto I:swarm) {
		I.show_all();
	}
	cout<<"****************************************\n";
}

void EA4CARP::show_best() {
	cout<<"\n************ best *********************";
	cout<<endl<<"new best "<<best_individual.fitness<<endl;
	cout<<"costs "<<best_individual.costs<<endl;
	cout<<"over load "<<best_individual.over_load<<endl;
	//best_individual.show_all();
	cout<<"****************************************\n";
}

void EA4CARP::show_swarm_fitness() {
	cout<<"\n************ swarm fitness ***********\n";
	for (auto I: swarm) {
		cout<<I.fitness<<" ";
	}
	cout<<endl;
	cout<<"****************************************\n";
}

void EA4CARP::show_all() {
	show_swarm();
	show_best();
	for (auto r:record) {
		cout<<r.first<<"\t"<<r.second<<endl;
	}
}

bool EA4CARP::is_stop(int iterator) {
	switch (this->end_type) {
		case 0 :
			return (iterator>=max_iteration);
			break;
		case 1 :
			return (run_time>=max_time);
			break;
		case 2 :
			return ((iterator>=max_iteration || run_time>=max_time) );
			break;
		default : return 0;
	}
}

int count_AnB(vector<int>&A,vector<int>&B){
	int count = 0;
	for (auto i: A) {
		if(i==0) continue;
		for (auto j: B) {
			if(j==0) continue;
			if(i==j){
				count++;
				break;
			}
		}
	}
	return count;
}

int count_AUB(vector<int>&A,vector<int>&B){
	int count = 0;
	for (auto i: A) {
		if(i==0) continue;
		count++;
	}
	for (auto j: B) {
		if(j==0) continue;
		count++;
	}
	int anb = count_AnB(A,B);
	return count-anb;
}

double EA4CARP::Jaccard_distance(CARP_INDIVIDUAL& I,CARP_INDIVIDUAL& S){
	//Jaccard distance the close
	I.del_double_zero();
	S.del_double_zero();
	I.split_to_subroute();
	S.split_to_subroute();
	double min_i,all_min,ij_iter;
	int total_inter_num;
	total_inter_num = 0;
	all_min = 1;
	for (auto& IS:I.sub_route) {
		min_i = 1;
		for (auto& SS:S.sub_route) {
			ij_iter = 1.0-(double)count_AnB(IS.route,SS.route)/count_AUB(IS.route,SS.route);
			// show(SS.route);
			// show(IS.route);
			// cout<<count_AnB(IS.route,SS.route)<<"/"<<count_AUB(IS.route,SS.route)<<"\t"<<ij_iter<<"\t"<<min_i<<"\t"<<all_min<<endl;
			// cout<<ij_iter<<endl;
			if(ij_iter<min_i) {
				min_i = ij_iter;
			}
		}
		if(min_i<all_min) {
			all_min = min_i;
		}
	}
	// cout<<total_inter_num<<endl;
	// average
	// return (double)total_inter_num/problem->task_number;
	return all_min;
}

double EA4CARP::solution_distance(CARP_INDIVIDUAL& I,CARP_INDIVIDUAL& S){
	if(I.sub_route_num==0) {
		I.split_to_subroute();
	}
	if(S.sub_route_num==0) {
		S.split_to_subroute();
	}
	int max_i,ij_iter,total_inter_num;
	total_inter_num = 0;
	for (int i=0; i<I.sub_route_num; i++) {
		max_i = 0;
		for (int j=0; j<S.sub_route_num; j++) {
			ij_iter = intersection_number_set(I.sub_route[i],S.sub_route[j]);
			if(ij_iter>max_i) {
				max_i = ij_iter;
			}
		}
		total_inter_num += max_i;
	}
	// cout<<total_inter_num<<endl;
	return (double)total_inter_num/problem->task_number;
}

int EA4CARP::intersection_number_set(CARP_INDIVIDUAL& I,CARP_INDIVIDUAL& S){
	int equal_number=0;
	int ip;
	for(auto i :I.route) {
		if(i==0) continue;
		ip=problem->get_invert(i);
		for (auto j: S.route) {
			if(j==0) continue;
			if(ip==j) equal_number++;
			if(i==j) equal_number++;
		}
	}
	return equal_number;
}

void EA4CARP::run(){
	alp = 0.75;
	PathScanning H(problem);
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
	init_swarm();
	// init_swarm_with_LS();
	// init_swarm_rand();
	unimproved = 0;
	int unupdate = 1;
	int upt = 0;
	while(iteration<max_iteration) {
		// main_loop(); 
		child.lamda = 0.2;
		main_loop_merge_halved();
		end_run  = clock();
		run_time = (double)(end_run - start_run) / CLOCKS_PER_SEC;
		// update_best();
		// cout<<iteration<<" got best : "<< best_individual.fitness<<"\r";
		// cout.flush();
		if(update_best()){
			unupdate = 1;
			if(upt>5){
				for (auto &I: swarm) {
					I.lamda /= 2;
					get_fitness(I);
				}
			}
		} else {
			upt = 0;
			unupdate++;
			if(unupdate>5){
				for (auto &I: swarm) {
					I.lamda *= 2;
					get_fitness(I);
				}
			}
		}
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

void EA4CARP::init_lamda(CARP_INDIVIDUAL& I) {
	DType Q = I.problem->Capacity;
	I.lamda = I.costs/Q*(best_individual.costs/I.costs+I.over_load/Q+1);
}

void EA4CARP::local_search() {

}

VType EA4CARP::get_fitness(CARP_INDIVIDUAL& I) {
	init_lamda(I);
	I.get_all_value();
	I.fitness = I.costs + I.lamda*I.over_load;
	return I.fitness;
}

int EA4CARP::union_num(vector<int>& A,vector<int>& B) {
	int count = 0;
	int flag = 1;
	for (auto i:A) {
		if (i==0) continue;
		flag = 1;
		for (auto j:B) {
			if (i==j||i==problem->get_invert(j)) {
				flag = 0;
				break;
			}
		}
		if(flag) count++;
	}
	count += B.size()-2;
	return count;
}

int EA4CARP::intersection_num(vector<int>& A,vector<int>& B) {
	int count = 0;
	for (auto i:A) {
		if (i==0) continue;
		for (auto j:B) {
			if (i==j||i==problem->get_invert(j)) count++;
		}
	}
	return count;
}

double EA4CARP::Jaccard_sim(vector<int>& A,vector<int>& B) {
	int a = intersection_num(A,B);
	int b = union_num(A,B);
	if (a==0 && b==0) return 1;
	// cout<<a<<" ,"<<b<<endl;
	return (1.0*a/b);
}

double EA4CARP::I_sim(CARP_INDIVIDUAL& I,CARP_INDIVIDUAL& S) {
	double v,max_v,sum_v=0;
	for (auto i:I.sub_route) {
		max_v = 0;
		for (auto j:S.sub_route) {
			v = Jaccard_sim(i.route,j.route);
			// cout<<v<<endl;
			// show(i.route);
			// show(j.route);
			// cout<<v<<endl;
			// exit(0);
			if (v>max_v) {
				max_v = v;
			}
		}
		sum_v += max_v;
	}
	// cout<<sum_v<<endl;
	return sum_v;
}

double EA4CARP::d_sim(CARP_INDIVIDUAL& I,CARP_INDIVIDUAL& S) {
	double sum_v=0.0;
	sum_v += I_sim(I,S);
	//cout<<"swarm_distance"<<sum_v;
	sum_v += I_sim(S,I);
	//cout<<"swarm_distance"<<sum_v;
	// cout<<sum_v<<endl;
	return (sum_v/2);
}

// double EA4CARP::swarm_distance() {
// 	double sum_v,min_v=INF;
// 	double v;
// 	for (auto& i:swarm) {
// 		i.split_to_subroute();
// 		min_v = 1.0*INF;
// 		for (auto& j:swarm) {
// 			j.split_to_subroute();
// 			if (i==j) continue;
// 			v = d_sim(i,j);
// 			// cout<<v<<endl;
// 			if (v<min_v) {
// 				min_v = v;
// 			}
// 		}
// 		sum_v += min_v;
// 	}
// 	return sum_v;
// }

double EA4CARP::swarm_distance() {
	double sum_v,max_v=0;
	double v;
	for (auto& i:swarm) {
		i.split_to_subroute();
		max_v = 0;
		for (auto& j:swarm) {
			j.split_to_subroute();
			if (i==j) continue;
			v = d_sim(i,j);
			// cout<<v<<endl;
			if (v>max_v) {
				max_v = v;
			}
		}
		sum_v += max_v;
	}
	return 1.0/sum_v;
}

#endif
