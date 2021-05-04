/*************************************************************************
 *  Name  : local_search.hpp
 *  Author: zmdsn
 *  Mail  : zmdsn@126.com 
 * Created Time: 2020年11月23日 星期一 14时29分11秒
 * 
 ************************************************************************/
#ifndef  __LSMALS__HPP__
#define  __LSMALS__HPP__
#include <sstream>
#include <iostream>
#include <string>
#include <ostream>
#include "../local_search/local_search.hpp"
using namespace std;
#define _SWAP MOVE<SWAP_MOVE>
#define _DRELOCATE MOVE<DOUBLE_RELOCATE>
#define _RELOCATE MOVE<Relocate>
#define _O2OPT MOVE<BETWEEN_2OPT>
#define _I2OPT MOVE<INNER_2OPT>
#define _InvRELOCATE MOVE<InvRelocate>



//template<class M>
class LSMALS:public LocalSearchBase{
  public:
    _RELOCATE tmp_relocate; 
    _RELOCATE best_relocate; 
    _DRELOCATE Dtmp_relocate;  
    _DRELOCATE best_Drelocate;  
    _SWAP SWAP_IMPROVE;
    _SWAP best_swap;
    _O2OPT B2OPT;
    _O2OPT best_B2OPT;
    _I2OPT I2OPT;
    _I2OPT best_I2OPT;
    _InvRELOCATE tmp_inv_relocate;
    _InvRELOCATE best_inv_relocate;
    int sign;
    double best_costs;
    void find_wanted_move_of_two_routes(
        CARP_INDIVIDUAL* R,CARP_INDIVIDUAL* S){
      // temp_move->set(R,S);
      for (int i = 0; i < R->route.size(); ++i) {
        if (sign==5)  tmp_inv_relocate.set(R,S);
        for (int j = 0; j < S->route.size(); ++j) {
          // cout<<"sign"<<sign<<endl;
          switch (sign) {
            case 0: 
              tmp_relocate.set(R,S,i,j);
              tmp_relocate.get_move_costs();
              if(tmp_relocate.check_move_feasible()){
                if(tmp_relocate.costs<costs) {
                  fitness = tmp_relocate.costs;
                  costs = tmp_relocate.costs;
                  best_relocate = tmp_relocate;
                }
              }
              break;
            case 1:
              Dtmp_relocate.set(R,S,i,j);
              Dtmp_relocate.get_move_costs();
              if(Dtmp_relocate.check_move_feasible()){
                if(Dtmp_relocate.costs<costs) {
                  fitness = Dtmp_relocate.costs;
                  costs = Dtmp_relocate.costs;
                  best_Drelocate = Dtmp_relocate;
                  // cout<<"success "<< costs<<endl;
                }
              }
              break;
            case 2:
              SWAP_IMPROVE.set(R,S,i,j);
              SWAP_IMPROVE.get_move_costs();
              if(SWAP_IMPROVE.check_move_feasible()){
                if(SWAP_IMPROVE.costs<costs) {
                  fitness = SWAP_IMPROVE.costs;
                  costs = SWAP_IMPROVE.costs;
                  best_swap = SWAP_IMPROVE;
                }
              }
              // SWAP_IMPROVE.set(R,S,i,j);
              // SWAP_IMPROVE.get_move_costs();
              // fitness = SWAP_IMPROVE.costs;
              break;
            case 3:
              B2OPT.set(R,S,i,j);
              B2OPT.get_move_costs();
              if(B2OPT.check_move_feasible()){
                if(B2OPT.costs<costs) {
                  fitness = B2OPT.costs;
                  costs = B2OPT.costs;
                  best_B2OPT = B2OPT;
                }
              }
              // B2OPT.set(R,S,i,j);
              // B2OPT.get_move_costs();
              // fitness = B2OPT.costs;
              break;
            case 4:
              I2OPT.set(R,S,i,j);
              I2OPT.get_move_costs();
              if(I2OPT.check_move_feasible()){
                if(I2OPT.costs<costs) {
                  fitness = I2OPT.costs;
                  costs = I2OPT.costs;
                  best_I2OPT = I2OPT;
                }
              }
              // I2OPT.set(R,S,i,j);
              // I2OPT.get_move_costs();
              // fitness = I2OPT.costs;
              break;
            case 5:
              tmp_inv_relocate.set(i,j);
              tmp_inv_relocate.get_move_costs();
              if(tmp_inv_relocate.check_move_feasible()){
                if(tmp_inv_relocate.costs<costs) {
                  fitness = tmp_inv_relocate.costs;
                  costs = tmp_inv_relocate.costs;
                  best_inv_relocate = tmp_inv_relocate;
                }
              }
              // tmp_inv_relocate.set(R,S,i,j);
              // tmp_inv_relocate.get_move_costs();
              // fitness = tmp_inv_relocate.costs;
              break;
          }
        }
      }
    }
    void find_wanted_move_of_all_routes() {
      improved = 0;
      I->route.push_back(0);
      I->split_to_subroute();
      costs = INF;
      for (auto &R: I->sub_route) {
        for (auto &S: I->sub_route) {
          sign = rand()%5;
          // sign = 0; 
          find_wanted_move_of_two_routes(&R,&S);
          if( costs<0 ) return;
        }
      }
    }
    inline DType get_over_load() {
      switch (sign) {
        case 0: 
          return tmp_relocate.get_over_load();
          break;
        case 1:
          return Dtmp_relocate.get_over_load();
          break;
        case 2:
          return SWAP_IMPROVE.get_over_load();
          break;
        case 3:
          return B2OPT.get_over_load();
          break;
        case 4:
          return I2OPT.get_over_load();
          break;
        case 5:
          return tmp_inv_relocate.get_over_load();
          break;
      }
      return 0;
    }
    inline void move() {
      switch (sign) {
        case 0: 
          // cout<<"moved";
          best_relocate.move();
          break;
        case 1:
          best_Drelocate.move();
          break;
        case 2:
          best_swap.move();
          break;
        case 3:
          best_B2OPT.move();
          break;
        case 4:
          best_I2OPT.move();
          break;
        case 5:
          best_inv_relocate.move();
          break;
      }
    }
    inline DType get_fitness() {
      // fitness = wanted_move->costs;
      // switch (sign) {
      //   case 0: 
      //     fitness = tmp_relocate.costs;
      //     break;
      //   case 1:
      //     fitness = Dtmp_relocate.costs;
      //     break;
      //   case 2:
      //     fitness = SWAP_IMPROVE.costs;
      //     break;
      //   case 3:
      //     fitness = B2OPT.costs;
      //     break;
      //   case 4:
      //     fitness = I2OPT.costs;
      //     break;
      //   case 5:
      //     fitness = tmp_inv_relocate.costs;
      //     break;
      // }
      return fitness;
    }
    LSMALS(CARP_INDIVIDUAL* indi):LocalSearchBase(indi){
        //wanted_move = new M(indi);
       // temp_move   = new M(indi);  
    };
    // LSMALS(CARP_INDIVIDUAL* indi,    M* _wanted_move,
    //   M* _move):LocalSearchBase(indi,_wanted_move,_move){
    // }
    ~LSMALS(){};
};
#endif
