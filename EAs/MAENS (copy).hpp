#ifndef MAENS_H_
#define MAENS_H_
#include<algorithm>
#include<iostream>
#include <dirent.h>
#include <fstream>
using namespace std;
typedef pair<int,VType> mark;
// typedef pair<int,int> insert_pos;

class MAENS:public EA4CARP{
	public:
		// CARP *problem;
		// int popsize;      // the size of population
		// int ubntrial;
		// vector<CARP_INDIVIDUAL> swarm;  // main swarm
		// vector<CARP_INDIVIDUAL> children; // children
		// CARP_INDIVIDUAL child;
		// CARP_INDIVIDUAL Sx,Sl;
		// CARP_INDIVIDUAL best_individual;
		// CARP_INDIVIDUAL best_solution;
		// CARP_INDIVIDUAL candidate_individual;
		// vector<mark> record;//iterative optimal value
		// int max_ite;
		// int max_fun_cal;
		// int iteration ;
		// void init_lamda(CARP_INDIVIDUAL& I);
		// double run_time;
		// void init_swarm();
		void init_swarm_PSrand();
		// void init_swarm_with_LS();
		// void init_swarm_rand();
		// void test_distance_to_best();
		// void run();
		// void set_popsize(int popsize);
		// void main_loop();
		// int end_type;     // the end type of algorithm, 0 for max_iteration,1 fo    r max_time, 2 for the min of them
		// int max_iteration;// the max iteration
		// int max_time;     // the max time
		// int parent1,parent2; // for Crossover
		// void record_best(int iterator);
		// bool is_record(int iterator);
		// bool is_stop(int iterator);
		// bool not_in();
		bool child_not_in();
		// bool update_best();
		// void show_all();
		// void show_best();
		// void show_swarm_fitness();
		// void check();
		// void rand_second_half_change();
		// bool add_to_swarm(CARP_INDIVIDUAL& I);
		// bool rand_second_half_change(CARP_INDIVIDUAL& I);
		// void second_half_change_min_distance();	
		// void show_swarm();
		// void get_fitness();
		// void get_fitness(CARP_INDIVIDUAL& I);
		// double solution_distance(CARP_INDIVIDUAL& I,CARP_INDIVIDUAL& S);
		// double Jaccard_distance(CARP_INDIVIDUAL& I,CARP_INDIVIDUAL& S);
		// double min_swarm_distance();
		// double max_swarm_distance();
		// int intersection_number_set(CARP_INDIVIDUAL& I,CARP_INDIVIDUAL& S);
		// void order_crossover_subroute_PS(CARP_INDIVIDUAL *p1,CARP_INDIVIDUAL *p2);       //order crossover
		// void order_crossover_subroute_PS();       //order crossover
		// void order_crossover(CARP_INDIVIDUAL *p1,CARP_INDIVIDUAL *p2);       //order crossover
		void order_crossover();       //order crossover
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
	// best_solution.fitness = INF;
	// best_solution.costs   = INF;
	// child.problem = P;
	// popsize = 10;
	// end_type = 2;
	// max_iteration = 1000;
	// max_time = 600;
	// swarm.resize(popsize);
	// children.resize(popsize);
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

// void MAENS::tournament_select() {
//   // to parent1,parent2
//   int candidate1,candidate2;	
//   rand_select();
//   candidate1 = swarm[parent1]<swarm[parent2]?parent1:parent2;
//   rand_select(); 
//   candidate2 = swarm[parent1]<swarm[parent2]?parent1:parent2;
// }

// void MAENS::save_date() {
//   // to parent1,parent2
//   DIR *dir;
//   problem->set_save_file_name();
//   cout<<problem->save_filename<<endl;
//   string dir_name;
//   int pos = problem->save_filename.rfind("/");
//   dir_name = problem->save_filename;
//   dir_name.resize(pos+1);
//   cout<<dir_name;
//   string cmd = "mkdir -p " + dir_name;
//   if ((dir=opendir(dir_name.c_str())) == NULL) {
//     system(cmd.c_str());      //system( s.c_str() );
//   }
//   fstream _file;
//   string filename = problem->save_filename;
//   int k;
//   k = time(0);
//   filename.insert(filename.rfind("."),"_"+to_string(k));
//   _file.open(filename.c_str(),ios::out);
//   if(!_file) {
//     cout<< "Cannot find file:"<<problem->save_filename<<endl;
//     exit(-1);
//   }
//   _file<<"best value: ";
//   _file<<best_individual.fitness<<endl;
//   _file<<"over_load: ";
//   _file<<best_individual.over_load<<endl;
//   _file<<"best solution : "<<endl;
//   for (int i=0; i<best_individual.route.size(); i++) {
//     _file<<best_individual.route[i]<<" ";
//   }
//   _file<<endl;
//   _file<<"record: "<<endl;
//   for (int i=0; i<record.size(); i++) {
//     _file<<record[i].first;
//     _file<<" ";
//     _file<<record[i].second;
//     _file<<endl;
//   }
//   _file<<"time used : ";
//   _file<<run_time<<endl;
//   _file.close();
// }

// void MAENS::set_popsize(int ps) {
//   popsize = ps;
//   swarm.resize(popsize);
//   children.resize(popsize);
// }

// void MAENS::rand_select() {
//   // to parent1,parent2
//   parent1 = rand()%popsize;
//   parent2 = rand()%(popsize-1);
//   if (parent2 >= parent1) 
//     parent2 += 1;
// }

// void MAENS::order_crossover_subroute_PSR(){
//   rand_select(); 
//   CARP_INDIVIDUAL *p1 = &swarm[parent1];
//   CARP_INDIVIDUAL *p2 = &swarm[parent2];
//   order_crossover_subroute_PSR(p1,p2);
// }

// void MAENS::order_crossover_subroute_PSR(CARP_INDIVIDUAL *p1,CARP_INDIVIDUAL *p2){
//   p1->route_to_tour();
//   p2->route_to_tour();
//   vector<int> C,C2;
//   // rand select k subroute from route
//   int k = rand()%p1->sub_route_num+1;
//   p1->rand_get_part_route(k);
//   C = p1->part_route;
//   CARP_INDIVIDUAL I(problem);
//   I.set_route(C);
//   child.sub_route_num = 1;
//   child.sub_route.clear();
//   child.sub_route.push_back(I);
//   p2->rand_shuffle_sub();
//   p2->part_route_not_in_edges(C);
//   C2 = p2->part_route;
//   if(C2.size()>0){
//     HEURISTIC H(problem);
//     H.path_scanning_rand(C2);
//     H.get_costs();
//     I.set_route(H.result);
//     I.get_fitness();
//     for (int i=0; i<100; i++) {
//       H.path_scanning_rand(C2);
//       H.get_costs();
//       if(H.costs<I.fitness) {
//         I.set_route(H.result);
//         I.get_fitness();
//       }
//     }
//     H.path_scanning_rand(C2);
//     I.set_route(H.result);
//     // I._1();
//     child.sub_route.push_back(I);
//     child.sub_route_num++;
//   }	
//   child.merge_subroute();
//   child.del_double_zero();
// }

// void MAENS::order_crossover_subroute_PSE(){
//   rand_select(); 
//   CARP_INDIVIDUAL *p1 = &swarm[parent1];
//   CARP_INDIVIDUAL *p2 = &swarm[parent2];
//   order_crossover_subroute_PSE(p1,p2);
// }

// void MAENS::order_crossover_subroute_PSE(CARP_INDIVIDUAL *p1,CARP_INDIVIDUAL *p2){
//   // rand k in p1 and PSE with other
//   p1->route_to_tour();
//   p1->split_to_subroute();
//   p2->route_to_tour();
//   vector<int> C;
//   vector<int> C2;
//   int k = rand()%p1->sub_route_num+1;
//   p1->rand_get_part_route(k);
//   C = p1->part_route;
//   CARP_INDIVIDUAL I(problem);
//   I.set_route(C);
//   child.sub_route_num = 1;
//   child.sub_route.clear();
//   child.sub_route.push_back(I);
//   p2->part_route_not_in_edges(C);
//   C2 = p2->part_route;
//   if(C2.size()>0){
//     HEURISTIC H(problem);
//     H.path_scanning_ellipses(C2);
//     H.get_costs();
//     I.set_route(H.result);
//     I.get_fitness();
//     for (int i=0; i<100; i++) {
//       H.path_scanning_ellipses(C2);
//       H.get_costs();
//       if(H.costs<I.fitness) {
//         I.set_route(H.result);
//         I.get_fitness();
//       }
//     }
//     H.path_scanning_rand(C2);
//     I.set_route(H.result);
//     child.sub_route.push_back(I);
//     child.sub_route_num++;
//   }	
//   child.merge_subroute();
//   child.del_double_zero();
//   child.get_fitness();
// }

// void MAENS::order_crossover_subroute_PS(){
//   rand_select(); 
//   CARP_INDIVIDUAL *p1 = &swarm[parent1];
//   CARP_INDIVIDUAL *p2 = &swarm[parent2];
//   order_crossover_subroute_PS(p1,p2);
// }

// void MAENS::order_crossover_subroute_PS(CARP_INDIVIDUAL *p1,CARP_INDIVIDUAL *p2){
//   p1->route_to_tour();
//   p2->route_to_tour();
//   vector<int> C,C2;
//   // rand select k subroute from route
//   // construct two subroute merge to one
//   // first 
//   int k = rand()%p1->sub_route_num+1;
//   p1->rand_get_part_route(k);
//   C = p1->part_route;
//   CARP_INDIVIDUAL I(problem);
//   I.set_route(C);
//   child.sub_route_num = 1;
//   child.sub_route.clear();
//   child.sub_route.push_back(I);
//   // second
//   p2->rand_shuffle_sub();
//   p2->part_route_not_in_edges(C);
//   C2 = p2->part_route;
//   if(C2.size()>0){
//     HEURISTIC H(problem);
//     H.path_scanning(C2);
//     I.set_route(H.result);
//     child.sub_route.push_back(I);
//     child.sub_route_num++;
//   }	
//   // merge
//   child.merge_subroute();
//   child.del_double_zero();
//   child.get_fitness();
// }

// void MAENS::order_crossover_subroute_based(){
//   rand_select(); 
//   CARP_INDIVIDUAL *p1 = &swarm[parent1];
//   CARP_INDIVIDUAL *p2 = &swarm[parent2];
//   order_crossover_subroute_based(p1,p2);
// }

// void MAENS::order_crossover_subroute_based(CARP_INDIVIDUAL *p1,CARP_INDIVIDUAL *p2){
//   p1->route_to_tour();
//   p2->route_to_tour();
//   vector<int> C,C2;
//   // rand select k subroute from route
//   // construct two subroute merge to one
//   // first 
//   int k = rand()%p1->sub_route_num+1;
//   p1->rand_get_part_route(k);
//   C = p1->part_route;
//   CARP_INDIVIDUAL I(problem);
//   I.set_route(C);
//   child.sub_route_num = 1;
//   child.sub_route.clear();
//   child.sub_route.push_back(I);
//   // second
//   p2->rand_shuffle_sub();
//   p2->part_route_not_in_edges(C);
//   C2 = p2->part_route;
//   if(C2.size()>0){
//     I.set_tour(C2);
//     I.Ulusoy_split();
//     child.sub_route.push_back(I);
//     child.sub_route_num++;
//   }	
//   child.is_sub = 0;
//   // merge
//   child.merge_subroute();
//   child.del_double_zero();
//   child.get_fitness();
// }

// void MAENS::order_crossover(){
//   rand_select(); 
//   order_crossover(&swarm[parent1],&swarm[parent2]);
// }

// void MAENS::order_crossover(CARP_INDIVIDUAL *p1,CARP_INDIVIDUAL *p2){
//   p1->route_to_tour();
//   p2->route_to_tour();
//   vector<int> C(p2->tour);
//   int p,q;
//   int n = p1->tour.size();
//   // n-1 for the do-while 
//   rand_select_sort_2(p,q,n-1);
//   int j = q+1,ip;
//   if (n==0) cout<<"may error tour.size()==0"<<endl;
//   //for (int i=q+1;i%n!=q; i++) {
//   int i=q+1;
//   do{	
//     ip = i%n;
//     bool in = 0;
//     for (int k=p; k<=q; k++) {
//       in |= (C[k] == p1->tour[ip]);
//       in |= ((*problem)[C[k]].invert== p1->tour[ip]);
//     }
//     if (in==0){
//       //cout<<p1->tour[ip]<<endl;
//       C[j%n] = p1->tour[ip];
//       j++;
//       if(j%n==p) break;
//     }
//     i++;
//   }while(i%n!=(q+1)%n);
//   //}while(((p!=n)&&(i%n!=q+1))||((i%n==0)&&(p==n)));
//   child.set_tour(C);
//   if(child.is_unique()==0){
//     cout<<"\nchild not unique"<<endl;
//     show(p1->tour);
//     show(p2->tour);
//     show(C);
//     cout<<C[p]<<"**"<<C[q]<<endl;
//     cout<<p<<"**"<<q<<endl;
//     exit(0);
//   }
//   child.Ulusoy_split();
//   child.get_fitness();
// }


// void MAENS::SBX(){
//   // sequence based crossover
//   rand_select(); 
//   SBX(&swarm[parent1],&swarm[parent2]);
// }

// void MAENS::SBX(CARP_INDIVIDUAL *p1,CARP_INDIVIDUAL *p2){
//   // sequence based crossover
//   p1->del_double_zero();
//   p1->split_to_subroute();
//   p2->del_double_zero();
//   p2->split_to_subroute();
//   vector<int> C1(p2->tour);
//   vector<int> C;
//   child = *p1;
//   // child.show_all();
//   int k1 = rand()%p1->sub_route_num;
//   vector<int>* route_p1 = 
//     &p1->sub_route[k1].route;
//   vector<int>* route_p2 = 
//     &p2->sub_route[rand()%p2->sub_route_num].route;
//   int p,q;
//   // cout<<route_p2->size()<<endl;
//   p = rand()%(route_p1->size()-2)+1;
//   q = rand()%(route_p2->size()-2)+1;
//   vector<int> R11;
//   vector<int> R12;
//   vector<int> R21;
//   vector<int> R22;
//   R11.insert(R11.end(),
//       route_p1->begin(), route_p1->begin()+p); 
//   R12.insert(R12.end(),
//       route_p1->begin()+p, route_p1->end()); 
//   R21.insert(R21.end(),route_p2->begin(), route_p2->begin()+q); 
//   R22.insert(R22.end(),route_p2->begin()+q, route_p2->end()); 

//   // R11-R22
//   // show(R11);
//   // show(R22);
//   A_B(R11,R22,problem);
//   // show(R11);
//   C.insert(C.end(),R11.begin(), R11.end()); 
//   C.insert(C.end(),R22.begin(), R22.end()); 
//   // copy(R22.begin(), R22.end(),C.end()); 
//   // show(C);
//   // child.sub_route[k1].set_route(C);
//   // R12-R22
//   A_B(R12,R22,problem);
//   // cout<<"*******\n";
//   // show(R11);
//   // show(R12);
//   // show(R22);
//   // show(C);

//   for (auto &I: child.sub_route) {
//     // show(R22);
//     // show(I.route);
//     A_B(I.route,R22,problem);
//     // show(I.route);
//   }
//   child.sub_route[k1].set_route(C);

//   // best_insert
//   for (auto e: R12) {
//     if(e==0) continue;
//     // cout<<e<<" ";
//     child.get_best_inserts(e);
//     if(child.best_inserts.size()==1){
//       child.insert(child.best_inserts[0]);
//     } else {
//       if(child.best_inserts.size()>1){
//         child.insert(child.best_inserts[rand()%child.best_inserts.size()]);
//       }
//     }
//   }
//   child.merge_subroute();
//   child.get_all_value();
// }

// void MAENS::record_best(int iterator) {
//   record.push_back(make_pair(iterator,best_individual.fitness));
// }

// bool MAENS::child_not_in() {
//   for(auto	I:swarm) {
//     if(I.fitness==child.fitness)
//       return 0;
//   }
//   return 1;
// }

// bool MAENS::not_in() {
//   for(auto	I:swarm) {
//     if(I.fitness==child.fitness)
//       return 0;
//     // if(Jaccard_distance(I,child)<0.1){
//     //   return 0;
//     // }
//     // if(Jaccard_distance(I,child)==0
//     //     &&(I.fitness==child.fitness)
//     //     ){
//     //   return 0;
//     // }
//   }
//   return 1;
// }

// bool MAENS::is_record(int iterator) {
//   if(iterator%30==0) 
//     return 1;
//   return 0;
// }

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
	CARP_INDIVIDUAL S(problem);
	for (int i=0; i<popsize; i++) {
		for (ntrial=0; ntrial<ubntrial; ntrial++) {
			// child.rand_init();
			child.fitness = INF;
			// for (int i=0; i<50; i++) {
				S.PS_rand_init();
				// S.PS_init();
				S.get_fitness();
				if(S<child){
					child = S;
				}
			// }
			cout<<child.fitness<<endl;
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

// void MAENS::init_swarm_rand() {
//   for (int i=0; i<popsize; i++) {
//     swarm[i].problem = problem;
//     swarm[i].fitness = INF;
//   }	
//   child.problem = problem;
//   ubntrial = 50;
//   int ntrial;
//   for (int i=0; i<popsize; i++) {
//     for (ntrial=0; ntrial<ubntrial; ntrial++) {
//       // child.rand_init();
//       child.rand_init();
//       get_fitness(child);
//       if(child_not_in()){
//         swarm[i] = child;
//         break;
//       }
//     }
//     if(ntrial>=ubntrial){
//       set_popsize(i);
//       break;
//     }
//   }
//   sort(swarm.begin(),swarm.end());
//   best_individual.problem= problem;
//   best_individual.fitness = INF;
//   best_individual.costs   = INF;
//   update_best();
// }

// void MAENS::init_swarm() {
//   init_swarm_rand();
//   HEURISTIC H(problem);
//   H.path_scanning();
//   CARP_INDIVIDUAL C(problem);
//   C.set_route(H.result);
//   C.get_all_value();
//   swarm[0] = C;
//   get_fitness();
//   sort(swarm.begin(),swarm.end());
//   check();
//   best_individual.problem = problem;
//   best_individual.fitness = INF;
//   best_individual.costs   = INF;
//   update_best();
// }

// void MAENS::init_swarm_with_LS() {
//   init_swarm_rand();
//   // HEURISTIC H(problem);
//   // H.path_scanning();
//   // CARP_INDIVIDUAL C(problem);
//   // C.set_route(H.result);
//   // C.get_all_value();
//   // swarm[0] = C;
//   for (int i=0; i<popsize; i++) {
//     int it=1;
//     while(++it<10){
//       swarm[i].merge_split();
//       swarm[i].while_relocate();
//       swarm[i].while_swap();
//       swarm[i].while_between_2opt();
//       swarm[i].inner_2opt();
//     }
//     // (swarm[i]._21());
//     // while(swarm[i]._123()){}
//     // swarm[i].local_search();
//     get_fitness();
//     cout<<swarm[i].fitness<<endl;
//     // swarm[i].get_best_in_sort();
//   }
//   get_fitness();
//   sort(swarm.begin(),swarm.end());
//   check();
//   best_individual.problem = problem;
//   best_individual.fitness = INF;
//   best_individual.costs   = INF;
//   update_best();
// }

// void MAENS::main_loop() {
//   // order_crossover_subroute_based();
//   // order_crossover_subroute_PS();
//   // order_crossover_subroute_PSR();
//   // order_crossover_subroute_PSE();
//   // order_crossover();
//   for (int i=0; i<popsize; i++) {
//     SBX();
//   // order_crossover();
//     // order_crossover_subroute_PSR();

//     child.get_fitness();
//     Sx = child;
//     Sl = child;
//     if(rand()%100<20){
//       // Sl.local_search();
//       Sl.MAENS_LS();
//       Sl.get_MAENS_fitness();
//       if(!add_to_swarm(Sl)) {
//         add_to_swarm(Sx);
//       }
//     }
//   }
//   sort(swarm.begin(),swarm.end());
//   swarm.resize(popsize);
//   if(update_best()) {
//     // cout<<"min_distance "<< min_swarm_distance()<<endl;
//     // cout<<"max_distance "<< max_swarm_distance()<<endl;
//     // exit(0);
//   }
// }

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

// bool MAENS::add_to_swarm(CARP_INDIVIDUAL& I) {
//   if(not_in()) {
//     swarm.push_back(I);
//     return 1;
//   }
//   return 0;
// }

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
	
// void MAENS::init_lamda(CARP_INDIVIDUAL& I) {
//   DType Q = I.problem->Capacity;
//   I.lamda = I.costs/Q*(best_individual.costs/I.costs+I.over_load/Q+1);
// }

// void MAENS::get_fitness(CARP_INDIVIDUAL& I) {
//   init_lamda(I);
//   I.get_all_value();
//   I.fitness = I.costs + I.lamda*I.over_load;
// }

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

void MAENS::run(){
	// HEURISTIC H(problem);
	// H.path_scanning();
	// candidate_individual.problem = problem;
	// candidate_individual.set_route(H.result);
	// candidate_individual.get_fitness();
	// cout<<"candidate_individual : "<<candidate_individual.fitness<<endl;
	clock_t start_run,end_run;
	run_time;
	start_run = clock();
	iteration = 0;
	// init_swarm();
	// init_swarm_with_LS();
	// init_swarm_rand();
	init_swarm_PSrand();
	int unupdate = 1;
	int upt = 0;
	// init_swarm_rand();
	while(iteration<max_iteration) {
		main_loop(); 
		// cout<<"*"<<iteration<<endl;
		end_run  = clock();
		run_time = (double)(end_run - start_run) / CLOCKS_PER_SEC;
		// update_best();
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

#endif
