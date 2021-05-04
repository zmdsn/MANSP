/*************************************************************************
 *  Name  : local_search.hpp
 *  Author: zmdsn
 *  Mail  : zmdsn@126.com 
 * Created Time: 2020年11月23日 星期一 14时29分11秒
 * 
 ************************************************************************/
#ifndef  __BestFeasibleLocalSearch__HPP__
#define  __BestFeasibleLocalSearch__HPP__
#include <sstream>
#include <iostream>
#include <string>
#include <ostream>
#include "../local_search/local_search.hpp"
using namespace std;

template<class M>
class BestFeasibleLocalSearch:public LocalSearchBase{
  public:
    M* wanted_move;
    M* temp_move;
    void find_wanted_move_of_two_routes(
      CARP_INDIVIDUAL* R,CARP_INDIVIDUAL* S){
      temp_move->set(R,S);
      for (int pos1=0; pos1<R->route.size(); pos1++) {
        for (int pos2=0; pos2<S->route.size(); pos2++) {
          temp_move->set(pos1,pos2);
          temp_move->get_move_costs();
          if(temp_move->check_move_feasible()){
            // if(*temp_move<*wanted_move) {
            if(temp_move->costs<costs) {
              *wanted_move = *temp_move;
              costs = wanted_move->costs;
            }
          }
        }
      }
    }
    void find_wanted_move_of_all_routes(){
      // improved = 0;
      //I->split_to_subroute();
      wanted_move->costs = INF;
      for (auto &R: I->sub_route) {
        for (auto &S: I->sub_route) {
          find_wanted_move_of_two_routes(&R,&S);
          // costs = wanted_move->costs;
        }
      }
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
    BestFeasibleLocalSearch(CARP_INDIVIDUAL* indi):
      LocalSearchBase(indi) {
        wanted_move = new M(indi);
        temp_move   = new M(indi);  
    };
    BestFeasibleLocalSearch(CARP_INDIVIDUAL* indi, M* _wanted_move, M* _move):
      LocalSearchBase(indi,_wanted_move,_move){
    }
    ~BestFeasibleLocalSearch(){};
};
#endif
