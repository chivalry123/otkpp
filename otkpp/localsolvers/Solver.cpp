
#include "Solver.h"

#include <typeinfo>

void Solver::setup(const Function &objFunc,
                   const vector< double > &x0,
                   const Solver::Setup &solverSetup,
                   const Constraints &C)
{
  n_ = objFunc.getN();
  
  if(typeid(solverSetup) != typeid(const Solver::DefaultSetup &) && 
     !solverSetup.isCompatibleWith(*this))
    throw std::invalid_argument("the solver and its setup are incompatible");
  
  if(n_ != x0.size())
    throw std::runtime_error("dimension mismatch: f!=x0");
  
  objFunc_ = objFunc;
  objFunc_.resetEvalCounters();
  objFunc_.enableEvalCounting();
  
  setup_(objFunc, x0, solverSetup, C);
}
