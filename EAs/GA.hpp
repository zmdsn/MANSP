#ifndef GA_H_
#define GA_H_
#include "../bin/general.hpp"
#include "../bin/individual.hpp"
#include "../EAs/EA.hpp"
#include <math.h>
#include <vector>
// #include "../local_search/first_feasible_of_MA.hpp"
#define Individual INDIVIDUAL<std::vector<int> >  

using namespace std;

class GA{
	public:
		Individual S;
		Individual best_solution;
		Individual best_individual;
		Individual child;
		vector<Individual> swarm;
		vector<Individual> children;
		int end_type = 2;
		int max_iteration = 1000;
		int max_time = 600;
		int Dim = 30;
		int popsize = 20;
		double pm;
		void main_loop();
		void main_loop_each_one_swarm();
		void run();
		VType get_fitness(Individual& I);
		void rand_init(Individual& I,int k);
		void rand_init_swarm(int dim);
		int select();
		void crossover();
		bool mutation();
		void get_children();
		GA();
		GA(CARP* P);
		GA(const GA& algorithm);
		GA(int popsize);
		GA(int _popsize,int _end_type,int _max_iteration,int _max_time);
		GA& operator=(const GA&);
		~GA();
}
;
GA::GA() {}
GA::GA(CARP* P){
	// problem = P;
	best_solution.fitness = INF;
	// child.problem = P;
	popsize = 10;
	end_type = 2;
	max_iteration = 3000;
	max_time = 600;
	swarm.resize(popsize);
	children.resize(popsize);
	// best_individual.problem = problem;
	best_individual.fitness = INF;
}

GA::GA(int _popsize=3){
	swarm.resize(popsize);
	children.resize(popsize);
}

GA::GA(int _popsize,int _end_type,int _max_iteration,int _max_time) {
	best_individual.fitness = INF;
}

GA::~GA(){}

bool GA::mutation() {
	for (int i=0; i<Dim; i++) {
		if (rand()%100<10) {
			// child.S[i] = (rand()%20000-10000)/100;
			child.S[i] += 0.1*(rand()%100-50);
		}
	}
	return 0;
}

void GA::crossover() {
	int p1,p2;
	p1 = select();
	p2 = select();
	child.S.clear();
	for (int i=0; i<Dim; i++) {
		if (rand()%2) {
			child.S.push_back(swarm[p1].S[i]);
		} else {
			child.S.push_back(swarm[p2].S[i]);
		}
	}
}

int GA::select() {
	return rand()%popsize;
}

void GA::get_children() {
	children.clear();
	for (int i=0; i<popsize; i++) {
		crossover();
		mutation();
		get_fitness(child);
		// show(child.S);
		// cout<<get_fitness(child)<<endl;
		children.push_back(child);
	}
}

void GA::main_loop() {
	for (int ite = 0;ite<max_iteration;ite++) {
		get_children();
		swarm.insert(swarm.end(),children.begin(),children.end());
		sort(swarm.begin(),swarm.end());
		swarm.resize(popsize);
		cout<<ite<<" : "<<swarm[0].fitness<<endl;
	}
}

void GA::run(){
	rand_init_swarm(Dim);
	clock_t start_run,end_run;
  double	run_time;
	start_run = clock();
	main_loop();
	show(swarm[0].S);
	end_run  = clock();
	run_time = (double)(end_run - start_run) / CLOCKS_PER_SEC;
	cout<<endl<<"final result : "<<best_individual.fitness<<endl;
}

void GA::rand_init(Individual& I,int k) {
	I.S.clear();
	for (int i=0; i<k; i++) {
		I.S.push_back(rand()%10000/200-100);
	}
	// show(I.S);
}

void GA::rand_init_swarm(int dim) {
	for (int i=0; i<popsize; i++) {
		rand_init(swarm[i],dim);
		get_fitness(swarm[i]);
	}
}

VType GA::get_fitness(Individual& I) {
	VType res = 0;
	for (int i=0; i<I.S.size(); i++) {
		// res += pow(I.S[i],2);
		// res += pow(I.S[i]-20.1,2);
		res += sin(I.S[i]);
	}
	I.fitness = res;
	return res;
}

#endif
