#ifndef PROBLEM_H_
#define  PROBLEM_H_

#include<iostream>
#include <vector>

// objective 
template<typename Variable_Type ,typename Obj_Type>
class Problem{
	public:
		int result_dimension;
		std::vector<Obj_Type> objective_value;
		// objective function
		Obj_Type objective_function(Variable_Type& x);
		Obj_Type Constraint_violation_value;
		Obj_Type Constraint_violation(Variable_Type& x);
		Problem();
		Problem(int dimension);
		~Problem();
};

template<typename Variable_Type ,typename Obj_Type>
Problem<Variable_Type,Obj_Type>::Problem() : result_dimension(1) {
	objective_value.resize(result_dimension);
}

template<typename Variable_Type ,typename Obj_Type>
Problem<Variable_Type,Obj_Type>::Problem(int dimension) 
	: result_dimension(dimension) {
	objective_value.resize(result_dimension);
}

template<typename Variable_Type ,typename Obj_Type>
Problem<Variable_Type,Obj_Type>::~Problem() {

}

template<typename Variable_Type ,typename Obj_Type>
Obj_Type Problem<Variable_Type,Obj_Type>::objective_function(Variable_Type& x){
	Obj_Type result = 0;
	for(auto v:x) {
		result += v*v;
	}
	objective_value[0] = result;
	objective_value[1] = result+1;
	return result;
}

template<typename Variable_Type ,typename Obj_Type>
Obj_Type Problem<Variable_Type,Obj_Type>::Constraint_violation(Variable_Type& x){
	Constraint_violation_value = 0;
	return Constraint_violation_value;
}

#endif
