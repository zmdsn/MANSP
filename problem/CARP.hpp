#ifndef CARP_H_
#define  CARP_H_
#include "../bin/general.hpp"
#include "../problem/problem.hpp"
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <algorithm>
// #define CODETYPE 

using namespace std;
class CARP : public Problem<std::vector<int>,double> {
	public:
		struct doCompare {
			doCompare( CARP& info ) : m_info(info) { } 
			// only if you really need the object state
			CARP& m_info;
			bool operator()( const int & a, const int & b  ) {
				return m_info(m_info.aim_task,a)<
					m_info(m_info.aim_task,b);
			}
		};

		/***************** basic variable ************/
		int vertices_number;		// vertices' number
		int edges_number;				// edges' number
		double** G;       			// Adjacency Matrix
		vector<E> E_list; 			// E edge list
		int aim_task;
		vector<vector<int> > sorted_tasks;
		vector<vector<int> > adjaceney;
		void set_sorted_tasks();
		int task_number;				// number of tasks
		int no_task_number;				// number of tasks
		int Capacity;						// Capacity
		VType total_service_cost;       // total service cost
		DType total_demand;     // total demand to be loaded
		int depot; 							// depot
		int depot_beq_1;         // depot is big than 0
		int Edepot;							// virtual E depot
		string instance_filename;// data filename
		string matrix_filename;
		string save_filename;
		double average_demand;
		inline double get_average_demand() 
			{average_demand = total_demand/task_number; return average_demand;};
		// matrix filename, to speed up calculate adjaceney matrix
		//
		// read instance filename from file => E_list,D
		//
		/***************** basic function ************/
		bool read_file();
		// init G by vertices num
		bool init_G();
		bool init_E_list();
		void set_task_0();
		// read gdb,egl instances set
		bool read_instance();
		bool read_egls();
		bool read_Beijing();
		bool is_overload(DType loaded);
		DType get_over_load(DType loaded);
		DType get_demand(int tag);
		inline VType get_distance_nodes(int ni,int nj);
		inline VType get_distance_node_e(int ni,int e);
		inline DType get_invert(int tag);
		VType get_cost(int tag);
		VType get_costs(vector<int>& route);
		VType get_costs();
		inline int get_head(int e){return E_list[e].head;};
		inline int get_tail(int e){return E_list[e].tail;};
		// construct a matrix file by datafile
		// read matrix file
		void set_matrix_file_name();
		void set_save_file_name();
		// set E_list[i] = e
		void set_E_list(int i,E& e);
		void show_E_list();
		void get_all_service_cost();
		void get_all_demand();
		void show_adjacency_matrix();
		bool read_matrix_file();
		bool set_matrix();
		void egl_set_value(string& s,stringstream& _file);
		void egl_set_E_list(stringstream& _file);
		void get_noreq_edge(stringstream& _file);
		void Beijing_set_value(string& s,stringstream& _file);
		void Beijing_set_E_list(stringstream& _file);
		void instance_to_adjacency_matrix();
		int LB;
		bool get_LB();
		E & operator[](const int& ite);
		// VType* & operator[](const int& tail);
		VType & operator()(const int &e,const int & next_e) ;
		void read_all_files(string& str);
		int get_node_degree_direct(int node);
		int get_node_degree_undirect(int node);
		bool is_Euler() ;
		/***************** analysis function ************/
		double smooth_degree(vector<int> V);
		VType smooth_link_costs(vector<int> routes);
		bool is_smooth_link(int x,int y);
		VType get_smooth_link_costs(int x,int y);
		CARP();
		CARP(int dimension);
		CARP(string& str);
		~CARP();
};

CARP::CARP(int dimension): Problem(dimension){

}

CARP::CARP(string& str){
	read_all_files(str);
}

void CARP::read_all_files(string& str){
	instance_filename = str;
	depot_beq_1 = 1;
	set_matrix_file_name();
	//read_egls();
	read_instance();
	read_matrix_file();
	get_LB();
}

void CARP::instance_to_adjacency_matrix(){
	vector<int> candidate;
	candidate.clear();
	for (int i=0; i<=vertices_number; i++) {
		candidate.push_back(0);
	}
	for (int i=0; i<=vertices_number; i++) {
		adjaceney.push_back(candidate);
	}
	for (auto e:E_list) {
		if (e.head==e.tail) continue;
		// adjaceney[e.head][e.tail] = 1;
		adjaceney[e.tail][e.head] = 1;
	}
}

void CARP::get_all_demand(){
	total_demand = 0;
	for (int i=0; i<E_list.size(); i++) {
		total_demand += E_list[i].demand;
	}
	total_demand /= 2;
}

inline bool CARP::is_overload(DType loaded){
	return loaded > Capacity;
}

inline DType CARP::get_over_load(DType loaded){
	if (is_overload(loaded)) {
		return loaded - Capacity;
	}
	return 0;
}

DType CARP::get_demand(int tag){
	return E_list[tag].demand;
}

inline VType CARP::get_distance_nodes(int ni,int nj){
	return G[ni][nj];
}

inline VType CARP::get_distance_node_e(int ni,int e){
	return G[ni][get_tail(e)];
}

VType CARP::get_cost(int tag){
	return E_list[tag].cost;
}

inline DType CARP::get_invert(int tag){
	return E_list[tag].invert;
}

void CARP::get_all_service_cost(){
	total_service_cost = 0;
	for (int i=0; i<E_list.size(); i++) {
		total_service_cost+= E_list[i].cost;
	}
	total_service_cost /= 2;
}

inline E& CARP::operator[](const int& ite){
	return E_list[ite];
}

inline VType& CARP::operator()(const int &e,const int & next_e)    {
	// the distance from e() to next_e()
	return G[E_list[e].head][E_list[next_e].tail];	
//	return G[tail][head];
}
CARP::CARP(){}
CARP::~CARP(){}

void CARP::set_save_file_name(){
  save_filename = instance_filename;
	string str = "result";
	int pos = save_filename.find("instances");
	cout<<pos<<endl;
	save_filename = save_filename.replace(pos,9,str); 
	//用str替换从指定位置0开始长度为5的字符串
	save_filename[save_filename.size()-3] = 'd';
	save_filename[save_filename.size()-2] = 'a';
	save_filename[save_filename.size()-1] = 't';

}

void CARP::set_matrix_file_name(){
  matrix_filename = instance_filename;
	matrix_filename[matrix_filename.size()-3] = 'm';
}

bool CARP::init_G(){
	// +depot for the instances which depot=1, or start from 1
	int total_size = vertices_number + 1;
	G = construct_matrix<VType>(total_size,total_size,INF);
	return 1;
}

bool CARP::set_matrix(){
	// +depot for the instances which depot=1, or start from 1
	// depot maybe read later set as 1 to void error
	int total_size = this->vertices_number + 1;
	for (int i=0; i<total_size; i++) {
		for (int j=0; j<total_size; j++) {
			G[i][j] = INF;
		} 
  }  
	return 1;
}

bool CARP::init_E_list(){
	E_list.resize(2*task_number+1);
	return 1;
}

bool CARP::read_matrix_file(){
	ifstream destFile(matrix_filename,ios::in); //以文本模式打开filename
	if(!destFile) {
		cout << "error opening destination file." << endl;
		return 0;
	}
	string x;
	VType cost;
	stringstream file_buf;
	file_buf << destFile.rdbuf();
	destFile.close();
	//cout<<vertices_number+depot<<" total:";
	for(int i=0;i<vertices_number+depot_beq_1;i++){
		for(int j=0;j<vertices_number+depot_beq_1;j++){
			//destFile >> G[i][j];
			file_buf >> x;
			if (x=="inf") {
				G[i][j] = INF;
			}else {
				/* ss.clear();
				 * ss << x;
				 * ss >>	cost;
				 * G[i][j] = cost;  */
				// atof is fast than sstream
				G[i][j] = atof(x.c_str());
			}
		}
	}
	return 1;
}

void CARP::set_E_list(int i,E& e){
	E_list[i] = e;
}

void CARP::show_E_list(){
	for(auto e:E_list) {
		cout<<e.tail<<" to "<<e.head<<" cost "<<e.cost<<" demand "<<e.demand<<" invert "<<e.invert<<endl;
		//sleep(1);
	}
}

void CARP::show_adjacency_matrix(){
	for (int i=0; i<vertices_number+depot_beq_1; i++) {
		for (int j=0; j<vertices_number+depot_beq_1; j++) {
			// cout<<G[i][j]<<" ";
			cout<<adjaceney[i][j]<<" ";
		}
		cout<<endl;
	}
}

void CARP::egl_set_E_list(stringstream& _file){
	int i=0;
	int u,v;
	VType c,d;
	string s;
	int esi = 1;
	E e;
	while( i<task_number){
		_file >> s;
		_file >> u;
		_file >> s;
		_file >> v;
		_file >> s;
		_file >> s;
		_file >> c;
		_file >> s;
		_file >> d;
		// for (1,2,5,9)
		if(u>vertices_number)vertices_number=u;
		if(v>vertices_number)vertices_number=v;
		G[u][v] = c;
#ifdef CODETYPE
			e = {u,v,c,d,esi+task_number};
			set_E_list(esi,e);
			e = {v,u,c,d,esi};
			set_E_list(esi+task_number,e);
			esi++;
#else
			e = {u,v,c,d,esi+1};
			set_E_list(esi++,e);
			e = {v,u,c,d,esi-1};
			set_E_list(esi++,e);
#endif
		i++;
	}
}

void CARP::get_noreq_edge(stringstream& _file){
	int i=0;
	int u,v;
	VType c,d;
	string s;
	while( i<no_task_number){
		_file >> s;
		_file >> u;
		_file >> s;
		_file >> v;
		_file >> s;
		_file >> s;
		_file >> c;
		if(u>vertices_number)vertices_number=u;
		if(v>vertices_number)vertices_number=v;
		// cout<<u<<","<<v<<","<<c<<endl;
		G[u][v] = c;
		i++;
	}
}

void CARP::set_task_0(){
	E_list[0].tail = depot;
	E_list[0].head = depot;
	E_list[0].cost = 0;
	E_list[0].demand = 0;
}

void CARP::egl_set_value(string& s,stringstream& _file){
	if(s == "CAPACIDAD"){
		_file >> s;
		_file >> Capacity;
		return;
	}
	if(s == "VERTICES"){
		_file >> s;
		_file >> vertices_number;
		init_G();
		return;
	}
	if(s == "ARISTAS_REQ"){
		_file >> s;
		_file >> task_number;
		init_E_list();
		return;
	}
	if(s == "ARISTAS_NOREQ"){
		_file >> s;
		_file >> no_task_number;
		return;
	}
	// BEIJING && HEFEI
	if(s == "CAPACITY:"){
		_file >> Capacity;
		return;
	}
	if(s == "REQUIRED"){
		_file >> s;
		_file >> task_number;
		return;
	}
	if(s == "LISTA_ARISTAS_REQ"){
		_file >> s;
		egl_set_E_list(_file);
		return;
	}
	if(s == "LISTA_ARISTAS_NOREQ"){
		_file >> s;
		get_noreq_edge(_file);
		return;
	}
	if(s == "DEPOSITO"){
		_file >> s;
		_file >> depot;
		set_task_0();
		if (depot>0) {
			//depot_not_0 = 1;
			depot_beq_1 = 1;
		}
		return;
	}
}

bool CARP::read_egls(){
	// for gdb,egl
	//可以像用cin那样用ifstream对象
	ifstream _file(instance_filename,ios::in); 
	if(!_file) { 
		cout << "error opening source file." << endl;
		return 0;
	}
	stringstream _file_buff;
	_file_buff << _file.rdbuf();
	_file.close();
	string x;
	int esi = 1;
	// time_t start,stop;
	// start = time(NULL);
	while(_file_buff>>x){
	//while(_file >> x){
		// cout<<x<<" ";
		// start = clock();
		egl_set_value(x,_file_buff);
		// stop = clock();
		// printf("Use Time:%f\n",(double)(stop-start)/CLOCKS_PER_SEC);
		//egl_set_value(x,_file);
	} 
	// stop = time(NULL);
	// printf("Use Time:%f\n",(double)(stop-start)/CLOCKS_PER_SEC);
	return 1;
}

bool CARP::read_instance(){
	if (instance_filename.find("Beijing") < instance_filename.length() 
			|| instance_filename.find("Hefei") < instance_filename.length() ){
		time_t start,stop;
		start = clock();
		read_Beijing();
		stop = clock();
		// printf(" Use Time:%f\n",(double)(stop-start)/CLOCKS_PER_SEC);
	} else {
		read_egls();
	}
	return 1;
}

void CARP::Beijing_set_E_list(stringstream& _file){
	string x;
	_file >> x;
	_file >> x;
	int i = 0;
	int esi=1;
	int u,v;
	VType cost,demand;
	E e;
	while( i< task_number+no_task_number){
		_file >> u;
		_file >> v;
		_file >> cost;
		_file >> demand;
		if(u>vertices_number)vertices_number=u;
		if(v>vertices_number)vertices_number=v;
		if(demand>0){ 
#ifdef CODETYPE
			e = {u,v,cost,demand,esi+task_number};
			set_E_list(esi,e);
			G[u][v] = cost;
			e = {v,u,cost,demand,esi};
			// cout<<esi<<" ";
			set_E_list(esi+task_number,e);
			esi++;
#else
			e = {u,v,cost,demand,esi+1};
			set_E_list(esi++,e);
			G[u][v] = cost;
			e = {v,u,cost,demand,esi-1};
			set_E_list(esi++,e);
#endif
		}
		i++;
	}
}

void CARP::Beijing_set_value(string& s,stringstream& _file){
	if(s == "CAPACITY:"){
		_file >> Capacity;
		return;
	}
	if(s == "VERTICES:"){
		_file >> vertices_number;
		init_G();
		return;
	}
	if(s == "REQUIRED"){
		_file >> s;
		_file >> task_number;
		init_E_list();
		set_task_0();
		return;
	}
	if(s == "TOTAL"){
		_file >> s;
		_file >> s;
		_file >> s;
		_file >> s;
		_file >> s;
		return;
	}
	if(s == "NON-REQUIRED"){
		_file >> s;
		_file >> no_task_number;
		return;
	}
	if(s == "NODES"){
		Beijing_set_E_list(_file);
		return;
	}
	if(s == "DEPOT:"){
		_file >> depot;
		if (depot>0) {
			//depot_not_0 = 1;
			depot_beq_1 = 1;
		}else{
			depot_beq_1 = 0;
		}
		return;
	}
}

bool CARP::read_Beijing() {
	ifstream _file(instance_filename,ios::in); 
	if(!_file) { 
		cout << "error opening source file." << endl;
		return 0;
	}
	string x;
	stringstream ss;
	ss << _file.rdbuf();
	_file.close();
	time_t start,stop;
	while(ss>> x){
		// start = clock();
		Beijing_set_value(x,ss);
		// stop = clock();
		// cout<<x;
		// printf(" Use Time:%f\n",(double)(stop-start)/CLOCKS_PER_SEC);
	} 
	return 1;
}

bool CARP::get_LB() {
	// cout<<"getline"<<endl;
	ifstream _file("../data/low_bound.csv",ios::in); 
	if(!_file) { 
		cout << "error opening source file." << endl;
		return 0;
	}
	string x;
	stringstream ss;
	ss << _file.rdbuf();
	_file.close();
	time_t start,stop;
	string line;

	stringstream instance_name_str(instance_filename);
	// ../instances/gdb/gdb1.dat
	string name;
	string instance_name;
	while(getline(instance_name_str,name,'/')) {
		// cout<<name<<endl;
	}
	stringstream ins_name_str(name);
	getline(ins_name_str,instance_name,'.');
	// cout<<instance_name<<endl;

	// cout<<instance_filename<<endl;
	while(getline(ss,line)){
		// cout<<line<<endl;
		istringstream readstr(line); //string数据流化
		getline(readstr,name,','); //循环读取数据
		// start = clock();
		// Beijing_set_value(x,ss);
		// stop = clock();
		// cout<<name<<endl;
		if(name==instance_name){
			getline(readstr,instance_name,',');
			stringstream stream(instance_name );  
			stream>>LB; 
			// cout<<LB;
					// exit(0);
			return 1; 
		}
		// printf(" Use Time:%f\n",(double)(stop-start)/CLOCKS_PER_SEC);
	} 
	return 0;
}

void CARP::set_sorted_tasks() {
	vector<int> candidate;
	for (int i=1; i<=E_list.size(); i++) {
		candidate.push_back(i);
	}
	for (int i=0; i<=E_list.size(); i++) {
		aim_task = i;
		sort(candidate.begin(),candidate.end(),doCompare(*this));
		// show(candidate);
		sorted_tasks.push_back(candidate);
	}
}

int CARP::get_node_degree_direct(int node) {
	return 0;
}

int CARP::get_node_degree_undirect(int node) {
	int count = 0;
	for (int i=0; i<=vertices_number; i++) {
		// if (adjaceney[node][i]>0&&adjaceney[node][i]<INF) 
		if (adjaceney[node][i]==1) 
			count++;
	}
	// cout<<node<<" "<<count<<endl;
	return count;
}

bool CARP::is_Euler() {
	instance_to_adjacency_matrix();
	int even = 0;
	for (int i=0; i<vertices_number; i++) {
		even += get_node_degree_undirect(i)%2;
	}
	return even==0;
}

VType CARP::get_costs(vector<int> &route){
	VType costs = 0;
	int i;
	for (i=0; i<route.size()-1; i++) {
		costs += (*this)(route[i],route[i+1]);
		costs += get_cost(route[i]);
	}
	costs += get_cost(route[i]);
	return costs;
}

VType CARP::get_smooth_link_costs(int task1,int task2) {
	int u,v,x,y;
	u = get_tail(task1);
	v = get_head(task1);
	x = get_tail(task2);
	y = get_head(task2);
	VType min_distance = min(min(G[u][x],G[u][y]),
			min(G[v][x],G[v][y]));
	return min_distance;
}

bool CARP::is_smooth_link(int task1,int task2) {
	return get_smooth_link_costs(task1,task2) == 
		(*this)(task1,task2);
}

VType CARP::smooth_link_costs(vector<int> routes) {
	VType sc=0;
	for (int i=0; i<routes.size()-1; i++) {
		if ( is_smooth_link(routes[i],routes[i+1])) {
			sc += (*this)(routes[i],routes[i+1]);
		}
	}
	return sc;
}

double CARP::smooth_degree(vector<int> routes) {
	VType total_cost,smooth_travel_cost;
	total_cost = get_costs(routes);
	smooth_travel_cost = smooth_link_costs(routes);
	get_all_service_cost();
	// (tc+tic-tsc) / (tc-tic) = (2*tc-sc-tsc) / (sc)
	VType non_smooth_cost = 2*total_cost-
		total_service_cost-smooth_travel_cost;
	double smooth_degree = non_smooth_cost/total_service_cost;
	return smooth_degree;
}

#endif
