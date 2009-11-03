
#ifndef GLOBALSOLVER_H

#include <otkpp/constraints/BoundConstraints.h>
#include <otkpp/function/Function.h>

class Function;

class GlobalSolver
{
 public:
  void setup(const Function &objFunc,
             const BoundConstraints &bounds);
  
  void iterate();
 protected:
  int n_;
  Function objFunc_;
  
  virtual void iterate_() = 0;
};

#define GLOBALSOLVER_H

#endif
