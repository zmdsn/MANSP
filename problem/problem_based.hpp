#ifndef _PROBLEM_BASE_
#define _PROBLEM_BASE_
#include <dirent.h>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>         // 提供mode_t 类型  
#include <sys/stat.h>        // 提供属性操作函数  
//#include <dirent.h>            // 提供目录流操作函数  
#include <stdlib.h>
#include <fcntl.h>             // 提供open()函数  
using namespace std;

template<typename EMAP>
inline DType GetTail(int x,EMAP& ES){
    return ES[x].tail;
}

template<typename EMAP>
inline DType GetHead(int x,EMAP& ES){
    return ES[x].head;
}

template<typename EMAP>
inline DType GetCost(int x,EMAP& ES){
    return ES[x].cost;
}

template<typename EMAP>
inline DType GetDemand(int x,EMAP& ES){
    return ES[x].demand;
}

inline DType GetTail(int x,EMAP& ES){
    return ES[x].tail;
}

inline DType GetHead(int x,EMAP& ES){
    return ES[x].head;
}

inline DType GetCost(int x,EMAP& ES){
    return ES[x].cost;
}

inline DType GetDemand(int x,EMAP& ES){
    return ES[x].demand;
}

inline void RandD(){
    int N = 100;
	int i,j;
	for(i=0;i<DMS;i++){
		for(j=0;j<DMS;j++){
			D[i][j] = RandChoose(N);
		}
	}
}

inline void RandEL(){
    int N = 100;
	int i=0,j;
	ES[i].tail   = depot;
	ES[i].head   = depot;
	ES[i].cost   = 0;
	ES[i].demand = 0;
	for(i=1;i<30;i++){
		ES[i].tail   = Rand(6)+1;
		ES[i].head   = Rand(6)+1;
		ES[i].cost   = Rand(N);
		ES[i].demand = Rand(4)+2;
	}
}

template<typename Matrix>
inline bool save2fileMatrix(Matrix &D, int n,string filename){
    ofstream destFile(filename,ios::out); //以文本模式打开out.txt备写
    if(!destFile) {
        cout << "error opening destination file." << endl;
        return 0;
    }
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            destFile << D[i][j] <<" ";
        }
        destFile<<endl;
    }
    destFile.close();
    return 1;
}

template<typename Matrix>
inline bool readfileMatrix(Matrix &D, int n, string filename){
    ifstream destFile(filename,ios::in); //以文本模式打开out.txt备写
    if(!destFile) {
        cout << "error opening destination file." << endl;
        return 0;
    }
    string x;
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            destFile >> D[i][j];
        }
        //destFile >> x;
    }
    destFile.close();
    return 1;
}

template <typename EMAP, typename DMatrix>
bool READ_EGL2GM(string filename, EMAP& ES, DMatrix& D,int &Q,int& LES){
    ifstream srcFile(filename,ios::in); 
    if(!srcFile) { 
        cout << "error opening source file." << endl;
        return 0;
    }
 	int ARISTAS_REQ;
    int ARISTAS_NOREQ;
    int i,u,v,c,d;
    int DEPOSITO;
    string x;
    int n_vertices = 0;
    int esi = 1;
    while(srcFile >> x){
        if(x == "NOMBRE"){
            // cout<<"read name"<<endl;
        }
        if(x == "CAPACIDAD"){
            srcFile >> x;
            srcFile >> Q;
        }
        if(x == "VERTICES"){
            srcFile >> x;
            srcFile >> n_vertices;
        }
        if(x == "ARISTAS_REQ"){
            srcFile >> x;
            srcFile >> ARISTAS_REQ;
        }
        if(x == "ARISTAS_NOREQ"){
            srcFile >> x;
            srcFile >> ARISTAS_NOREQ;
        }
        // BEIJING && HEFEI
        if(x == "CAPACITY:"){
            srcFile >> Q;
        }
        if(x == "REQUIRED"){
            srcFile >> x;
            srcFile >> ARISTAS_REQ;
        }
        if(x == "LISTA_ARISTAS_REQ"){
            srcFile >> x;
            int i=0;
            while( i<ARISTAS_REQ){
                srcFile >> x;
                srcFile >> u;
                srcFile >> x;
                srcFile >> v;
                srcFile >> x;
                srcFile >> x;
                srcFile >> c;
                srcFile >> x;
                srcFile >> d;
                if(u>n_vertices)n_vertices=u;
                if(v>n_vertices)n_vertices=v;
                
                ES[esi].tail = u;
                ES[esi].head = v;
                ES[esi].cost = c;
                ES[esi++].demand = d;
#ifdef CODETYPE
                ES[esi+ARISTAS_REQ].tail = v;
                ES[esi+ARISTAS_REQ].head = u;
                ES[esi+ARISTAS_REQ].cost = c;
                ES[esi+ARISTAS_REQ].demand = d;
#else
				ES[esi].tail = v;
				ES[esi].head = u;
				ES[esi].cost = c;
				ES[esi++].demand = d;
#endif
                i++;
            }
        }
        if(x == "DEPOSITO"){
            srcFile >> x;
            srcFile >> depot;
            if (depot) {
                depot_not_0 = 1;
            }
        }
    } //可以像用cin那样用ifstream对象
    srcFile.close();
    ES[0].tail = depot;
    ES[0].head = depot;
    ES[0].cost = 0;
    ES[0].demand = 0;
    filename[filename.size()-3] = 'm';
    readfileMatrix(D, n_vertices,filename);
//	LES = esi-1;
	LES = ARISTAS_REQ;
    // cout<<"init G ok"<<endl;
    return 1;
}

template <typename EMAP, typename DMatrix>
bool READ_BJ2GM(string filename, EMAP& ES, DMatrix& D,int &Q,int& LES)
{
    ifstream srcFile(filename,ios::in); 
    if(!srcFile) { 
        cout << "error opening source file." << endl;
        return 0;
    }
		int ARISTAS_REQ;
    int ARISTAS_NOREQ;
    int i,u,v,c,d;
    int DEPOSITO;
    string x;
    int n_vertices = 0;
    int esi = 1;
    while(srcFile >> x){
        if(x == "CAPACITY:"){
            srcFile >> Q;
        }
        if(x == "VERTICES:"){
            srcFile >> n_vertices;
        }
        if(x == "REQUIRED"){
            srcFile >> x;
            srcFile >> ARISTAS_REQ;
        }
        if(x == "TOTAL"){
            srcFile >> x;
            srcFile >> x;
            srcFile >> x;
            srcFile >> x;
            srcFile >> x;
        }
        if(x == "NON-REQUIRED"){
            srcFile >> x;
            srcFile >> ARISTAS_NOREQ;
        }
        if(x == "NODES"){
            srcFile >> x;
            srcFile >> x;
            i = 0;
            // cout << ARISTAS_NOREQ<<endl;
            while( i<ARISTAS_REQ+ARISTAS_NOREQ){
                srcFile >> u;
                srcFile >> v;
                srcFile >> c;
                srcFile >> d;
                if(u>n_vertices)n_vertices=u;
				if(v>n_vertices)n_vertices=v;
				if(d>0){ 
					ES[esi].tail = u;
					ES[esi].head = v;
					ES[esi].cost = c;
					ES[esi++].demand = d;
#ifdef CODETYPE
					ES[esi+ARISTAS_REQ].tail = v;
					ES[esi+ARISTAS_REQ].head = u;
					ES[esi+ARISTAS_REQ].cost = c;
					ES[esi+ARISTAS_REQ].demand = d;
#else
					ES[esi].tail = v;
					ES[esi].head = u;
					ES[esi].cost = c;
					ES[esi++].demand = d;
#endif
				}
				i++;
			}
		}
		if(x == "DEPOT:"){
            srcFile >> depot;
            if (depot) {
                depot_not_0 = 1;
            }
        }
    } //可以像用cin那样用ifstream对象
    srcFile.close();
    ES[0].tail = depot;
    ES[0].head = depot;
    ES[0].cost = 0;
    ES[0].demand = 0;
    filename[filename.size()-3] = 'm';
    readfileMatrix(D, n_vertices,filename);
	LES = ARISTAS_REQ;
  //  cout<<"init G ok"<<endl;
    return 1;
}

void GotFile(string filename){
	 READ_EGL2GM(filename, ES, D,Q,LES);
}

//template<typename EMAP,typename DMatrix>
inline VType min_Distance(int a,int b,EMAP& ES, DMatrix &D){
	int u,v,x,y;
	u = ES[a].head-depot_not_0;
	v = ES[a].tail-depot_not_0;
	x = ES[b].head-depot_not_0;
	y = ES[b].tail-depot_not_0;
	VType mindistance = INF;
	if (D[u][x] < mindistance) mindistance = D[u][x];
	if (D[u][y] < mindistance) mindistance = D[u][y];
	if (D[v][x] < mindistance) mindistance = D[v][x];
	if (D[v][y] < mindistance) mindistance = D[v][y];
	return mindistance;
}

//template<typename EMAP,typename DMatrix>
inline VType Distance(int a,int b,EMAP& ES, DMatrix &D){
    // cout << ES[a].head<<","<<ES[b].tail;
// #ifdef CODETYPE
// 	return D[ES[a].head-depot_not_0 ][ES[b].tail-depot_not_0];
// #else
	return D[ES[a].head-depot_not_0 ][ES[b].tail-depot_not_0];
// #endif
}

template<typename EMAP,typename DMatrix>
inline VType Distance(int a,int b,EMAP& ES, DMatrix &D){
// #ifdef CODETYPE
//     return D[ES[a].head-depot_not_0 ][ES[b].tail-depot_not_0];
// #else
//     return D[ES[a].head-depot_not_0 ][ES[b].tail-depot_not_0];
// #endif
    return D[ES[a].head-depot_not_0 ][ES[b].tail-depot_not_0];
}

//template<typename PATH,typename EMAP,typename DMatrix>
inline bool QC(PATH &P,EMAP &ES,DMatrix &D,DType& Q){
	int len = P[0];
	DType dem = 0;
	PATH S;
	int SP[len];
	Decode(S,P);
	Split(P,SP);
	int nextE,E;
//	Trail(SP);
	for(int j=1;j<SP[0];j++){
//		cout<<SP[j]<<" ";
		dem = 0;
		for(int i=SP[j];i<SP[j+1];i++){
			E = S[i];
			dem += GetDemand(E,ES);
			
		}
	//	cout<<dem<<" ";
		if(dem>Q){
			return 0;
		}
	}
	return 1;
}

//template<typename PATH,typename EMAP,typename DMatrix>
DType VioConsCal(PATH &P,EMAP &ES,DMatrix &D,DType& Q){
	int len = P[0];
	DType vio = 0,loadk=0;
    PATH S;
	Decode(S,P);
	int nextE,E;
	for(int i=1;i<=len;i++){
		E = S[i];  
		if(E==0){
			vio  += loadk>Q?loadk - Q:0;
			loadk = 0;
			continue;	
		}
		loadk += GetDemand(E,ES);
	}
	return vio;
}


//template<typename PATH,typename EMAP,typename DMatrix>
VType ValueCal(PATH &P,EMAP &ES,DMatrix &D){
	int len = P[0];
	VType cost = 0;
    PATH S;
	Decode(S,P);
	int nextE,E;
	cost += GetCost(P[len],ES);
    for(int i=1;i<len;i++){
        // E = S[i];
        // nextE = S[i+1];
        cost += Distance(S[i],S[i+1],ES,D) + GetCost(S[i],ES);
		// E = S[i];
		// nextE = S[i+1];
		// cost += Distance(E,nextE,ES,D) + GetCost(E,ES);
		// std::cout<<E<<" ("<<GetTail(E,ES)<<","<<GetHead(E,ES)<<")"<<" : "<<GetCost(E,ES)<<"\t == "<<Distance(E,nextE,ES,D)<<" =>\t"<<nextE<<" ("<<GetTail(nextE,ES)<<","<<GetHead(nextE,ES)<<") :\t "<<","<<cost<<std::endl;
	}
//	std::cout<<cost <<std::endl;
	//delete [] S;	
	return cost;
}

template<typename PATH,typename EMAP,typename DMatrix>
VType ValueCal(PATH &P,EMAP &ES,DMatrix &D){
    int len = P[0];
    VType cost = 0;
    PATH S;
    Decode(S,P);
    int nextE,E;
    cost += GetCost(P[len],ES);
    // for(int i=len-1;i>=1;i--){
    for(int i=1;i<len;i++){
        // E = S[i];
        // nextE = S[i+1];
        cost += Distance(S[i],S[i+1],ES,D) + GetCost(S[i],ES);
        // E = S[i];
        // nextE = S[i+1];
        // cost += Distance(E,nextE,ES,D) + GetCost(E,ES);
        // std::cout<<E<<" ("<<GetTail(E,ES)<<","<<GetHead(E,ES)<<")"<<" : "<<GetCost(E,ES)<<"\t == "<<Distance(E,nextE,ES,D)<<" =>\t"<<nextE<<" ("<<GetTail(nextE,ES)<<","<<GetHead(nextE,ES)<<") :\t "<<","<<cost<<std::endl;
    }
//  std::cout<<cost <<std::endl;
    //delete [] S;  
    return cost;
}

VType _0ECal(PATH &P,int& len,EMAP &ES,DMatrix &D){
    // calculate all cost of (e,0) and (0,e) 
    int splitPos[ P[0] ];
    Split(P,splitPos);
    len = splitPos[0]-1;
    VType cost = 0;
    PATH S;
    Decode(S,P);
    for(int i=1;i<splitPos[0];i++){
        cost += Distance(S[splitPos[i]],S[splitPos[i]+1],ES,D);
    }  
    for(int i=splitPos[0];i>1;i--){
        cost += Distance(S[splitPos[i]-1],S[splitPos[i]],ES,D);
    } 
    return cost;
}

void SubLCal(PATH &P,int* len,EMAP &ES,DMatrix &D){
    // calculate all cost of (e,0) and (0,e) 
    int splitPos[ P[0] ];
    Split(P,splitPos);
    len[0] = 0;
    PATH S;
    for(int i=1;i<splitPos[0];i++){
        getSubk(S,P,i);
        len[++len[0]] = ValueCal(S,ES,D);
    }  
}

//template<typename PATH,typename EMAP,typename DMatrix>
VType MaxLSubP(PATH &P,EMAP &ES,DMatrix& D){
	int len = P[0];
	VType cost = 0;
//	int* S = new int[len];
    PATH S;
	Decode(S,P);
	int MaxL=0;
	int SubLen = 0;
	int E;
	for(int i=1;i<=len;i++){
		E = S[i];
//		std::cout<<SubLen<<std::endl;
		if(E==0){
			if(SubLen>MaxL){
				MaxL = SubLen;
			}
			SubLen = 0;
		}else{
			SubLen++;
		}
	}
//	delete [] S;	
	return (VType)MaxL;	
}

//template<typename PATH,typename EMAP,typename DMatrix>
VType MObj(PATH &P,EMAP &ES,DMatrix& D,int k){
	switch(k){
		case 1: return ValueCal(P,ES,D);
		case 2: return MaxLSubP(P,ES,D);
		case 3: return ValueCal(P,ES,D);
		case 4: return ValueCal(P,ES,D);
		case 5: return ValueCal(P,ES,D);
		case 6: return ValueCal(P,ES,D);
	}
	return -1;
}



//template<typename PATH,typename EMAP,typename DMatrix>
inline VType ChangeCost(PATH &P,EMAP &ES,DMatrix& D,int e,int k){
	if(k==1){
		return Distance(Edepot,e,ES,D)+Distance(e,P[k+1],ES,D)-Distance(Edepot,P[k],ES,D) - Distance(P[k],P[k+1],ES,D);
	}else if(k==P[0]){
		return Distance(P[k-1],e,ES,D)+Distance(e,Edepot,ES,D)-Distance(P[k-1],P[k],ES,D)-Distance(P[k],Edepot,ES,D);
	}
	return Distance(P[k-1],e,ES,D)+Distance(e,P[k+1],ES,D)-Distance(P[k-1],P[k],ES,D)-Distance(P[k],P[k+1],ES,D);	
}

//template<typename PATH,typename EMAP,typename DMatrix>
inline VType OPT2Cost(PATH &P,EMAP &ES,DMatrix& D,int ki,int kj){
	VType cost;
	int s;
	int x,y,u,v;
	// ki->y, kj -> u
	if (ki<kj) {
		// case 1 [(x,y),...,(u,v)] => [(x,u'),...,(y',v)]
		x = ki-1;
		y = ki;
		u = kj;
		v = kj+1;
		cost  = Distance(P[x],ReverseElement(P[u]),ES,D);
		cost += Distance(ReverseElement(P[y]),P[v],ES,D);
		return cost - Distance(P[x],P[y],ES,D) - Distance(P[u],P[v],ES,D);
	} else {
		// case 2 [(x,y),...,(u,v)] => [(x,v),...,(u,y)]
		x = kj-1;
		y = kj;
		u = ki;
		v = ki+1;
		//cout<<P[x]<<P[u];
		cost  = Distance(P[x],P[v],ES,D);
		cost += Distance(P[u],P[y],ES,D);
		return cost - Distance(P[x],P[y],ES,D) - Distance(P[u],P[v],ES,D);
	}
}

//template<typename PATH,typename EMAP,typename DMatrix>
inline VType InsertCost(PATH &P,EMAP &ES,DMatrix& D,int e,int k){
	if(k==1){
		return Distance(Edepot,e,ES,D)+Distance(e,P[k],ES,D)-Distance(Edepot,P[k],ES,D);
	}else if(k==P[0]+1){
		return Distance(P[P[0] ],e,ES,D)+Distance(e,Edepot,ES,D)-Distance(P[P[0] ],Edepot,ES,D);
	}
	return Distance(P[k-1],e,ES,D)+Distance(e,P[k],ES,D)-Distance(P[k-1],P[k],ES,D);
}

//template<typename PATH,typename EMAP,typename DMatrix>
inline VType DeleteCost(PATH &P,EMAP &ES,DMatrix& D,int k){
	if(k==1){
		return Distance(Edepot,P[k+1],ES,D)-Distance(Edepot,P[k],ES,D)-Distance(P[k],P[k+1],ES,D);
	}else if(k==P[0]){
		return Distance(P[k-1],Edepot,ES,D) - Distance(P[k-1],P[k],ES,D) - Distance(P[k],Edepot,ES,D);
	}
	return Distance(P[k-1],P[k+1],ES,D) - Distance(P[k-1],P[k],ES,D) - Distance(P[k],P[k+1],ES,D);	
}

//template<typename PATH,typename EMAP,typename DMatrix>
inline VType InsertSegmentCost(PATH &P,EMAP &ES,DMatrix& D,int* ei,int k){
    int ksize = ei[0];
    int e1,ed;
    e1 = ei[1];
    ed = ei[ei[0]];
    VType cost=0;
    for(int i=1;i<ei[0];i++){
        cost += GetCost(ei[i],ES)+Distance(ei[i],ei[i+1],ES,D);
    }
    cost += GetCost(ei[ei[0] ] ,ES);
    if(k==1){
        return cost + Distance(Edepot,e1,ES,D)+Distance(ed,P[k],ES,D) - Distance(Edepot,P[k],ES,D);
    }else if(k==P[0]+1){
        return cost + Distance(Edepot,e1,ES,D)+Distance(ed,Edepot,ES,D);
    }
    return cost + Distance(P[k-1],e1,ES,D)+Distance(ed,P[k],ES,D) - Distance(P[k-1],P[k],ES,D); 
}

//template<typename PATH,typename EMAP,typename DMatrix>
inline VType ChangeSegmentCost(PATH &P,EMAP &ES,DMatrix& D,int* ei,int k){
	int ksize = ei[0];
	int e1,ed;
	int l = ei[0];
	e1 = ei[1];
	ed = ei[ei[0]];
	VType cost=0;
	if(k+l>P[0]) return 0x3f3f3f3f;
	cost = InsertSegmentCost(P,ES,D,ei,k);
	for(int j=k;j<k+l;j++){
		cost -= (Distance(P[j],P[j+1],ES,D)+GetCost(P[j],ES)); 
	}
	cost += -Distance(ed,P[k],ES,D)+ Distance(ed,P[k+l],ES,D);
	return cost ;	
}



//template<typename PATH,typename EMAP,typename DMatrix>
inline VType DeleteSegmentCost(PATH &P,EMAP &ES,DMatrix& D,int k,int ksize){
	int e1,ed;
	e1 = P[k];
	ed = P[k+ksize-1];
	VType cost=0;
	for(int i=k;i<k+ksize;i++){
		cost -= GetCost(P[i],ES)+Distance(P[i],P[i+1],ES,D);
	}
//	cost -= Distance(P[k-1],P[k],ES,D);
	if(k==1){
		cost -= Distance(Edepot,P[k],ES,D);
		return cost + Distance(Edepot,P[k+ksize],ES,D);
	}else if(k==P[0]-ksize+1){
		cost -= Distance(P[k-1],P[k],ES,D);
		return cost + Distance(P[k-1],Edepot,ES,D);
	}
	cost -= Distance(P[k-1],P[k],ES,D);
	return cost+Distance(P[k-1],P[k+ksize],ES,D);
}



vector<string> get_files_under(string FILEPATH){
    struct dirent *ptr;
    DIR *dir;
    // FILEPATH = "./gdb";
    dir=opendir(FILEPATH.c_str());
    vector<string> files;
    cout << "文件列表: "<< endl;
    string feature = ".dat";
    string feature1 = ".txt";
    string fname;
    while((ptr=readdir(dir))!=NULL) {
        //跳过'.'和'..'两个目录
        if(ptr->d_name[0] == '.')
            continue;
        fname = ptr->d_name;
        if(fname.find(feature) != string::npos || fname.find(feature1) != string::npos){
            files.push_back(ptr->d_name);
        }
    }
    for (int i = 0; i<files.size(); ++i) {
        cout << files[i] << endl;
    }
    closedir(dir);
    return files;
}

vector<string> get_folders_under(string FILEPATH){
  // char buff[1000];
  // getcwd(buff, 1000);
  // cout << "当前路径是：" << buff << endl;
  struct dirent *ptr; 
  DIR *dir; 
	int res;
#ifdef WIN32
	struct _stat statbuf;  // 获取下一级成员属性 
#else
	struct stat statbuf; // 获取下一级成员属性 
#endif	
  dir=opendir(FILEPATH.c_str()); 
			//cout<<FILEPATH<<"\t";
  vector<string> folders; 
  cout<< endl << "文件夹列表: "<< endl; 
  //chdir (FILEPATH.c_str()); 
  string fname; 
// cout<<length(dir);
	if(dir) {
		while((ptr=readdir(dir))!=NULL) 
		{ 
			//跳过'.'和'..'两个目录 
			if (ptr->d_name[0] == '.') 
				continue; 
			fname = ptr->d_name;
			fname = FILEPATH+"/"+fname;
#ifdef WIN32
			cout<<"WIN32"<<endl;
			_stat(ptr->d_name, &statbuf); // 获取下一级成员属性 
			if(statbuf.st_mode & S_IFDIR)    // 判断下一级成员是否是目录 S_IFDIR
			{ 
				folders.push_back(ptr->d_name); 
			} 
#else
			//cout<<fname<<"\t";
			res = lstat(fname.c_str(), &statbuf); // 获取下一级成员属性 
			//res = lstat("../instance/gdb/gdb1.dat", &statbuf); // 获取下一级成员属性 
			//res = lstat(ptr->d_name, &statbuf); // 获取下一级成员属性 
			if (res==-1) cout<<ptr->d_name<<" get stat false\n";
			if(statbuf.st_mode & __S_IFDIR)    // 判断下一级成员是否是目录 S_IFDIR
			{ 
				folders.push_back(ptr->d_name); 
			} 
#endif

		} 
		for (int i = 0; i<folders.size(); ++i) 
		{ 
			cout << folders[i] << endl; 
		} 
		//chdir(buff);
		closedir(dir);
		return folders;
	} else {
		cout << "can't open the foldes "<< endl; 
	}
	return folders;
}


template<typename PATH>
inline bool readresult(PATH &P, string filename){
    ifstream srcFile(filename,ios::in); //以文本模式打开out.txt备写
    if(!srcFile) {
        cout << "error opening destination file." << endl;
        return 0;
    }
    string x;
    while(srcFile >> x) { 
			if(x == "best"){
				srcFile >> x;
				if(x == "solution"){
					srcFile >> x;
					srcFile >> P[0];
					for (int i=1;i<=P[0];i++) {
						srcFile >> P[i];
					}
				}
			}
		}
    srcFile.close();
    return 1;
}

template<typename T>
bool read_low_bound(string filename,string obj,T& low_bound){
  low_bound = 0;
  ifstream destFile(filename,ios::in);
  if(!destFile) {
    cout << "error opening destination file." << endl;
    return 0;
  }
  string x;
  int n =10;
  int pos,pos2;
  string strpos;
  while(destFile>>x) {
    //cout<<x<<endl;
    pos = x.find(",", 0);
    //cout<<pos<<endl;
    strpos = x.substr(0,pos);
    if (strpos == obj) {
      pos2 = x.find(",", pos+1);
      // cout<<pos<< pos2-pos<<endl;
      strpos = x.substr(pos+1,pos2-pos-1);
      istringstream is(strpos);
      is>>low_bound;
      //low_bound = stoi(strpos);
      destFile.close();
      return 1;
    }
  }
  destFile.close();
  return 0;
}

template<typename T>
bool check_low_bound(T& result,T& low_bound){
	if (result<=low_bound) {
		return 1;
	}else{
		return 0;
	}
}

template<typename EMAP,typename DMatrix>
void Get_graph(EMAP& ES,DMatrix& D,DMatrix& G,DMatrix& W,int depot){
	int tail,head;
	int n_vertex,v;
	for(int i=0;i<=2*LES;i++) {
		tail = GetTail(i,ES);
		head = GetHead(i,ES);
		D[tail][head] = GetCost(i,ES);
		if (tail>n_vertex) n_vertex = tail;
		if (head>n_vertex) n_vertex = head;
	}	
	for(int i=0;i<=n_vertex;i++) {
		for(int j=0;j<=n_vertex;j++) {
			G[i][j] = INF;
			if (i>0&&j>0) {
				v = D[i-depot][j-depot];
				if (v>0) G[i][j] = v;
			}
			if (i==depot&&j==depot) 
				G[i][j] = 0;
		}
	}
}

template<typename EMAP,typename DMatrix>
void Get_graph(EMAP& ES,DMatrix& D,DMatrix& G,DMatrix& W,int depot,int& n_vertex){
	int tail,head,v;
	n_vertex = 0;
	for(int i=0;i<=2*LES;i++) {
		tail = GetTail(i,ES);
		head = GetHead(i,ES);
		//cout<<tail<<","<<head<<endl;
		if (tail>n_vertex) n_vertex = tail;
		if (head>n_vertex) n_vertex = head;
		W[tail][head] = GetDemand(i,ES);
	}	
	//cout<<n_vertex<<endl;
	for(int i=0;i<=n_vertex;i++) {
		for(int j=0;j<=n_vertex;j++) {
			G[i][j] = INF;
			if (i>0&&j>0) {
				v = D[i-depot][j-depot];
				if (v>0) G[i][j] = v;
			}
			if (i==depot&&j==depot) 
				G[i][j] = 0;
		}
	}
}
/*
template<typename EMAP,typename DMatrix>
int Get_graph(EMAP& ES,DMatrix& D,DMatrix& W){
	int tail,head;
	for(int i=0;i<=LES;i++) {
		for(int j=0;j<=LES;j++) {
			D[i][j] = INF;
		}
	}
	for(int i=0;i<=LES;i++) {
		tail = GetTail(i,ES);
		head = GetHead(i,ES);
		D[tail][head] = GetCost(i,ES);
		if (D[tail][head] ==0) 
			D[tail][head] = INF;
		W[tail][head] = GetDemand(i,ES);
	}	
}

template<typename EMAP,typename DMatrix>
int Get_graph(EMAP& ES,DMatrix& D,DMatrix& W,int& n_vertex){
	int tail,head;
	n_vertex = 0;
	for(int i=0;i<=2*LES;i++) {
		for(int j=0;j<=LES;j++) {
			D[i][j] = INF;
		}
	}
	for(int i=0;i<=2*LES;i++) {
		tail = GetTail(i,ES);
		head = GetHead(i,ES);
		cout<<tail<<"&"<<head<<endl;
		if (tail>n_vertex) n_vertex = tail;
		if (head>n_vertex) n_vertex = head;
		D[tail][head] = GetCost(i,ES);
		if (D[tail][head] ==0) 
			D[tail][head] = INF;
		W[tail][head] = GetDemand(i,ES);
	}	
}
*/

#endif
