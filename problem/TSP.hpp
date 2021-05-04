#ifndef TSP_H_
#define  TSP_H_
#include "../bin/general.hpp"
#include "../problem/problem.hpp"
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <math.h>
// #define CODETYPE 
typedef struct NODE {
	int tag;
	int x;
	int y;
}

NODE;
using namespace std;
class TSP : public Problem<std::vector<int>,double> {
	public:
		/***************** basic variable ************/
		int vertices_number;		// vertices' number
		double** G;       			// Adjacency Matrix
		vector<int> best_sol; 			// E edge list
		vector<NODE> node_list; 			// E edge list
		string instance_filename;// data filename
		string best_filename;
		/***************** basic function ************/
		bool init_G();
		bool read_tsp(); 
		int LB;
		void set_best_file_name();
		bool read_best_file();
		bool read_instance();
		VType get_distance(int ni,int nj);
		void instance_to_adjacency_matrix();
		void show_adjacency_matrix();
		// bool set_matrix();
		E & operator[](const int& ite);
		// VType* & operator[](const int& tail);
		VType & operator()(const int &e,const int & next_e) ;
		TSP();
		TSP(int dimension);
		TSP(string& str);
		~TSP();
};

TSP::TSP(int dimension): Problem(dimension){}

TSP::TSP(string& str){
	instance_filename = str;
	read_instance();
}

TSP::TSP(){}
TSP::~TSP(){}

void TSP::instance_to_adjacency_matrix(){
	for (int i=1; i<=vertices_number; i++) {
		for (int j=1; j<=vertices_number; j++) {
			G[i][j] = get_distance(i,j);
		} 
  }  
}

int Euclidean_distance(NODE& n1,NODE& n2) {
	return sqrt(pow(n1.x-n2.x,2)+pow(n1.y-n2.y,2)); 
}

VType TSP::get_distance(int ni,int nj){
	return Euclidean_distance(node_list[ni],node_list[nj]);
}

VType& TSP::operator()
	(const int &node,const int & next_node) {
	return G[node][next_node];	
}

bool TSP::init_G(){
	int total_size = vertices_number + 1;
	G = construct_matrix<VType>(total_size,total_size,INF);
	return 1;
}

void TSP::show_adjacency_matrix(){
	for (int i=0; i<=vertices_number; i++) {
		for (int j=0; j<=vertices_number; j++) {
			cout<<G[i][j]<<" ";
		}
		cout<<endl;
	}
}

bool TSP::read_instance(){
	read_tsp();
	init_G();
	instance_to_adjacency_matrix();
	read_best_file();
	return 1;
}

bool TSP::read_tsp() {
	ifstream _file(instance_filename,ios::in); 
	if(!_file) { 
		cout << "error opening tsp file." << endl;
		return 0;
	}
	string x;
	stringstream ss;
	ss << _file.rdbuf();
	_file.close();
	while(ss>> x){
		if (x=="DIMENSION" ) {
			ss>>x;
			ss>>vertices_number;
		}
		if (x=="NODE_COORD_SECTION" ) {
			node_list.clear();
			NODE N;
			node_list.push_back(N);
			while (ss>>x&&x!="EOF") {
				ss>>N.x>>N.y;
				node_list.push_back(N);
			}
			break;
		}
	} 
	return 1;
}

void TSP::set_best_file_name(){
  best_filename = instance_filename;
	best_filename[best_filename.size()-3] = 'm';
	best_filename.resize(best_filename.size()-3);
	best_filename += "opt.tour";
}

bool TSP::read_best_file(){
	set_best_file_name();
	ifstream destFile(best_filename,ios::in); //以文本模式打开filename
	if(!destFile) {
		cout << "error opening tsp_opt file." << endl;
		return 0;
	}
	string x;
	VType cost;
	stringstream file_buf;
	file_buf << destFile.rdbuf();
	destFile.close();
	int pos;
	while(file_buf>>x) {
		pos = x.find(".tsp");
		if (pos!=x.npos) {
			char c;
			file_buf>>c;
			file_buf>>LB;
			file_buf>>c;
		}
		if (x=="TOUR_SECTION") {
			best_sol.clear(); 	
			int k;
			while(file_buf>>k&&k!=-1) {
				best_sol.push_back(k);
			}
			best_sol.push_back(k);
			break;
		}
	}
	return 1;
}

#endif
