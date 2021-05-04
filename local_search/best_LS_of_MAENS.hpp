/*************************************************************************
 *  Name  : local_search.hpp
 *  Author: zmdsn
 *  Mail  : zmdsn@126.com 
 * Created Time: 2020年11月23日 星期一 14时29分11秒
 * 
 ************************************************************************/
#ifndef  __BestLocalSearch_MAENS__HPP__
#define  __BestLocalSearch_MAENS__HPP__
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
class BestLocalSearchMAENS:public LocalSearchBase{
  public:
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
    double best_costs;
    void set_best_costs(double costs) { best_costs=costs; }
    void init_lamda(){ 
      int C = I->problem->Capacity;
      lamda = best_costs/C*(1.0*best_costs/I->costs + 1.0*I->over_load/C+1); 
    }
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
          tmp_relocate.get_penatly_fitness(lamda); 
          if (tmp_relocate.fitness < fitness) {
            sign = 1;
            best_relocate = tmp_relocate;
            costs   = best_relocate.costs;
            fitness = best_relocate.fitness;
          }
          Dtmp_relocate.set(i,j);
          Dtmp_relocate.get_penatly_fitness(lamda);
          if (Dtmp_relocate.fitness < fitness) {
            sign = 2;
            best_Drelocate = Dtmp_relocate;
            costs   = best_Drelocate.costs;
            fitness = best_Drelocate.fitness;
          }
          SWAP_IMPROVE.set(i,j);
          SWAP_IMPROVE.get_penatly_fitness(lamda); 
          if (SWAP_IMPROVE.fitness<fitness) {
            sign = 3;
            best_swap = SWAP_IMPROVE;
            costs   = best_swap.costs;
            fitness = best_swap.fitness;
          }
          if (R==S) {
            I2OPT.set(i,j);
            I2OPT.get_penatly_fitness(lamda); 
            if (I2OPT.fitness<fitness) {
              sign = 4;
              best_I2OPT = I2OPT;
              costs   = best_I2OPT.costs;
              fitness = best_I2OPT.fitness;
            }
    /*      } else {
            B2OPT.set(i,j);
            B2OPT.get_penatly_fitness(lamda); 
            if (B2OPT.fitness<fitness) {
              sign = 5;
              best_B2OPT= B2OPT;
              costs   = best_B2OPT.costs;
              fitness = best_B2OPT.fitness;
            }*/
          }
        }
      }
    }
    void find_wanted_move_of_all_routes() {
      improved = 0;
      I->route.push_back(0);
      I->split_to_subroute();
      // wanted_move->costs = INF;
      for (auto &R: I->sub_route) {
        for (auto &S: I->sub_route) {
          find_wanted_move_of_two_routes(&R,&S);
          // if( costs<0 ) return;
        }
      }
    }
    inline DType get_over_load() {
      // return wanted_move.over_load;
	//return fitness;
      switch(sign) {
        case 1:
          return best_relocate.M.over_load;
          break;
        case 2:
          return best_Drelocate.M.over_load;
          break;
        case 3:
          return best_swap.M.over_load;
          break;
        case 4:
          return best_I2OPT.M.over_load;
          break;
        case 5:
          return best_B2OPT.M.over_load;
          break;
      }
      return 0;
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
 	    return fitness;
      /*switch(sign) {
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
      }*/
    }
    BestLocalSearchMAENS(CARP_INDIVIDUAL* indi)
      : LocalSearchBase(indi) {
        // wanted_move = new M(indi);
        // temp_move   = new M(indi);  
    };
    // BestLocalSearchMAENS(CARP_INDIVIDUAL* indi, 
    //     Move_base* _wanted_move, Move_base* _move) :
    //   LocalSearchBase(indi,_wanted_move,_move) {
    // }
    ~BestLocalSearchMAENS(){};
};

#endif
