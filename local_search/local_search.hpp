/*************************************************************************
 *  Name  : local_search.hpp
 *  Author: zmdsn
 *  Mail  : zmdsn@126.com 
 * Created Time: 2020年11月23日 星期一 14时29分11秒
 * 
 ************************************************************************/
#ifndef  __LocalSearch__HPP__
#define  __LocalSearch__HPP__
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

template<class L>
class LocalSearch{
  public:
    L* LS;
    CARP_INDIVIDUAL* I;
    clock_t start_time,end_time;
    VType improved;
    VType total_improved;
    VType one_improved;
    VType old_costs;
    VType new_costs;
    DType old_over_load;
    DType new_over_load;
    DType over_load_changed;
    int improved_times;
    inline void set_lamda(double lm) {LS->lamda = lm;}
    bool is_improved_right_over_load(int rule) {
      old_costs = I->get_costs();
      old_over_load = I->get_over_load();
      I->split_to_subroute();
      I->get_costs();
      I->get_over_load();
      switch(rule) {
        case 0:
          move_of_all_routes();
          break;
        case 1:
          move_of_each_two_routes();
          break;
        case 2:
          while_move_of_all_routes();
          break;
      }
      I->merge_subroute();
      new_costs = I->get_costs();
      new_over_load = I->get_over_load();
      // cout<<"old,new,over_load "<<old_over_load<<","<<new_over_load<<","<<over_load<<endl;
      if (new_over_load-old_over_load != over_load_changed) {
        cout<<"old,new,over_load "<<old_over_load<<","<<new_over_load<<","<<over_load_changed<<endl;
        exit(0);
        return 0;
      }
      return 1;
    }
    bool is_improved_right_costs(int rule) {
      old_costs = I->get_costs();
      old_over_load = I->get_over_load();
      I->split_to_subroute();
      I->get_costs();
      I->get_over_load();
      switch(rule) {
        case 0:
          move_of_all_routes();
          break;
        case 1:
          move_of_each_two_routes();
          break;
        case 2:
          while_move_of_all_routes();
          break;
      }
      I->merge_subroute();
      new_costs = I->get_costs();
      old_over_load = I->get_over_load();
      if (improved+old_costs != new_costs) {
        cout<<"old,new,improved "<<old_costs<<","<<new_costs<<","<<improved<<endl;
        exit(0);
        return 0;
      }
      return 1;
    }
    void move_of_two_routes(CARP_INDIVIDUAL* R
        ,CARP_INDIVIDUAL* S) {
      LS->fitness = INF;
      LS->find_wanted_move_of_two_routes(R,S);
      if(LS->fitness<0) {
        LS->get_over_load();
        over_load_changed+= LS->over_load_changed;
        improved += LS->fitness; 
        LS->move();
        I->merge_subroute();
      }
    }
    bool move_of_each_two_routes() {
      I->split_to_subroute();
      I->get_costs();
      I->get_over_load();
      improved = 0;
      over_load_changed = 0;
      for (auto &R: I->sub_route) {
        for (auto &S: I->sub_route) {
          move_of_two_routes(&R,&S);
        }
      }
      I->merge_subroute();
      return improved<0;
    }
    bool move() {
      improved = 0;
      if(LS->fitness<0) {
        LS->get_over_load();
        over_load_changed += LS->over_load_changed;
        LS->move();
        improved += LS->fitness; 
      }
      I->merge_subroute();
      return improved<0;
    }
    bool move_of_all_routes() {
      improved = 0;
      over_load_changed = 0;
      LS->find_wanted_move_of_all_routes();
      LS->get_fitness();
      // cout<<LS->costs<<","<<LS->fitness<<endl;
      // cout<<*LS->wanted_move;
      if(LS->fitness<0) {
        //show(LS->temp_move->M.R1->route);
				// show(LS->wanted_move->M.R1->route);
				// show(LS->wanted_move->M.R2->route);
        //cout<<"move_of_all_routes"<<endl;
        LS->get_over_load();
        over_load_changed += LS->over_load_changed;
        LS->move();
				// show(LS->wanted_move->M.R1->route);
				// show(LS->wanted_move->M.R2->route);
        improved += LS->fitness; 
      }
      I->merge_subroute();
      return improved<0;
    }
    bool while_move_of_all_routes() {
      int stop = 0;      
      improved = 0;
      total_improved = 0;
      over_load_changed = 0;
      DType temp_over_load = 0;
      I->split_to_subroute();
      I->get_costs();
      I->get_over_load();
			VType oldC,newC;
      while(1) {
        LS->costs = INF;
        LS->fitness = INF;
				improved = 0;
				oldC = I->costs;
        move_of_all_routes();
        // cout<<"ov "<<over_load<<endl;
        if(improved<0) {
					I->get_costs();
					// newC = I->costs;
					// if (oldC!=newC-improved) {
					//   cout<<oldC<<" => "<<newC<<" "<<improved<<endl;
					//   cout<<"error val"<<endl;
					//   exit(0);
					// }
          temp_over_load += over_load_changed;
          total_improved += improved;
          // record_i = 
          stop = 0;
        }
        stop += LS->fitness>=0;
				// cout<<improved<<endl;
				// cout<<LS->fitness;
				// cout<<"stop"<<stop<<endl;
        if(stop==2) {
          break;
        }
      }
      improved = total_improved;
      over_load_changed= temp_over_load;
      I->merge_subroute();
      I->get_fitness();
      return improved<0;
    }
    bool while_move_of_all_routes_best_fitness() {
      int stop = 0;      
      improved = 0;
      total_improved = 0;
      over_load_changed = 0;
      DType temp_over_load = 0;
      I->split_to_subroute();
      I->get_costs();
      I->get_fitness();
      I->get_over_load();
      VType oldC,newC;
      LS->init_lamda();
      LS->lamda = 0.2;
      int all_non_feasible = 0;
      int all_feasible = 0;
      bool is_overload;
      while(1) {
        LS->costs = INF;
        LS->fitness = INF;
        improved = 0;
        // oldC = I->costs;
        move_of_all_routes();
        // cout<<"ov "<<over_load<<endl;
        is_overload = LS->over_load==0;
        if (improved<0) {
          I->get_fitness();
          // newC = I->costs;
          // if (oldC!=newC-improved) {
          //   cout<<oldC<<" => "<<newC<<" "<<improved<<endl;
          //   cout<<"error val"<<endl;
          //   exit(0);
          // }
          temp_over_load += over_load_changed;
          total_improved += improved;
          // record_i = 
          stop = 0;
        }
        stop += LS->fitness>=0;
        // cout<<improved<<endl;
        // cout<<LS->fitness;
        // cout<<"stop"<<stop<<endl;
        if (all_non_feasible==5) {
          LS->lamda *= 2;
        } else if (all_feasible==5) {
          LS->lamda /= 2;
        } else {
          all_non_feasible = is_overload?0:all_non_feasible+1;
          all_feasible     = is_overload?all_feasible+1:0;
        }
        if(stop==2) {
          break;
        }
      }
      improved = total_improved;
      over_load_changed= temp_over_load;
      I->merge_subroute();
      I->get_fitness();
      return improved<0;
    }
    void show_move() {
      //cout<<*LS->wanted_move;
    }
    LocalSearch(CARP_INDIVIDUAL* indi):I(indi) {
      I->split_to_subroute();
      I->get_costs();
      I->get_over_load();
      LS = new L(indi);
      //old_costs = I->costs;
    //  over_load_changed = I->get_over_load();
    };
    LocalSearch() {
  
    };
    ~LocalSearch() {
  
    };
/*
    void find_wanted_move_of_two_routes(CARP_INDIVIDUAL* R
      ,CARP_INDIVIDUAL* S) {}
    void find_wanted_move_of_all_routes() {}*/
};

/*
template<class M>
std::ostream &  operator<<(std::ostream & out,LocalSearch<M> *LS) {
  out<<"";
  //out<<"old,new,improved : "<<LS->old_costs<<" "<<LS->new_costs<<" "<<LS->improved<<endl;
  return out;
}

template<class M>
std::ostream &  operator<<(std::ostream & out,LocalSearch<M> &LS) {
  // out<<"******* move ********"<<endl;
  // show(m.R1->route);
  // show(m.R2->route);
  //out<<"old,improved,new: "<<LS.old_costs<<" "<<LS.LS->wanted_move->costs<<" "<<LS.old_costs+LS.improved<<"\t over load "<<LS.LS->wanted_move->over_load<<endl;
  //out<<"position from "<<LS.LS->wanted_move->position1<<" to "<<LS.LS->wanted_move->position2<<endl;
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
