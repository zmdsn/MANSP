#ifndef __HEUTISTIC__FILE__
#define __HEUTISTIC__FILE__
#include<algorithm>

template<typename IndividualModel,typename EMAP,typename DMatrix>
inline void FitnessCal1(IndividualModel &I,EMAP &ES,DMatrix& D){
//		std::cout<<I.P[0]<<std::endl;
		I.FitVal = ValueCal(I.P,ES,D);
}

/************************************** path_scanning **************************************************/
template<typename EMAP, typename DMatrix>
inline bool betteru(int u,int ubar,int rule, VType loadk, EMAP &ES , DMatrix &D, DType& Q ){
 	// VType u0    = Distance(u,Edepot,ES,D);
 	// VType ubar0 = Distance(ubar,Edepot,ES,D); 
	int is_end_depot = 1;
	bool res;
	VType cu,cubar,u0,ubar0;
	DType ru,rubar;
	u0    = Distance(u,Edepot,ES,D);
	ubar0 = Distance(ubar,Edepot,ES,D); 
	if(u0==0) is_end_depot = 0;
	if(ubar0==0) is_end_depot = 1;
	if(ubar0==0&&u0!=0) return 1;	
	switch(rule){
	//	case 1:	res = 1.0*ru/cu > 1.0*rubar/cubar;break;
	//	case 2: res = 1.0*ru/cu > 1.0*rubar/cubar;break;
		case 1:	
		    cu = GetCost(u,ES)   ,cubar = GetCost(ubar,ES);
		    ru = GetDemand(u,ES) ,rubar = GetDemand(ubar,ES);
			res = (double)cu/ru > (double)cubar/rubar;
			break;
		case 2: 
		    cu = GetCost(u,ES)   ,cubar = GetCost(ubar,ES);
		    ru = GetDemand(u,ES) ,rubar = GetDemand(ubar,ES);
		    res = (double)cu/ru < (double)cubar/rubar;
		    break;
		case 3: 
		 	// u0    = Distance(u,Edepot,ES,D); if(u0==0) return 0; // back to 0 as late as possible
	 		// ubar0 = Distance(ubar,Edepot,ES,D); 
			res = u0 > ubar0;
			break;
		case 4: 
		 	// u0    = Distance(u,Edepot,ES,D); if(u0==0) return 0;
		 	// ubar0 = Distance(ubar,Edepot,ES,D); 
			res = u0 < ubar0;
			break;
		case 5: 
		 	// u0    = Distance(u,Edepot,ES,D); if(u0==0) return 0;
		 	// ubar0 = Distance(ubar,Edepot,ES,D); 
			res = (loadk < Q/2)?(u0 > ubar0) : (u0 < ubar0);
			break;
	}
	return is_end_depot && res;
}

template<typename PATH,typename EMAP,typename DMatrix>
void path_scanning(PATH& P,EMAP &ES,DMatrix& D,int task_num,int& Q,int rule,int* freeL){
// //template<typename PATH1,typename EMAP,typename DMatrix>
// void path_scanning(int * P,E *ES,int D[][MS],int task_num,int& Q,int rule,int* (freeL)) {
	// int k = 0;
	int sel,u,ubar;
	int loadk,costk,i,ei;
	// int EQ[Max_PATH_len];
	DType qu;
	VType dbar;
	P[0] = 0;
	DType distance;
	int idx;
	while(freeL[0]>0){
		// AddElement(P,0, P[0]+1);
		AddElement(P,0);
		//P[++P[0]] = 0;
		// ++k;
		loadk = 0;
		// costk = 0;
		ei = Edepot;
//		i = 1;
		dbar = Edepot;
		while(freeL[0]>0&&dbar!=INF){
			dbar = INF;
			for(idx=1;idx<=freeL[0];++idx){
			//for(int idx=freeL[0];idx>=1;idx--){
				u  = freeL[idx];
				// qu = GetDemand(u,ES);
				if(loadk + GetDemand(u,ES) <=Q){
					//if(D[i][GetTail(u,ES)]<dbar){
				//	if(D[i-1][GetTail(u,ES)-1]<dbar){
					distance = Distance(ei,u,ES,D);
					if(distance<dbar){
						dbar = distance;
						ubar = u;
						// EQ[0]= 1;
						// EQ[1]= u;
					}else{
//						if(dbar==D[i-1][GetTail(u,ES)-1]&&betteru(u,ubar,rule,loadk,ES,D,Q) ){
						if(dbar==distance && betteru(u,ubar,rule,loadk,ES,D,Q) ){
							ubar = u;
							// EQ[0]= 1;
							// EQ[1]= u;
						}
//						else if(dbar==Distance(ei,u,ES,D)){
//							EQ[++EQ[0]] = u;
//						}
					}
				}
			}
			//if(EQ[0]>1) ubar = EQ[ RandChoose(EQ[0]) +1 ]; 
	//		if(EQ[0]>1) ubar = EQ[1]; 
			
			if(dbar != INF){
				DeleteElementAndRev(freeL,ubar);
				// sel = FindElement(freeL,ReverseElement(ubar) );
				// if(sel) DeleteElement(freeL,sel);
				// sel = FindElement(freeL,ubar);
				// if(sel) DeleteElement(freeL,sel);
				// AddElement(P,ubar,P[0]+1);
				AddElement(P,ubar);
				// P[++P[0]] = ubar;
				loadk += GetDemand(ubar,ES);
				// costk += GetCost(ubar,ES);
				//std::cout<<loadk<<"*"<<ES[ubar].demand<<" ";
			//	i = GetHead(ubar,ES);
				ei = ubar;
//			Trail(freeL);
				//exit(0);
			}else{
				break;
			}
		}
	}
	// P[++P[0]] = 0;
	// AddElement(P,0,P[0]+1);
	AddElement(P,0);
}

//template<typename PATH>
void path_scanning(PATH& P,EMAP& ES,DMatrix& D,int task_num,int& Q,int rule,int* (freeL)) {
	int sel,u,ubar;
	int loadk,costk,i,ei;
	DType qu;
	VType dbar;
	P[0] = 0;
	DType distance;
	int idx;
	while(freeL[0]>0){
		P[++P[0]] = 0;
		loadk = 0;
		ei = Edepot;
		dbar = Edepot;
		while(freeL[0]>0&&dbar!=INF){
			dbar = INF;
			for(idx=1;idx<=freeL[0];++idx){
				u  = freeL[idx];
				if(loadk + ES[u].demand <=Q){
					//distance = D[ES[ei-depot_not_0].head][ES[u-depot_not_0].tail];
					distance = Distance(ei,u,ES,D);//[ES[ei-depot_not_0].head][ES[u-depot_not_0].tail];
					if(distance<dbar){
						dbar = distance;
						ubar = u;
					}else{
						if(dbar==distance && betteru(u,ubar,rule,loadk,ES,D,Q) ){
							ubar = u;
						}
					}
				}
			}
			if(dbar != INF){
				int Ra = ReverseElement(ubar);
				for(i=freeL[0];i>=1;i--){
					if(freeL[i] == ubar || freeL[i] == Ra){
						Swap(freeL,i,freeL[0]);
						freeL[0]--;
					}
				}
				P[++P[0]] = ubar;
				loadk += ES[ubar].demand;
				ei = ubar;
			}else{
				break;
			}
		}
	}
	P[++P[0]] = 0;
}

template<typename EMAP,typename DMatrix,typename PATH>
void path_scanning(PATH& P,EMAP &ES,DMatrix& D,int& task_num,DType Q, int* freeL){
	PATH PS[5];
	VType Cost[5],minV=INF;
	int sel;
	int freeLT[DMS];
	for (int i = 4; i >0;i--) {
		for(int j=task_num;j>=0;j--){
			freeLT[j] = freeL[j];
		}
		path_scanning(PS[i],ES,D,task_num,Q,i+1,freeLT);
		Cost[i] = ValueCal(PS[i],ES,D);	
		if (Cost[i]<minV) {
			sel = i;
			minV = Cost[i];
		}
	}
	CopyP(P,PS[sel]);
}


//template<typename EMAP,typename DMatrix,typename PATH>
void path_scanning(IndividualModel &I,EMAP &ES,DMatrix& D,int task_num,DType& Q,int rule,int* freeL){
	path_scanning(I.P,ES,D,task_num,Q,rule,freeL);
}

template<typename EMAP,typename DMatrix,typename PATH>
void path_scanning(IndividualModel &I,EMAP &ES,DMatrix& D,int task_num,DType& Q,int rule,int* freeL){
	path_scanning(I.P,ES,D,task_num,Q,rule,freeL);
}

//template<typename IndividualModel,typename EMAP,typename DMatrix>
void path_scanning(IndividualModel &I,EMAP &ES,DMatrix& D,int task_num,DType& Q,int rule){
	// int k = 0;
	int sel,u,ubar;
	PATH freeL;
	for(int j=task_num;j>=0;j--){
		freeL[j] = j;
	}
	freeL[0] = task_num;
	path_scanning(I.P,ES,D,task_num,Q,rule,freeL);
}

template<typename IndividualModel,typename EMAP,typename DMatrix>
void path_scanning(IndividualModel &I,EMAP &ES,DMatrix& D,int task_num,DType& Q,int rule){
	// int k = 0;
	int sel,u,ubar;
	PATH freeL;
	for(int j=task_num;j>=0;j--){
		freeL[j] = j;
	}
	freeL[0] = task_num;
	path_scanning(I.P,ES,D,task_num,Q,rule,freeL);
}


//template<typename PATH1,typename EMAP,typename DMatrix>
void path_scanning(PATH& P,EMAP &ES,DMatrix& D,int task_num,DType& Q){
	PATH PS[5];
	VType Cost[5],minV=INF;
	int sel;
	PATH freeL;
	for (int i = 4; i >0;i--) {
		for(int j=task_num;j>=0;j--){
			freeL[j] = j;
		}
		freeL[0] = task_num;
		path_scanning(PS[i],ES,D,task_num,Q,i+1,freeL);
		//Trail(PS[i]);
		Cost[i] = ValueCal(PS[i],ES,D);	
		//cout<<Cost[i]<<endl;
		if (Cost[i]<minV) {
			sel = i;
			minV = Cost[i];
		}
	}
	CopyP(P,PS[sel]);
}

//template<typename IndividualModel,typename EMAP,typename DMatrix>
inline void path_scanning(IndividualModel &I,EMAP &ES,DMatrix& D,int task_num,DType& Q){
	PATH P;
	path_scanning(P,ES,D,task_num,Q);
	CopyP(I.P,P);
	FitnessCal1(I,ES,D);
}

//template<typename PATH1,typename EMAP,typename DMatrix>
// void path_scanning(PATH& P,EMAP &ES,DMatrix& D,int task_num,DType& Q){
// 	int k = 0;
// 	int PS[5][ Max_PATH_len];
// 	int freeLCopy[ Max_PATH_len];
// 	VType Cost[5],minV=INF;
// 	PATH  freeL;
// 	int sel,u,ubar;
// 	for (int i = 0; i <5;i++) {
// 		for(int j=0;j<=task_num;++j){
// 			freeLCopy[j] = freeL[j];
// 		}
// 		path_scanning(PS[i],ES,D,task_num,Q,i+1,freeLCopy);
// 		Cost[i] = ValueCal(PS[i],ES,D);	
// 		if (Cost[i]<minV) {
// 			sel = i;
// 			minV = Cost[i];
// 		}
// 	}
// 	CopyP(P,PS[sel]);
// }

/****************************************************************************************/

/************************************** Augment_merge ***********************************/

void QSORT(int low,int height,VType* cost,int (&R)[DMS][DMS],int * load){
	if (low>=height) return;
	int i = low;
	int j = height;
	int key = cost[low];
	static int P[2000];
	while(1){
		while (cost[i]>=key && ++i<height) {}
		while (cost[j]<=key && --j>low) {}
		if (i>=j) break;
		CopyP(P,R[j]);
		CopyP(R[j],R[i]);
		CopyP(R[i],P);
		Swap(cost,i,j);
		Swap(load,i,j);
	}
	if (j!=low) {
		CopyP(P,R[low]);
		CopyP(R[low],R[j]);
		CopyP(R[j],P);
		Swap(cost,j,low);
		Swap(load,j,low);
	}
	QSORT(low,j-1,cost,R,load);
	QSORT(j+1,height,cost,R,load);
}

//template<typename PATH ,typename EMAP, typename DMatrix>
bool AugmentMerge(PATH &P, EMAP &ES , DMatrix &D,int lenES, DType& Q,int *RE_list){
	int k = 0;
	int m = lenES/2;
	// 这里
	VType  pk[lenES+1]{(VType)lenES};
	DType  load[lenES+1]{(DType)lenES};
	static VType  cost[Max_Path_len]{(VType)lenES};
	static		int RS[5000][5000];
	int u,v,x,y;
	int seltag;
	int i,j;
	// initial routes
	for(i=1;i<=RE_list[0];++i){
		seltag = RE_list[i];
		RS[i][0] = 3;
		RS[i][1] = Edepot;
		RS[i][2] = seltag;
		RS[i][3] = Edepot;
		load[i] = GetDemand(seltag,ES);
		cost[i] = Distance(Edepot,seltag,ES,D) + GetCost(seltag,ES) + Distance(seltag,Edepot,ES,D);
	}
	int flag = 0;
	int t = m;
	QSORT(1,m,cost,RS,load);
	int exflag = 0;
	// Augment phase
	for(int k=1; k<=t; ++k){
		if(RS[k][0]<3) continue;
		int x = RS[k][2];
		int y = RS[k][RS[k][0]-1 ];
		for(int p = k+1;p<=t;p++){
			if (k==p) continue;
			if(RS[p][0]<3) continue;
			if(RS[p][0]>=3 && load[k]+load[p]<=Q){
				for (int m=0;m<2;m++) {
					int ep = RS[p][2];
					if (m==1) ep = ReverseElement(ep);
					if (Distance(ep,Edepot,ES,D)==0 && Distance(y,ep,ES,D)==0){
						InsertElement(RS[k],ep,RS[k][0]);
						load[k] += load[p];
						cost[k] += GetCost(ep,ES)-Distance(y,Edepot,ES,D);
						RS[p][0] = 0;
						load[p] = 0;
						cost[p] = 0;
						y = Edepot;
						exflag = 1;
						break;
					}
					if (Distance(Edepot,ep,ES,D)==0 && Distance(ep,x,ES,D)==0) {
						InsertElement(RS[k],ep,2);
						load[k] += load[p];
						cost[k] += GetCost(ep,ES)-Distance(Edepot,x,ES,D);
						RS[p][0] = 0;
						load[p] = 0;
						cost[p] = 0;
						exflag = 1;
						x = Edepot;
						break;
					}
				}
			}
			if (exflag == 1) {
				exflag = 0;
				break;
			}
		}
	}
	// construct lamda 

	static eSortType lamda[2*5000*5000];
	eSortType ch;
	int sk = 0,u1,v1,u2,v2,mode;	
	for(int ts=0;ts<1;ts++){
		QSORT(1,m,cost,RS,load);
		for (int k=1;k<=m;k++) {
			if (RS[k][0] ==0 || cost[k]==0) { 
				m=k;
				break;
			}
		}
		sk = 0;
		for (int k=1; k<=m;k++) {
			u1 = RS[k][2];
			v1 = RS[k][RS[k][0]-1 ];
			for (int p=k+1; p<=m;p++) {
				if (k==p) continue;
				u2 = RS[p][2];
				v2 = RS[p][RS[p][0]-1 ];
				if (load[k]+load[p]>Q) continue;
				for (	mode=0;mode<4;mode++) {
					(mode%2 == 1) ? u=v1 : u = ReverseElement(u1);
					(mode<2)      ? v=u2 : v = ReverseElement(v2);
					lamda[sk].k = k;
					lamda[sk].p = p;
					lamda[sk].u = u;
					lamda[sk].v = v;
					lamda[sk].mode = mode;
					lamda[sk].saving = -Distance(u,v,ES,D);
					lamda[sk].saving += Distance(u,Edepot,ES,D); 	
					lamda[sk].saving += Distance(Edepot,v,ES,D); 	
					if (lamda[sk].saving > 0) sk++;
				}
			}
		}
		if (sk == 0) break;
		// 0 u1 u2 
		// 1 v1 u2
		// 2 u1 v2
		// 3 v1 v2
		sort(lamda,lamda+sk);	
		int p;
		int max_sk = 500;
		for (int s=0;s<sk;s++) {
			// for (int tsk=1;tsk<=m;tsk++) {
			// 	cout<<tsk<<" cost tsk "<<ValueCal(RS[tsk],ES,D)<<" "<<cost[tsk]<<endl;
			// 	Trail(RS[tsk]);
			// 	if (ValueCal(RS[tsk],ES,D) != cost[tsk]){
			// 		Trail(RS[tsk]);
			// 		cout<<" cost tsk"<<endl;
			// 		exit(0);
			// 	}
			// }
			u = lamda[s].u;
			v = lamda[s].v;
			k =	lamda[s].k;
			p =	lamda[s].p;
			mode = lamda[s].mode;
			if (RS[k][0] < 3 || RS[p][0] < 3)   continue;
			if (load[k]+load[p]>Q) continue;
			if (lamda[s].saving<0) break;
			// if (mode%2==0&& RS[k][2] != ReverseElement(u) ) continue;
			// if (mode%2   && RS[k][RS[k][0]-1] != u) continue;
			// if (mode<2   && RS[p][RS[p][0]-1] != ReverseElement(v) ) continue;
			// if (mode>=2  && RS[p][2] != v) continue;
		// cout<<(mode%2==0&& RS[k][2] != ReverseElement(u) )<<" ";
		// cout<<(mode%2   && RS[k][RS[k][0]-1] != u)<<" ";
		// cout<<(mode<2   && RS[p][2] !=  v) <<" ";
		// cout<<(mode>=2  && RS[p][RS[p][0]-1] != ReverseElement(v))<<" ";
			if ( (mode%2==0&& RS[k][2] != ReverseElement(u))
			|| (mode%2   && RS[k][RS[k][0]-1] != u) 
			|| (mode<2   && RS[p][2] !=  v)
			|| (mode>=2  && RS[p][RS[p][0]-1] != ReverseElement(v)) ){
				u1 = RS[k][2];
				v1 = RS[k][RS[k][0]-1 ];
				u2 = RS[p][2];
				v2 = RS[p][RS[p][0]-1 ];
				(mode%2 == 1) ? u=v1 : u = ReverseElement(u1);
				(mode<2)      ? v=u2 : v = ReverseElement(v2);
				lamda[s].k = k;
				lamda[s].p = p;
				lamda[s].u = u;
				lamda[s].v = v;
				lamda[s].mode = mode;
				lamda[s].saving = -Distance(u,v,ES,D);
				lamda[s].saving += Distance(u,Edepot,ES,D); 	
				lamda[s].saving += Distance(Edepot,v,ES,D); 	
				if (sk-s>max_sk) {
					sort(lamda+s,lamda+s+max_sk);
				} else {
					sort(lamda+s,lamda+sk);
				}
				if (s%max_sk == 0) sort(lamda+s,lamda+sk);
			// if(s<sk && lamda[s+1]<lamda[s]) s--;
				// if (lamda[s+1]<lamda[s] ) {
				// 	ch = lamda[s+1];
				// 	lamda[s+1] = lamda[s];
				// 	lamda[s] = ch;
				// 	s--;
				// }
				s--;
				//cout<<s<<" "<<sk<<endl;
				continue;
			}
		//	cout<<s<<" "<<endl;
			// mode = lamda[s].mode;
			// if (k>m||p>m||k==0||p==0) continue;
			// if (RS[p][0] < 3) continue;	
			// if (RS[k][0] < 3) continue;	
			// if (load[k]+load[p]>Q) continue;
			// if (mode%2==0&& RS[k][2] != ReverseElement(u) ) continue;
			// if (mode%2   && RS[k][RS[k][0]-1] != u) continue;
			// if (mode>=2  && RS[p][RS[p][0]-1] != ReverseElement(v) ) continue;
			// if (mode<2   && RS[p][2] != v) continue;
			if (mode%2==0) { ReverseElementR(RS[k],1,RS[k][0]); }
			if (mode>=2)   { ReverseElementR(RS[p],1,RS[p][0]); }
			
			if (ValueCal(RS[k],ES,D) > cost[k]){
				ReverseElementR(RS[k],1,RS[k][0]);
				continue;
			}
			if (ValueCal(RS[p],ES,D) > cost[p]){
				ReverseElementR(RS[p],1,RS[p][0]);
				continue;
			}
			// if (ValueCal(RS[k],ES,D) != cost[k]){
			// 	Trail(RS[k]);
			// 	cout<<lamda[s].mode;
			// 	cout<<" cost k "<<k<<" "<<ValueCal(RS[k],ES,D)<<" != "<<cost[k]<<endl;
			// 	exit(0);
			// }
			// 问题.风向路径会出问题
			// if (ValueCal(RS[p],ES,D) != cost[p]){
			// 	Trail(RS[p]);
			// 	cout<<lamda[s].mode;
			// 	cout<<" cost p "<<p<<" "<<ValueCal(RS[p],ES,D)<<" != "<<cost[p]<<endl;
			// 	exit(0);
			// }
			DeleteElement(RS[p],1);
			InsertElement(RS[k],RS[p],RS[k][0]);
			RS[k][0]--;
			RS[p][0] = 0;
			load[k] += load[p];
			cost[k] += cost[p];
			cost[k] -= lamda[s].saving;
			cost[p] = 0;
			load[p] = 0;
			if (ValueCal(RS[k],ES,D) != cost[k]){
				DeleteManyZero(RS[k]);
				cost[k] = ValueCal(RS[k],ES,D);
				int lenrs = RS[k][0];
				DeleteManyZero(RS[k]);
				cout<<"可能存在风向路径 "<<endl;
				// cout<<k<<" 可能存在风向路径 "<<cost[k]<<" "<<endl;
				// Trail(RS[k]);
				// exit(0);
				// cout<<lamda[s].mode;
				if(RS[k][0] != lenrs) {
					cout<<"lens "<<lenES<<endl;
					cout<<k<<" 可能存在风向路径 "<<cost[k]<<" "<<cost[p]<<" "<<lamda[s].saving<<endl;
					Trail(RS[k]);
					cout<<ValueCal(RS[k],ES,D)<<endl; 
					exit(0);
				}
			}
		}
	}
	P[0] = 1;
	P[1] = 0;
	int c = 0,d=0;
	for (int i=1;i<=m;i++) {
		if (RS[i][0]>=3) {
			d += ValueCal(RS[i],ES,D);
			if( ValueCal(RS[i],ES,D)!=cost[i]){
				//Trail(RS[i]);
				cout<<i<<" "<<ValueCal(RS[i],ES,D)<<" 不0等 "<<cost[i]<<" ";
				// cout<<ValueCal(RS[i],ES,D)<<endl; 
			}
			DeleteElement(RS[i],1);
			InsertElement(P,RS[i],P[0]+1);
			c += cost[i];
		}
	}
	// Trail(P);
//	int e=ValueCal(P,ES,D);
//	Trail(P);
 // cout<<c<<" "<<d<<" "<<endl; 
	// cout<<"lens "<<lenES<<endl;
	return 1;   
}

template<typename PATH ,typename EMAP, typename DMatrix>
bool AugmentMerge(PATH &P, EMAP &ES , DMatrix &D,int lenES, DType& Q,int *RE_list){
	int k = 0;
	int m = lenES/2;
	// 这里
	VType  pk[lenES+1]{(VType)lenES};
	DType  load[lenES+1]{(DType)lenES};
	static VType  cost[Max_Path_len]{(VType)lenES};
	static		int RS[5000][5000];
	int u,v,x,y;
	int seltag;
	int i,j;
	// initial routes
	for(i=1;i<=RE_list[0];++i){
		seltag = RE_list[i];
		RS[i][0] = 3;
		RS[i][1] = Edepot;
		RS[i][2] = seltag;
		RS[i][3] = Edepot;
		load[i] = GetDemand(seltag,ES);
		cost[i] = Distance(Edepot,seltag,ES,D) + GetCost(seltag,ES) + Distance(seltag,Edepot,ES,D);
	}
	int flag = 0;
	int t = m;
	QSORT(1,m,cost,RS,load);
	int exflag = 0;
	// Augment phase
	for(int k=1; k<=t; ++k){
		if(RS[k][0]<3) continue;
		int x = RS[k][2];
		int y = RS[k][RS[k][0]-1 ];
		for(int p = k+1;p<=t;p++){
			if (k==p) continue;
			if(RS[p][0]<3) continue;
			if(RS[p][0]>=3 && load[k]+load[p]<=Q){
				for (int m=0;m<2;m++) {
					int ep = RS[p][2];
					if (m==1) ep = ReverseElement(ep);
					if (Distance(ep,Edepot,ES,D)==0 && Distance(y,ep,ES,D)==0){
						InsertElement(RS[k],ep,RS[k][0]);
						load[k] += load[p];
						cost[k] += GetCost(ep,ES)-Distance(y,Edepot,ES,D);
						RS[p][0] = 0;
						load[p] = 0;
						cost[p] = 0;
						y = Edepot;
						exflag = 1;
						break;
					}
					if (Distance(Edepot,ep,ES,D)==0 && Distance(ep,x,ES,D)==0) {
						InsertElement(RS[k],ep,2);
						load[k] += load[p];
						cost[k] += GetCost(ep,ES)-Distance(Edepot,x,ES,D);
						RS[p][0] = 0;
						load[p] = 0;
						cost[p] = 0;
						exflag = 1;
						x = Edepot;
						break;
					}
				}
			}
			if (exflag == 1) {
				exflag = 0;
				break;
			}
		}
	}
	// construct lamda 
	typedef struct eSortType{
		VType saving;
		int k,p,mode,u,v;
	    bool operator <(const eSortType &other)const
	    {
	        return saving > other.saving;
	    }	
	}eSortType;
	static eSortType lamda[2*5000*5000];
	eSortType ch;
	int sk = 0,u1,v1,u2,v2,mode;	
	for(int ts=0;ts<1;ts++){
		QSORT(1,m,cost,RS,load);
		for (int k=1;k<=m;k++) {
			if (RS[k][0] ==0 || cost[k]==0) { 
				m=k;
				break;
			}
		}
		sk = 0;
		for (int k=1; k<=m;k++) {
			u1 = RS[k][2];
			v1 = RS[k][RS[k][0]-1 ];
			for (int p=k+1; p<=m;p++) {
				if (k==p) continue;
				u2 = RS[p][2];
				v2 = RS[p][RS[p][0]-1 ];
				if (load[k]+load[p]>Q) continue;
				for (	mode=0;mode<4;mode++) {
					(mode%2 == 1) ? u=v1 : u = ReverseElement(u1);
					(mode<2)      ? v=u2 : v = ReverseElement(v2);
					lamda[sk].k = k;
					lamda[sk].p = p;
					lamda[sk].u = u;
					lamda[sk].v = v;
					lamda[sk].mode = mode;
					lamda[sk].saving = -Distance(u,v,ES,D);
					lamda[sk].saving += Distance(u,Edepot,ES,D); 	
					lamda[sk].saving += Distance(Edepot,v,ES,D); 	
					if (lamda[sk].saving > 0) sk++;
				}
			}
		}
		if (sk == 0) break;
		// 0 u1 u2 
		// 1 v1 u2
		// 2 u1 v2
		// 3 v1 v2
		sort(lamda,lamda+sk);	
		int p;
		int max_sk = 500;
		for (int s=0;s<sk;s++) {
			// for (int tsk=1;tsk<=m;tsk++) {
			// 	cout<<tsk<<" cost tsk "<<ValueCal(RS[tsk],ES,D)<<" "<<cost[tsk]<<endl;
			// 	Trail(RS[tsk]);
			// 	if (ValueCal(RS[tsk],ES,D) != cost[tsk]){
			// 		Trail(RS[tsk]);
			// 		cout<<" cost tsk"<<endl;
			// 		exit(0);
			// 	}
			// }
			u = lamda[s].u;
			v = lamda[s].v;
			k =	lamda[s].k;
			p =	lamda[s].p;
			mode = lamda[s].mode;
			if (RS[k][0] < 3 || RS[p][0] < 3)   continue;
			if (load[k]+load[p]>Q) continue;
			if (lamda[s].saving<0) break;
			// if (mode%2==0&& RS[k][2] != ReverseElement(u) ) continue;
			// if (mode%2   && RS[k][RS[k][0]-1] != u) continue;
			// if (mode<2   && RS[p][RS[p][0]-1] != ReverseElement(v) ) continue;
			// if (mode>=2  && RS[p][2] != v) continue;
		// cout<<(mode%2==0&& RS[k][2] != ReverseElement(u) )<<" ";
		// cout<<(mode%2   && RS[k][RS[k][0]-1] != u)<<" ";
		// cout<<(mode<2   && RS[p][2] !=  v) <<" ";
		// cout<<(mode>=2  && RS[p][RS[p][0]-1] != ReverseElement(v))<<" ";
			if ( (mode%2==0&& RS[k][2] != ReverseElement(u))
			|| (mode%2   && RS[k][RS[k][0]-1] != u) 
			|| (mode<2   && RS[p][2] !=  v)
			|| (mode>=2  && RS[p][RS[p][0]-1] != ReverseElement(v)) ){
				u1 = RS[k][2];
				v1 = RS[k][RS[k][0]-1 ];
				u2 = RS[p][2];
				v2 = RS[p][RS[p][0]-1 ];
				(mode%2 == 1) ? u=v1 : u = ReverseElement(u1);
				(mode<2)      ? v=u2 : v = ReverseElement(v2);
				lamda[s].k = k;
				lamda[s].p = p;
				lamda[s].u = u;
				lamda[s].v = v;
				lamda[s].mode = mode;
				lamda[s].saving = -Distance(u,v,ES,D);
				lamda[s].saving += Distance(u,Edepot,ES,D); 	
				lamda[s].saving += Distance(Edepot,v,ES,D); 	
				if (sk-s>max_sk) {
					sort(lamda+s,lamda+s+max_sk);
				} else {
					sort(lamda+s,lamda+sk);
				}
				if (s%max_sk == 0) sort(lamda+s,lamda+sk);
			// if(s<sk && lamda[s+1]<lamda[s]) s--;
				// if (lamda[s+1]<lamda[s] ) {
				// 	ch = lamda[s+1];
				// 	lamda[s+1] = lamda[s];
				// 	lamda[s] = ch;
				// 	s--;
				// }
				s--;
				//cout<<s<<" "<<sk<<endl;
				continue;
			}
		//	cout<<s<<" "<<endl;
			// mode = lamda[s].mode;
			// if (k>m||p>m||k==0||p==0) continue;
			// if (RS[p][0] < 3) continue;	
			// if (RS[k][0] < 3) continue;	
			// if (load[k]+load[p]>Q) continue;
			// if (mode%2==0&& RS[k][2] != ReverseElement(u) ) continue;
			// if (mode%2   && RS[k][RS[k][0]-1] != u) continue;
			// if (mode>=2  && RS[p][RS[p][0]-1] != ReverseElement(v) ) continue;
			// if (mode<2   && RS[p][2] != v) continue;
			if (mode%2==0) { ReverseElementR(RS[k],1,RS[k][0]); }
			if (mode>=2)   { ReverseElementR(RS[p],1,RS[p][0]); }
			
			if (ValueCal(RS[k],ES,D) > cost[k]){
				ReverseElementR(RS[k],1,RS[k][0]);
				continue;
			}
			if (ValueCal(RS[p],ES,D) > cost[p]){
				ReverseElementR(RS[p],1,RS[p][0]);
				continue;
			}
			// if (ValueCal(RS[k],ES,D) != cost[k]){
			// 	Trail(RS[k]);
			// 	cout<<lamda[s].mode;
			// 	cout<<" cost k "<<k<<" "<<ValueCal(RS[k],ES,D)<<" != "<<cost[k]<<endl;
			// 	exit(0);
			// }
			// 问题.风向路径会出问题
			// if (ValueCal(RS[p],ES,D) != cost[p]){
			// 	Trail(RS[p]);
			// 	cout<<lamda[s].mode;
			// 	cout<<" cost p "<<p<<" "<<ValueCal(RS[p],ES,D)<<" != "<<cost[p]<<endl;
			// 	exit(0);
			// }
			DeleteElement(RS[p],1);
			InsertElement(RS[k],RS[p],RS[k][0]);
			RS[k][0]--;
			RS[p][0] = 0;
			load[k] += load[p];
			cost[k] += cost[p];
			cost[k] -= lamda[s].saving;
			cost[p] = 0;
			load[p] = 0;
			if (ValueCal(RS[k],ES,D) != cost[k]){
				// cout<<" 可能存在风向路径 "<<cost[k]<<" "<<ValueCal(RS[k],ES,D)<<endl;
				// Trail(RS[k]);
				// ValueCal(RS[k],ES,D);
				// exit(0);
				DeleteManyZero(RS[k]);
				cost[k] = ValueCal(RS[k],ES,D);
				int lenrs = RS[k][0];
				DeleteManyZero(RS[k]);
				cout<<"可能存在风向路径 "<<endl;
				// cout<<lamda[s].mode;
				if(RS[k][0] != lenrs) {
					cout<<"lens "<<lenES<<endl;
					cout<<k<<" 可能存在风向路径 "<<cost[k]<<" "<<cost[p]<<" "<<lamda[s].saving<<endl;
					Trail(RS[k]);
					cout<<ValueCal(RS[k],ES,D)<<endl; 
					exit(0);
				}
			}
		}
	}
	P[0] = 1;
	P[1] = 0;
	int c = 0,d=0;
	for (int i=1;i<=m;i++) {
		if (RS[i][0]>=3) {
			d += ValueCal(RS[i],ES,D);
			if( ValueCal(RS[i],ES,D)!=cost[i]){
				cout<<i<<" "<<ValueCal(RS[i],ES,D)<<" 不等= "<<cost[i]<<" ";
				Trail(RS[i]);
				cout<<ValueCal(RS[i],ES,D)<<endl; 
				exit(0);
			}
			DeleteElement(RS[i],1);
			InsertElement(P,RS[i],P[0]+1);
			c += cost[i];
		}
	}
	// Trail(P);
//	int e=ValueCal(P,ES,D);
//	Trail(P);
 // cout<<c<<" "<<d<<" "<<endl; 
	// cout<<"lens "<<lenES<<endl;
	return 1;   
}

bool c_in_path_a_b(int a,int b,int c,EMAP& ES,DMatrix& D) {
	// if c in the (a,b)
	int Dab = Distance(a,b,ES,D);
	int Dac = Distance(a,c,ES,D);
	int Dcb = Distance(c,b,ES,D);
	VType Ccost = GetCost(c,ES);
	if (Dab == Dac+Dcb+Ccost) 
		return 1;
	else 
		return 0;
}

bool can_smaller_detour_c_in_a_b(int a,int b,int c,EMAP& ES,DMatrix& D) {
	// if c in the (a,b)
	int Dab = Distance(a,b,ES,D);
	int Dac = Distance(a,c,ES,D);
	int Dcb = Distance(c,b,ES,D);
	VType Ccost = GetCost(c,ES);
	if (Dab < Dac+Dcb) 
		return 1;
	else 
		return 0;
}

template<typename A,typename B>
void idx_sorted_by_value(A& idxs,B& value) {
	int i,j;
	idxs[0] = value[0];
	for (int i=1;i<=value[0];i++) {
		idxs[i] = i;
	}
	for (int i=1;i<=value[0];i++) {
		for (int j=1;j<=value[0];j++) {
			if(value[idxs[i]]>value[idxs[j]]) {
				Swap(idxs,i,j);
			}
		}
	}
//	for (int i=1;i<=value[0];i++) {
//		cout<<value[idxs[i]]<<" ";
//	}
//	exit(0);
}

DType cal_total_demand(int * P,EMAP& ES,DMatrix& D) {
  DType tal = 0;
  for (int i=1;i<=P[0];i++) {
		tal += GetDemand(P[i],ES);
	}
  return tal;
}

int cal_min_vehicle_num(int * P,EMAP& ES,DMatrix& D,int Q) {
  int tal = 0;
  int  num_t = cal_total_demand(P,ES,D)/Q;
	//tal = num_t+(cal_total_demand(P,ES,D)>num_t*Q);
	//cout<<cal_total_demand(P,ES,D)<<" "<<(cal_total_demand(P,ES,D)>(2*Q))<<endl;
	tal = ceil((double)cal_total_demand(P,ES,D)/Q);
  return tal;
}

VType detour_saving_value(int a,int b,int c,EMAP& ES,DMatrix& D) {
	int Dab = Distance(a,b,ES,D);
	int Dac = Distance(a,c,ES,D);
	int Dcb = Distance(c,b,ES,D);
  return Dac+Dcb-Dab;
}

//template<typename PATH ,typename EMAP, typename DMatrix>
bool Parallel_Insert(PATH &P, EMAP &ES , DMatrix &D,int lenES, DType& Q,int *RE_list){
	// RE_list without rev(e)
	DeleteAllZero(RE_list);
	int k = 0;
	VType minDIJ = INF,DIJ;
	int idx,sel_e,e;
	PATH orders,served;
	idx_sorted_by_value(orders,RE_list);
	DType  load[lenES+1]{(DType)lenES};
	static VType  cost[Max_Path_len]{(VType)lenES};
	static int RS[5000][5000];
	int all_closed = 0,all_served = 0;
	int m = cal_min_vehicle_num(RE_list,ES,D,Q);
	// find farthest edge
	//for(int i=1;i<=RE_list[0];i++) {
	//	e = RE_list[i];
	//	DIJ = Distance(e,Edepot,ES,D) + Distance(Edepot,e,ES,D);
	//	if (DIJ<minDIJ) {
	//		minDIJ = DIJ;
	//		sel_e = e;
	//		idx = i;
	//	}
	served[0] = RE_list[0];
	for(int i=1;i<=RE_list[0];i++) {
		served[i]=0;
	}
	RS[0][0] = 3;
	RS[0][1] = 0;
	RS[0][2] = RE_list[orders[1]];
	RS[0][3] = 0;
	load[0] = GetDemand(RS[0][2],ES);
	//DeleteElement(orders,1);
	served[orders[1]] = 1;
	// first insert like Augment in AM
	int number_route=1;
	int dem,ei,ej,detour_value;
	PATH closed;
	for (int i=0;i<number_route;i++) {
		closed[i]= 0;
	}
	while(1) {
		for (int i=2;i<=RE_list[0];i++) {
			idx =	orders[i]; 
			e = RE_list[idx];
			dem = GetDemand(e,ES);
			int is_in = 0;
			if (!served[idx]) {
				for (int k=0;k<number_route;k++) {
					if (load[k]+dem>Q) continue; 
					int len = RS[k][0];
					for (int j=1;j<RS[k][0];j++) {
						//cout<<j<<" , ";
						ei = RS[k][j];
						ej = RS[k][j+1];
						if (c_in_path_a_b(ei,ej,e,ES,D)) {
							InsertElement(RS[k],e,j+1);
							is_in = 1;
							served[idx] = 1;
							load[k] += GetDemand(e,ES);
							break;
						}
						if (c_in_path_a_b(ei,ej,ReverseElement(e),ES,D)) {
							InsertElement(RS[k],ReverseElement(e),j+1);
							is_in = 1;
							served[idx] = 1;
							load[k] += GetDemand(e,ES);
							break;
						}
					}
					if (is_in) break;
				}
				if (!is_in && number_route<m) {
					RS[number_route][0] = 3;
					RS[number_route][1] = 0;
					RS[number_route][2] = e;
					RS[number_route][3] = 0;
					load[number_route] = GetDemand(e,ES);
					++number_route;
					served[idx] = 1;
				}
			}
		}
		//Trail(RS[0]);
		//cout<<number_route<<" 2\n";
		// fill the open lowest load route C
		// find the minimum load of theC
		// insert to the minload route 
		//cout<<"3\n";
		int min_idx;
		while(!all_served&&!all_closed){
			int min_load = INF;
			idx = 0;
			for (int i=0;i<number_route;i++) {
				if(!closed[i]&& load[i]<min_load) {
					// [closed[i]= 0;
					min_load = load[i];
					min_idx = i;
				}
			}
			int smallest_deotur=0;
			int idx_detour,e_detour;
		//cout<<"31\n";
			//int is_in = 0;
			int eidx;
			for (int i=1;i<=RE_list[0];i++) {
				idx =	orders[i]; 
				if (served[idx]) continue; 
				e = RE_list[idx];
				dem = GetDemand(e,ES);
				k = min_idx;
				if (load[k]+dem>Q) continue; 
				// smallest detour
				int len = RS[k][0];
				//for (int j=1;j<RS[k][0];j++) {
				for (int j=1;j<len;j++) {
					ei = RS[k][j];
					ej = RS[k][j+1];
					detour_value = detour_saving_value(ei,ej,e,ES,D);
					//if (detour_value<
					if (detour_value<smallest_deotur) {
						smallest_deotur = detour_value;
						idx_detour = j+1;
						e_detour = e;
						eidx = idx;
					}
					detour_value = detour_saving_value(ei,ej,ReverseElement(e),ES,D);
					if (detour_value<smallest_deotur) {
						smallest_deotur = detour_value;
						idx_detour = j+1;
						e_detour = ReverseElement(e);
						eidx = idx;
					}
				}
			}
		//cout<<"3w"<<smallest_deotur<<endl;
			if (smallest_deotur<0) {
				InsertElement(RS[k],e_detour,idx_detour);
				served[eidx] = 1;
			} else {
				closed[min_idx] = 1;
			}
			for (int i=0;i<number_route;i++) {
				if (closed[i]==0) {
					all_closed = 0;
					break;
				}
				all_closed = 1;
			}
			for (int i=1;i<=served[0];i++) {
				if (served[i]==0) {
					all_served = 0;
					break;
				}
				all_served = 1;
			}
		//Trail(served);
		//Trail(closed);
		//sleep(3);
		}
			for (int i=1;i<=served[0];i++) {
				if (served[i]==0) {
					all_served = 0;
					break;
				}
				all_served = 1;
			}
			if (all_served) break;
			// the farest to new a route
			for (int i=1;i<=RE_list[0];i++) {
			idx =	orders[i]; 
			e = RE_list[idx];
			dem = GetDemand(e,ES);
			int is_in = 0;
			if (!served[idx]) {
				if (number_route) {
					RS[number_route][0] = 3;
					RS[number_route][1] = 0;
					RS[number_route][2] = e;
					RS[number_route][3] = 0;
					load[number_route] = GetDemand(e,ES);
					served[idx] = 1;
					++number_route;
					break;
				}
			}
		}
		//cout<<"all_served "<<all_served;
		//Trail(served);
		//Trail(closed);
	}
	P[0] = 1;
	P[1] = 0;
	for (int i=0;i<=number_route;i++) {
		if (RS[i][0]>=3) {
		//	Trail(RS[i]);
			DeleteElement(RS[i],1);
			InsertElement(P,RS[i],P[0]+1);
		}
	}
	return 1;
}


template<typename PATH,typename EMAP, typename DMatrix>
bool UlusoySplit(PATH & Ps, EMAP & ES , DMatrix &D, DType& Q ) {
	// split to routes from (e1 e2 e3) think about the end, our is (ei),
	// define the distance of e1,e2 is e1.end e2.end.  	int len = P[0];
	int len = Ps[0];
	int S[Max_Path_len];
	Decode(S,Ps);
	DeleteAllZero(S);
	len = S[0];	
	DType load;
	VType cost;
	// int t = len;
	int i,j,x;
	// int u,v,x,y;
	static int V[Max_Path_len];
	for(int pp=0;pp<=len;pp++) {
		V[pp] = INF;
	}
	static int P[Max_Path_len];
	V[0] = 0;
	P[0] = 0;
	for(i = 1;i<=len;i++){
		load = 0;
		cost = 0;
		j = i;
		while( load<=Q&&j<=len ){
			if(S[j] != 0){
				if (GetDemand(S[j],ES) > Q) {
					cout<<" maybe not right length of ES ";
					cout<<GetDemand(S[j],ES)<<" >Q ";
					cout<<S[j]<<","<<GetTail(S[j],ES)<<","<<GetHead(S[j],ES)<<","<<GetDemand(S[j],ES)<<","<<GetCost(S[j],ES);
					exit(0);
				}
				load += GetDemand(S[j],ES);
			}
			if (load > Q) break;
			if(j==i){
				// cost += Distance(Edepot,S[j],ES,D) + GetCost(S[j],ES) + Distance(S[j],Edepot,ES,D);
				cost = Distance(Edepot,S[j],ES,D) + GetCost(S[j],ES) + Distance(S[j],Edepot,ES,D);
			}else{
				// cost += Distance(x,S[j],ES,D) + GetCost(S[j],ES) + Distance(S[j],Edepot,ES,D) - Distance(x,Edepot,ES,D) ;
				cost += Distance(x,S[j],ES,D) + GetCost(S[j],ES) + Distance(S[j],Edepot,ES,D) - Distance(x,Edepot,ES,D) ;
			}
			// if (load <= Q) {
			if (V[i-1] + cost < V[j]) {
				V[j] = V[i-1]+ cost;
				P[j] = i-1;
			}
			x = S[j];
			++j;
			// } else {
			// 	break;
			// }
			// x = u;
			// y = v;
			// cout<<V[i-1]<<","<<j<<" ";
		}
		// cout<< endl;
	}
	j = P[len];
	// for(int pp=0;pp<len;pp++) std::cout<<V[pp]<<" ";
	AddElement(S,0,len+1);
	if(j != 0){
		while(j != 0){
			//	std::cout<<j<<" j ";
			AddElement(S,0,j+1);
			j = P[j];
		}
	}else{
		std::cout<<"UlusoySplit maybe error j ="<<j<<" P[j] " <<P[j]<<std::endl;
		//	for(int pp=0;pp<len;pp++) std::cout<<P[pp]<<" ";
		//	std::cout<<std::endl;
		return 0;
	}
	AddElement(S,0,1);
	CopyP(Ps,S);
	return 1;
}

// //template<typename PATH,typename EMAP, typename DMatrix>
// bool UlusoySplit(PATH & Ps, EMAP & ES , DMatrix &D, DType& Q ) {
//     // split to routes from (e1 e2 e3) think about the end, our is (ei),
//     // define the distance of e1,e2 is e1.end e2.end.  	int len = P[0];
// 	int len = Ps[0];
// //	int* S = new int[2*len];	
// 	int S[2*len];
// 	Decode(S,Ps);
// 	DeleteAllZero(S);
// 	// InsertElement(S,0,S[0]+1);
// 	len = S[0];	
// 	DType load;
// 	VType cost;
// 	int t = len;
// 	int i,j;
// 	int u,v,x,y;
// //	int* V = new int[len];
// 	static int V[Max_Path_len];
// 	for(int pp=0;pp<=len;pp++) {
// 		V[pp] = INF;
// 	}
// //	int* P = new int[len];

// 	static int P[Max_Path_len];

// 	V[0] = 0;
// 	P[0] = 0;
// 	for(i = 1;i<=t;i++){
// 		load = 0;
// 		cost = 0;
// 		j = i;
// 		while( load<=Q&&j<=t ){
// 			if(S[j] != 0){
// 				if (GetDemand(S[j],ES) > Q) {
// 					cout<<" maybe not right length of ES ";
// 					cout<<GetDemand(S[j],ES)<<" >Q ";
// 					cout<<S[j]<<","<<GetTail(S[j],ES)<<","<<GetHead(S[j],ES)<<","<<GetDemand(S[j],ES)<<","<<GetCost(S[j],ES);
// 					exit(0);
// 				}
// 				load += GetDemand(S[j],ES);
// 			}
// 			// cout<< j<< ","<<load<< ","<<GetDemand(S[j],ES)<<" ";
// 			// u = GetTail(S[j],ES);
// 			// v = GetHead(S[j],ES);
// 			if(i==j){
// 			//	cost += Distance(Edepot,S[j],ES,D) + GetCost(S[j],ES) + D[v][0];
// 				cost += Distance(Edepot,S[j],ES,D) + GetCost(S[j],ES) + Distance(S[j],Edepot,ES,D);
// 			}else{
// 		//		cost += D[y][u] + GetCost(S[j],ES) + D[v][0] - D[y][0] ;
// 				cost += Distance(x,S[j],ES,D) + GetCost(S[j],ES) + Distance(S[j],Edepot,ES,D) - Distance(x,Edepot,ES,D) ;
// 			}
// 			if (load <= Q) {
// 				// if (V[j]==INF && V[i-1] + cost >= V[j]) {
// 				// 	cout<<"#"<<Distance(Edepot,S[i-1],ES,D) + GetCost(S[i-1],ES) + Distance(S[i-1],Edepot,ES,D);
// 				// 	cout<<" "<<V[i-1] <<" "<< cost<<" Big than INF\n";
// 				// 	cout<<S[j]<<","<<GetTail(S[j],ES)<<","<<GetHead(S[j],ES)<<","<<GetDemand(S[j],ES)<<","<<GetCost(S[j],ES);
// 				// 	exit(0);
// 				// }
// 				if (V[i-1] + cost < V[j]) {
// 					V[j] = V[i-1]+ cost;
// 					P[j] = i-1;
// 				}
// 				++j;
// 			} else {
// 				break;
// 			}
// 			// x = u;
// 			// y = v;
// 			// cout<<V[i-1]<<","<<j<<" ";
// 			x = S[j];
// 		}
// 		// cout<< endl;
// 	}
// 	j = P[len];
// 	for(int pp=0;pp<len;pp++) std::cout<<V[pp]<<" ";
// 	AddElement(S,0,len+1);
// 	if(j != 0){
// 		while(j != 0){
// 		//	std::cout<<j<<" j ";
// 			AddElement(S,0,j+1);
// 			j = P[j];
// 		}
// 	}else{
// 		std::cout<<"UlusoySplit maybe error j ="<<j<<" P[j] " <<P[j]<<std::endl;
// 	//	for(int pp=0;pp<len;pp++) std::cout<<P[pp]<<" ";
// 	//	std::cout<<std::endl;
// 		return 0;
// 	}
// 	AddElement(S,0,1);
// 	CopyP(Ps,S);
// 	return 1;
// }

// template<typename PATH,typename EMAP, typename DMatrix>
// bool UlusoySplit(PATH & Ps, EMAP & ES , DMatrix &D, DType& Q ) {
template<typename PATH,typename EMAP, typename DMatrix>
double SplitForMS(PATH& giantRoute, PATH& splitSeq, EMAP& tArray, DMatrix& d, int capacity, int depot) {
	int arrayP[Max_Path_len];
	double arrayV[Max_Path_len];
	arrayP[0] = 0;
	arrayV[0] = 0;
	int x;
	for (int i=1; i<=giantRoute[0]; i++)
	{
		arrayV[i] = INF;
	}
	for (int i = 1; i <= giantRoute[0]; i++)
	{
		double load = 0;
		double cost = 0.0;
		int j = i;
		while (j <= giantRoute[0] && load <= capacity)
		{
			load += tArray[giantRoute[j]].demand;
			// if (j == i)
			// {
			// 	cost = d[depot][tArray[giantRoute[j]].tBegin] + tArray[giantRoute[j]].tCost
			// 		+ d[tArray[giantRoute[j]].tEnd][depot];
			// }
			// else
			// {
			// 	cost += d[tArray[giantRoute[j-1]].tEnd][tArray[giantRoute[j]].tBegin]
			// 	+ tArray[giantRoute[j]].tCost + d[tArray[giantRoute[j]].tEnd][depot]
			// 	-d[tArray[giantRoute[j-1]].tEnd][depot];
			// }
			if(j==i){
				// cost += Distance(Edepot,S[j],ES,D) + GetCost(S[j],ES) + Distance(S[j],Edepot,ES,D);
				cost = Distance(Edepot,giantRoute[j],ES,D) + GetCost(giantRoute[j],ES) + Distance(giantRoute[j],Edepot,ES,D);
			}else{
				// cost += Distance(x,S[j],ES,D) + GetCost(S[j],ES) + Distance(S[j],Edepot,ES,D) - Distance(x,Edepot,ES,D) ;
				cost += Distance(x,giantRoute[j],ES,D) + GetCost(giantRoute[j],ES) + Distance(giantRoute[j],Edepot,ES,D) - Distance(x,Edepot,ES,D) ;
			}

			if (load <= capacity)
			{
				double V_new = arrayV[i-1] + cost;

				if (V_new < arrayV[j])
				{
					arrayV[j] = V_new;
					arrayP[j] = i-1;
				}
				x = giantRoute[j];
				j ++;
			}
		}
	}
	splitSeq[0] = 1;
	splitSeq[1] = 0;
// 	routes[0][0] = 1;
// 	routes[1][0] = 1;
// 	routes[1][1] = 0;
	int j = giantRoute[0];
	int ptr = arrayP[j];
	while (ptr>0)
	{
		for (int k=ptr+1; k<=j; k++)
		{
// 			routes[routes[0][0]][0] ++;
// 			routes[routes[0][0]][routes[routes[0][0]][0]] = giantRoute[k];

			splitSeq[0] ++;
			splitSeq[splitSeq[0]] = giantRoute[k];
		}
		splitSeq[0] ++;
		splitSeq[splitSeq[0]] = 0;
// 		routes[routes[0][0]][0] ++;
// 		routes[routes[0][0]][routes[routes[0][0]][0]] = 0;
// 		routes[0][0] ++;
// 		routes[routes[0][0]][0] = 1;
// 		routes[routes[0][0]][routes[routes[0][0]][0]] = 0;
		j = ptr;
		ptr = arrayP[j];
	}
	for (int k = 1; k <= j; k++)
	{
		splitSeq[0] ++;
		splitSeq[splitSeq[0]] = giantRoute[k];
// 		routes[routes[0][0]][0] ++;
// 		routes[routes[0][0]][routes[routes[0][0]][0]] = giantRoute[k];
	}
// 	routes[routes[0][0]][0] ++;
// 	routes[routes[0][0]][routes[routes[0][0]][0]] = 0;
	splitSeq[0] ++;
	splitSeq[splitSeq[0]] = 0;
	double opt_cost = arrayV[giantRoute[0]];
	return opt_cost;
}

void EUH(PATH& P,EMAP &ES,DMatrix& D,int task_num,DType& Q){
	PATH PS[5];
	VType Cost[5],minV=INF;
	int sel;
	PATH freeL;
	int inf = INF;
	for (int i = 4; i >0;i--) {
		for(int j=task_num;j>=0;j--){
			freeL[j] = j;
		}
		freeL[0] = task_num;
		path_scanning(PS[i],ES,D,task_num,inf,i+1,freeL);
		UlusoySplit(PS[i],ES,D,Q);
		Cost[i] = ValueCal(PS[i],ES,D);	
		if (Cost[i]<minV) {
			sel = i;
			minV = Cost[i];
		}
	}
	CopyP(P,PS[sel]);
}

//template<typename IndividualModel,typename EMAP, typename DMatrix>
void RandScanningInit(PATH& PS, EMAP& ES,DMatrix& D, int ES_NUM, DType& Q)
{
	int i, j, k;
	int load, trial, mindist;
	int UnservedArc[ES_NUM+1];
	int CandTask[ES_NUM+1];
	int MinTask[ES_NUM+1];
	int CurrentTask;
	int NextTask;
	int P[ES_NUM*2];
	int position;
	int v,cu;
	P[0] = 1;
	P[1] = 0;
	UnservedArc[0] = 0;
	for(i = 1; i <= ES_NUM; i++)
	{
		UnservedArc[0] ++;
		UnservedArc[UnservedArc[0]] = i;
	}
	load = 0;
	trial = 0;
	while (UnservedArc[0]>0)
	{		
		CurrentTask = P[ P[0] ];
		CandTask[0] = 0;
		MinTask[0] = 0;
		for (i = 1; i <= UnservedArc[0]; i++)
		{
			if ( GetDemand(UnservedArc[i],ES) + load <= Q)
			{
				CandTask[0] ++;
				CandTask[CandTask[0]] = UnservedArc[i];
			}
		}
		if(CandTask[0] == 0)
		{
			P[0]++;
			P[ P[0] ] = 0;
			load = 0;
			continue;
		}
		mindist = INF;
//		v = GetHead(CurrentTask,ES);
		for (i = 1; i <= CandTask[0]; i++)
		{
//			cu = GetTail(CandTask[i],ES);
			if(Distance(CurrentTask,CandTask[i],ES,D) < mindist)
			{
				mindist = Distance(CurrentTask,CandTask[i],ES,D);
				MinTask[0] = 1;
				MinTask[1] = CandTask[i];
				NextTask =  CandTask[i];
			}
			else if ( Distance(CurrentTask,CandTask[i],ES,D) == mindist)
			{
				int rule = rand()%5+1;
				if(betteru(CandTask[i],NextTask,rule,load,ES,D,Q) ){
					NextTask =  CandTask[i];
					MinTask[0] ++;
					MinTask[MinTask[0]] = CandTask[i];
				}
			}
		}
	//	cout<<CurrentTask;	
	//	Trail(MinTask);
	//	exit(0);	
//		k = RandChoose(MinTask[0])+1;
//		NextTask = MinTask[k];
		AddElement(P,NextTask,P[0]+1);
		load += GetDemand(NextTask,ES);
		position = FindElement(UnservedArc, NextTask);
		if(position) DeleteElement(UnservedArc, position);
		if ( ReverseElement(NextTask) > 0)
		{
			position = FindElement(UnservedArc, ReverseElement(NextTask) );
			if(position) DeleteElement(UnservedArc, position);
		}
	}
	AddElement(P,0,P[0]+1);
	CopyP(PS,P);
}

//template<typename IndividualModel,typename EMAP, typename DMatrix>
void RandScanningInit_repeat(PATH& PS, EMAP& ES,DMatrix& D, int ES_NUM, DType& Q,int repet_num) {
	PATH Pl;
	VType s=INF,l;
	for (int i=0;i<repet_num;i++) {
		RandScanningInit(Pl,ES,D,ES_NUM,Q);
		l = ValueCal(Pl,ES,D);
		//cout<<l<<" ";
		if (l<s) {
			s = l;
			CopyP(PS,Pl);
		}
	}
}

void RandScanningInit(IndividualModel& I, EMAP& ES,DMatrix& D, int ES_NUM, DType& Q) {
	RandScanningInit(I.P,ES,D,ES_NUM,Q);
}

//template<typename IndividualModel,typename EMAP, typename DMatrix>
void RandScanningInit(PATH& PS, EMAP& ES,DMatrix& D, int ES_NUM, DType& Q,int UnservedArc[]) {
	int i, j, k;
	int load, trial, mindist;
	//int UnservedArc[ES_NUM+1];
	int CandTask[ES_NUM+1];
	int MinTask[ES_NUM+1];
	int CurrentTask;
	int NextTask;
	int P[ES_NUM*2];
	int position;
	int v,cu;
	P[0] = 1;
	P[1] = 0;
	/*
	UnservedArc[0] = 0;
	for(i = 1; i <= ES_NUM; i++)
	{
		UnservedArc[0] ++;
		UnservedArc[UnservedArc[0]] = i;
	}
	*/
	load = 0;
	trial = 0;
	while (UnservedArc[0]>0)
	{		
		CurrentTask = P[ P[0] ];
		CandTask[0] = 0;
		MinTask[0] = 0;
		for (i = 1; i <= UnservedArc[0]; i++)
		{
			if ( GetDemand(UnservedArc[i],ES) + load <= Q)
			{
				CandTask[0] ++;
				CandTask[CandTask[0]] = UnservedArc[i];
			}
		}
		if(CandTask[0] == 0)
		{
			P[0]++;
			P[ P[0] ] = 0;
			load = 0;
			continue;
		}
		mindist = INF;
//		v = GetHead(CurrentTask,ES);
		for (i = 1; i <= CandTask[0]; i++) {
//			cu = GetTail(CandTask[i],ES);
			if(Distance(CurrentTask,CandTask[i],ES,D) < mindist) {
		//		cout<<"& *";
				mindist = Distance(CurrentTask,CandTask[i],ES,D);
				MinTask[0] = 1;
				MinTask[1] = CandTask[i];
				NextTask =  CandTask[i];
			}
			else if ( Distance(CurrentTask,CandTask[i],ES,D) == mindist) {
				int rule = rand()%5+1;
		//		cout<<rule<<" *";
				if(betteru(CandTask[i],NextTask,rule,load,ES,D,Q) ){
					NextTask =  CandTask[i];
					MinTask[0] ++;
					MinTask[MinTask[0]] = CandTask[i];
				}
			}
		}
	//	cout<<CurrentTask;	
	//	Trail(MinTask);
	//	exit(0);	
//		k = RandChoose(MinTask[0])+1;
//		NextTask = MinTask[k];
		AddElement(P,NextTask,P[0]+1);
//		Trail(P);
		load += GetDemand(NextTask,ES);
		position = FindElement(UnservedArc, NextTask);
		if(position) DeleteElement(UnservedArc, position);
		if ( ReverseElement(NextTask) > 0)
		{
			position = FindElement(UnservedArc, ReverseElement(NextTask) );
			if(position) DeleteElement(UnservedArc, position);
		}
	}
	AddElement(P,0,P[0]+1);
//	Trail(P);
	CopyP(PS,P);
//	Trail(PS);
//	cout<<"^^\n";
}

//template<typename IndividualModel,typename EMAP, typename DMatrix>
void RandScanningLS_repeat(PATH& PS, EMAP& ES,DMatrix& D, int ES_NUM, DType& Q,int repet_num,int C[]) {
	PATH Pl;
	VType s=INF,l;
	PATH S;
	CopyP(S,C);
	for (int i=0;i<repet_num;i++) {
		CopyP(C,S);
		RandScanningInit(Pl,ES,D,ES_NUM,Q,C);
		l = ValueCal(Pl,ES,D);
//		cout<<l<<" ";
		if (l<s) {
			s = l;
			CopyP(PS,Pl);
		}
	}
//	Trail(PS);
}
#endif
