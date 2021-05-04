/*************************************************************************
 *  Name  : local_search.hpp
 *  Author: zmdsn
 *  Mail  : zmdsn@126.com 
 * Created Time: 2020年11月23日 星期一 14时29分11秒
 * 
 ************************************************************************/
#ifndef  __FirstLocalSearch_MA__HPP__
#define  __FirstLocalSearch_MA__HPP__
#include <sstream>
#include <iostream>
#include <string>
#include <ostream>
#include "../local_search/local_search.hpp"
#include "../move/relocate.hpp"
#include "../move/swap_move.hpp"
#include "../move/double_relocate.hpp"
#include "../move/between_2opt.hpp"
#include "../move/inner_2opt.hpp"
#define _SWAP MOVE<SWAP_MOVE>
#define _DRELOCATE MOVE<DOUBLE_RELOCATE>
#define _RELOCATE MOVE<Relocate>
#define _O2OPT MOVE<BETWEEN_2OPT>
#define _I2OPT MOVE<INNER_2OPT>
using namespace std;

// template<class M>
class FirstLocalSearchMA:public LocalSearchBase{
  public:
    // auto wanted_move;
    // auto temp_move;
    // MOVE<M>* wanted_move;
    // MOVE<M>* temp_move;
    // REVERSE Reverse; 
    // _RELOCATE MOVE<RELOCATE>
    // _DRELOCATE MOVE<DOUBLE_RELOCATE>
    // _SWAP MOVE<SWAP_MOVE>
    // _O2OPT MOVE<BETWEEN_2OPT>
    // _I2OPT MOVE<INNER_2OPT>
    _RELOCATE tmp_relocate; 
    _DRELOCATE Dtmp_relocate;  
    _SWAP SWAP_IMPROVE;
    _O2OPT B2OPT;
    _I2OPT I2OPT;
    _RELOCATE best_relocate; 
    _DRELOCATE best_Drelocate;  
    _SWAP best_swap;
    _O2OPT best_B2OPT;
    _I2OPT best_I2OPT;
    int sign;
    void find_wanted_move_of_two_routes(CARP_INDIVIDUAL* R
        ,CARP_INDIVIDUAL* S){
      for (int i = 0; i < R->route.size(); ++i) {
        tmp_relocate.set(R,S);
        Dtmp_relocate.set(R,S);
        SWAP_IMPROVE.set(R,S);
        I2OPT.set(R,S);
        B2OPT.set(R,S);
        for (int j = 0; j < S->route.size(); ++j) {
          tmp_relocate.set(i,j);
          tmp_relocate.get_move_costs(); 
          // cout<<"relocate "<<tmp_relocate;
          if (tmp_relocate.check_move_feasible()
              && tmp_relocate.costs<0) {
            sign = 1;
            best_relocate = tmp_relocate;
            costs = best_relocate.costs;
            return;
          }
          Dtmp_relocate.set(i,j);
          Dtmp_relocate.get_move_costs(); 
          // cout<<"dd"<<Dtmp_relocate;
          if (Dtmp_relocate.check_move_feasible()&&Dtmp_relocate.costs<0) {
            sign = 2;
            best_Drelocate = Dtmp_relocate;
            costs = best_Drelocate.costs;
            return;
          }
          SWAP_IMPROVE.set(i,j);
          SWAP_IMPROVE.get_move_costs(); 
          if (SWAP_IMPROVE.check_move_feasible()&&SWAP_IMPROVE.costs<0) {
            sign = 3;
            best_swap = SWAP_IMPROVE;
            costs = best_swap.costs;
            return;
          }
          if (R==S) {
            I2OPT.set(i,j);
            I2OPT.get_move_costs(); 
            if (I2OPT.check_move_feasible()&&I2OPT.costs<0) {
              sign = 4;
              best_I2OPT = I2OPT;
              costs = best_I2OPT.costs;
              return;
            }
          } else {
            B2OPT.set(i,j);
            B2OPT.get_move_costs(); 
            if (B2OPT.check_move_feasible()&&B2OPT.costs<0) {
              sign = 5;
              best_B2OPT= B2OPT;
              costs = best_B2OPT.costs;
              return;
            }
          }
        }
      }
    }
    void find_wanted_move_of_all_routes(){
      improved = 0;
      I->route.push_back(0);
      I->split_to_subroute();
      I->get_over_load();
      // wanted_move->costs = INF;
      for (auto &R: I->sub_route) {
        for (auto &S: I->sub_route) {
          find_wanted_move_of_two_routes(&R,&S);
          if( costs<0 ) return;
        }
      }
    }
    inline DType get_over_load() {
      // return wanted_move.over_load;
      switch(sign) {
        case 1:
          return best_relocate.get_over_load();
          break;
        case 2:
          return best_Drelocate.get_over_load();
          break;
        case 3:
          return best_swap.get_over_load();
          break;
        case 4:
          return best_I2OPT.get_over_load();
          break;
        case 5:
          return best_B2OPT.get_over_load();
          break;
      }
    }
    inline void move() {
      switch(sign) {
        case 1:
          best_relocate.move();
          break;
        case 2:
          best_Drelocate.move();
          break;
        case 3:
          best_swap.move();
          break;
        case 4:
          best_I2OPT.move();
          break;
        case 5:
          best_B2OPT.move();
          break;
      }
    }
    inline DType get_fitness() {
      switch(sign) {
        case 1:
          return best_relocate.fitness;
          break;
        case 2:
          return best_Drelocate.fitness;
          break;
        case 3:
          return best_swap.fitness;
          break;
        case 4:
          return best_I2OPT.fitness;
          break;
        case 5:
          return best_B2OPT.fitness;
          break;
      }
    }
    FirstLocalSearchMA(CARP_INDIVIDUAL* indi)
      : LocalSearchBase(indi) {
        // wanted_move = new M(indi);
        // temp_move   = new M(indi);  
    };
    // FirstLocalSearchMA(CARP_INDIVIDUAL* indi, 
    //     Move_base* _wanted_move, Move_base* _move) :
    //   LocalSearchBase(indi,_wanted_move,_move) {
    // }
    ~FirstLocalSearchMA(){};
};

#endif
