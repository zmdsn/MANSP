/*************************************************************************
 *  Name  : local_search.hpp
 *  Author: zmdsn
 *  Mail  : zmdsn@126.com 
 * Created Time: 2020年11月23日 星期一 14时29分11秒
 * 
 ************************************************************************/
#ifndef  __FirstLocalSearch__HPP__
#define  __FirstLocalSearch__HPP__
#include <sstream>
#include <iostream>
#include <string>
#include <ostream>
#include "../local_search/local_search_base.hpp"
using namespace std;

template<class M>
class FirstLocalSearch : public LocalSearchBase{
  public:
    M* wanted_move;
    M* temp_move;
    void find_wanted_move_of_two_routes(CARP_INDIVIDUAL* R
        ,CARP_INDIVIDUAL* S){
      temp_move->set(R,S);
        //show(R->route);
        //show(S->route);
      for (int pos1=0; pos1<R->route.size(); pos1++) {
        for (int pos2=0; pos2<S->route.size(); pos2++) {
          temp_move->set(pos1,pos2);
          temp_move->get_move_costs();
          //cout<<temp_move->costs<<"->"<<wanted_move->costs<<","<<endl;
          if(temp_move->costs<costs) {
            //cout<<"ss"<<endl;
            //show(temp_move->M.R1->route);
            *wanted_move = *temp_move;
            costs = wanted_move->costs;
            //cout<<temp_move->costs<<"->"<<wanted_move->costs<<",,"<<endl;
            if( costs<0 ) {
            //show(temp_move->M.R1->route);
            //  cout<<temp_move->costs<<"->"<<wanted_move->costs<<","<<endl;
            //  exit(0);
              return;
            }
          }
        }
      }
    }
    void find_wanted_move_of_all_routes() {
      improved = 0;
      wanted_move->costs = INF;
      wanted_move->fitness = INF;
      int i = 0;
      int j = 0;
      i = record?record_i:0;
      for (;i<I->sub_route.size();i++) {
        j = record?record_j:0;
        for (;j<I->sub_route.size();j++) {
          find_wanted_move_of_two_routes(&I->sub_route[i],
              &I->sub_route[j]);
          if( costs<0 ) {
              record_i = i;
              record_j = j;
              record = 1;
            return;
          }
          record = 0;
        }
      }
      if (i==j&&i==I->sub_route.size()) record = 0;
    }
    inline DType get_over_load() {
      // return wanted_move->over_load;
      return wanted_move->get_over_load();
    }
    inline void move() {
      wanted_move->move();
    }
    inline DType get_fitness() {
      fitness = wanted_move->costs;
      return fitness;
    }
    FirstLocalSearch(CARP_INDIVIDUAL* indi):
      LocalSearchBase(indi){
      wanted_move = new M(indi);
      temp_move   = new M(indi);
    };
    FirstLocalSearch(CARP_INDIVIDUAL* indi, M* _wanted_move,
      M* _move):LocalSearchBase(indi,_wanted_move,_move){
      wanted_move = new M(indi);
      temp_move   = new M(indi);
    }
    FirstLocalSearch(){};    
    ~FirstLocalSearch(){};
};
#endif
