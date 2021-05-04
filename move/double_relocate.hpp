/*************************************************************************
 *  Name  : move.hpp
 *  Author: zmdsn
 *  Mail  : zmdsn@126.com 
 * Created Time: 2020年11月18日 星期三 13时47分27秒
 * 
 ************************************************************************/
#ifndef  __DOUBLE_RELOCATE__HPP__
#define  __DOUBLE_RELOCATE__HPP__
#include<iostream>
#include "../move/k_relocate.hpp"
// #include "move.hpp"
using namespace std;
class DOUBLE_RELOCATE:public K_RELOCATE{
  // (x,y) <==>  (u,v) 
  public:
    int len1 = 1;
    DOUBLE_RELOCATE(){
      length1 = len1;
    }
    DOUBLE_RELOCATE(CARP_INDIVIDUAL *_R1,CARP_INDIVIDUAL *_R2,
        int _position1, int _position2):
      K_RELOCATE(_R1,_R2,_position1,_position2,len1){
        length1 = len1;
	}
    DOUBLE_RELOCATE(CARP_INDIVIDUAL *_R1,CARP_INDIVIDUAL *_R2):
      K_RELOCATE(_R1,_R2,0,0,len1){
        length1 = len1;
	}
};
#endif
