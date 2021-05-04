/*************************************************************************
 *  Name  : local_search.hpp
 *  Author: zmdsn
 *  Mail  : zmdsn@126.com 
 * Created Time: 2020年11月23日 星期一 14时29分11秒
 * 
 ************************************************************************/
#ifndef  __BestLocalSearchPenalty__HPP__
#define  __BestLocalSearchPenalty__HPP__
#include <sstream>
#include <iostream>
#include <string>
#include <ostream>
#include "../local_search/local_search.hpp"
using namespace std;

template<class M>
class BestLocalSearchPenalty:public LocalSearchBase{
  public:
    M* wanted_move;
    M* temp_move;
    double lamda;
    void set_lamda(double lm) {lamda = lm;}
    void find_wanted_move_of_two_routes(CARP_INDIVIDUAL* R
        ,CARP_INDIVIDUAL* S){
      temp_move->set(R,S);
      for (int pos1=0; pos1<R->route.size(); pos1++) {
        for (int pos2=0; pos2<S->route.size(); pos2++) {
          temp_move->set(pos1,pos2);
          //temp_move->get_move_costs();
          // cout<<temp_move->costs<<"->"<<wanted_move->costs<<","<<(*temp_move<*wanted_move)<<endl;
          temp_move->get_fitness(lamda);
          if(temp_move->fitness<wanted_move->fitness) {
            //cout<<temp_move->fitness<<"->"<<wanted_move->fitness<<endl;
            *wanted_move = *temp_move;
          }
        }
      }
      I->merge_subroute();
    }
    void find_wanted_move_of_all_routes(){
      I->split_to_subroute();
      wanted_move->costs = INF;      
      wanted_move->fitness = INF;
      for (auto &R: I->sub_route) {
        for (auto &S: I->sub_route) {
          find_wanted_move_of_two_routes(&R,&S);
        }
      }
    }
    VType get_fitness() {
      find_wanted_move_of_all_routes();
      return wanted_move->fitness;
    }
    BestLocalSearchPenalty(CARP_INDIVIDUAL* indi):
      LocalSearchBase(indi){
      wanted_move = new M(indi);
      temp_move   = new M(indi);  
      set_lamda(0.1);
    };
    BestLocalSearchPenalty(CARP_INDIVIDUAL* indi,    M* _wanted_move,
      M* _move):LocalSearchBase(indi,_wanted_move,_move){
    }
    ~BestLocalSearchPenalty(){};
};
#endif
