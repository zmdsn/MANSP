/*************************************************************************
 *  Name  : local_search.hpp
 *  Author: zmdsn
 *  Mail  : zmdsn@126.com 
 * Created Time: 2020年11月23日 星期一 14时29分11秒
 * 
 ************************************************************************/
#ifndef  __BestPenaltyLocalSearch__HPP__
#define  __BestPenaltyLocalSearch__HPP__
#include <sstream>
#include <iostream>
#include <string>
#include <ostream>
#include "../local_search/local_search.hpp"
using namespace std;
template<class M>
class BestPenaltyLocalSearch:public LocalSearchBase{
  public:
    double lamda;
    void set_lamda(double lm) {lamda = lm;}
    void find_wanted_move_of_two_routes(CARP_INDIVIDUAL* R
        ,CARP_INDIVIDUAL* S){
      I->split_to_subroute();
      for (int pos1=0; pos1<R->route.size(); pos1++) {
        for (int pos2=0; pos2<S->route.size(); pos2++) {
          temp_move->set(R,S,pos1,pos2);
          // temp_move->get_move_costs();
          temp_move->get_fitness(lamda);
          // cout<<move->costs<<"->"<<wanted_move->costs<<","<<(*temp_move<*wanted_move)<<endl;
          if(temp_move->fitness<wanted_move->fitness) {
            *wanted_move = *temp_move;
            if( wanted_move->costs<0 ) return;
          }
        }
      }
    }
    void find_wanted_move_of_all_routes(){
      improved = 0;
      I->split_to_subroute();
      wanted_move->costs = INF;
      for (auto &R: I->sub_route) {
        for (auto &S: I->sub_route) {
          find_wanted_move_of_two_routes(&R,&S);
          if( wanted_move->costs<0 ) return;
        }
      }
    }
    BestPenaltyLocalSearch(CARP_INDIVIDUAL* indi):
      LocalSearch(indi){
        wanted_move = new M(indi);
        temp_move   = new M(indi);  
        // set_lamda(0.1);
    };
    BestPenaltyLocalSearch(CARP_INDIVIDUAL* indi, M* _wanted_move,
      M* _move):LocalSearch(indi,_wanted_move,_move){

    }
    ~BestPenaltyLocalSearch(){};
};
#endif
