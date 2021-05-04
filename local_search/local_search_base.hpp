/*************************************************************************
 *  Name  : local_search.hpp
 *  Author: zmdsn
 *  Mail  : zmdsn@126.com 
 * Created Time: 2020年11月23日 星期一 14时29分11秒
 * 
 ************************************************************************/
#ifndef  __LocalSearchBase__HPP__
#define  __LocalSearchBase__HPP__
#include <sstream>
#include <iostream>
#include <string>
#include <ostream>
#include "../bin/general.hpp"
#include "../bin/CARP_individual.hpp"
#include "../move/move.hpp"
#include "../move/relocate.hpp"
/*#include "../move/double_relocate.hpp"
#include "../move/swap_move.hpp"
#include "../move/exchange.hpp"
#include "../move/inner_2opt.hpp"
#include "../move/between_2opt.hpp"
#include "../move/reverse.hpp"
*/
using namespace std;

class LocalSearchBase{
  public:
    clock_t start_time,end_time;
    VType improved;
    VType total_improved;
    VType one_improved;
    VType old_costs;
    VType new_costs;
    VType costs;
    double fitness;
    // get_over_load();
    // move();
    DType old_over_load;
    DType new_over_load;
    DType over_load_changed;
    // M* wanted_move;
    // M* wanted_move;
    // M* temp_move;
    int improved_times;
    CARP_INDIVIDUAL* I;
    bool record = 0;
    int record_i,record_j,record_k;
    double lamda;
    inline void set_lamda(double lm) {lamda = lm;}
    LocalSearchBase(CARP_INDIVIDUAL* indi) {
      I = indi;
    }
    // inline DType get_over_load() {
    //   // return wanted_move->over_load;
    //   return wanted_move->get_over_load();
    // }
    // inline void move() {
    //   wanted_move->move();
    // }
    // inline DType get_fitness() {
    //   // return wanted_move->over_load;
    //   return wanted_move->fitness;
    // }
    LocalSearchBase(){};
    ~LocalSearchBase(){};
};

/*
template<class M>
std::ostream &  operator<<(std::ostream & out,LocalSearchBase<M> *LS) {
  out<<"";
  //out<<"old,new,improved : "<<LS->old_costs<<" "<<LS->new_costs<<" "<<LS->improved<<endl;
  return out;
}

template<class M>
std::ostream &  operator<<(std::ostream & out,LocalSearchBase<M> &LS) {
  // out<<"******* move ********"<<endl;
  // show(m.R1->route);
  // show(m.R2->route);
  //out<<"old,improved,new: "<<LS.old_costs<<" "<<LS.wanted_move->costs<<" "<<LS.old_costs+LS.improved<<"\t over load "<<LS.wanted_move->over_load<<endl;
  //out<<"position from "<<LS.wanted_move->position1<<" to "<<LS.wanted_move->position2<<endl;
  // out<<"old,new,improved : "<<LS.old_costs<<" "<<LS.new_costs<<" "<<LS.improved<<endl;
  // out<<"position from "<<m.position1<<" to "<<m.position2<<endl;
  // out<<"task "<<m.task_i<<"  "<<m.task_j<<endl;
  // out<<"over_load: "<<m.over_load<<endl;
  // out<<"fitness: "<<m.fitness<<endl;
  // out<<"change_mode: "<<m.change_mode<<endl;
  // out<<"*********************"<<endl;
  out<<"";
  return out;
}
*/

#endif
