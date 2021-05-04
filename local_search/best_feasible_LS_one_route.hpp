/*************************************************************************
 *  Name  : local_search.hpp
 *  Author: zmdsn
 *  Mail  : zmdsn@126.com 
 * Created Time: 2020年11月23日 星期一 14时29分11秒
 * 
 ************************************************************************/
#ifndef  _ONE_ROUTE__BEST_FLS__HPP__
#define  _ONE_ROUTE__BEST_FLS__HPP__
#include <sstream>
#include <iostream>
#include <string>
#include <ostream>
#include "../local_search/local_search.hpp"
using namespace std;
// 
template<class M>
class BFLSS:public LocalSearchBase{
  public:
    M* wanted_move;
    M* temp_move;
    void find_wanted_move_of_two_routes(CARP_INDIVIDUAL* R
        ,CARP_INDIVIDUAL* S){
      temp_move->set(R,S);
      for (int pos1=0; pos1<R->route.size(); pos1++) {
        for (int pos2=0; pos2<S->route.size(); pos2++) {
          temp_move->set(pos1,pos2);
          temp_move->get_move_costs();
          if(temp_move->check_move_feasible()){
            if(*temp_move<*wanted_move) {
              *wanted_move = *temp_move;
            }
          }
        }
      }
      I->merge_subroute();
    }
    void find_wanted_move_of_all_routes(){
      improved = 0;
      I->split_to_subroute();
      wanted_move->costs = INF;
      for (auto &R: I->sub_route) {
        find_wanted_move_of_two_routes(&R,&R);
        I->merge_subroute();
      }
    }
    BFLSS(CARP_INDIVIDUAL* indi):LocalSearchBase(indi){
        wanted_move = new M(indi);
        temp_move   = new M(indi);  
    };
    BFLSS(CARP_INDIVIDUAL* indi, M* _wanted_move, M* _move):
      LocalSearchBase(indi,_wanted_move,_move){
    }
    ~BFLSS(){};
};
#endif
