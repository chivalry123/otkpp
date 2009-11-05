
#include "func_wrapper.h"
#include "NEWUOA.h"

extern "C"
{
  void newuoa_(int *n, int *npt, double *x, double *rhobeg, double *rhoend, 
               int *iprint, int *maxfun, double *w);
}

std::string NEWUOA::getName() const
{
  return "NEWUOA";
}


bool NEWUOA::isExternalSolver() const
{
  return true;
}

SolverResults NEWUOA::solve(const Function &objFunc,
                            const vector< double > &x0,
                            const SolverSetup &solverSetup,
                            const Constraints &C,
                            const StoppingCriterion *stopCrit,
                            bool timeTest)
{
  int n = objFunc.getN();
  SolverResults results;
  
  set_newuoa_obj_func(&objFunc);
  
  int npt = 2*n+1;
  
  double *x = new double[n];
  for(int i = 0; i < n; i++)
    x[i] = x0[i];
  
  double rhobeg = 10.0;
  double rhoend = 1e-6;
  
  int iprint = 3;
  int maxfun = 1000;
  
  double *w = new double[(npt+13)*(npt+n)+3*n*(n+3)/2];
  
  newuoa_(&n, &npt, x, &rhobeg, &rhoend, &iprint, &maxfun, w);
  
  delete[] w;
  delete[] x;
  
  return results;
}

bool NEWUOA::usesGradient() const
{
  return false;
}

bool NEWUOA::usesHessian() const
{
  return false;
}
