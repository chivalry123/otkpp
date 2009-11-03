
#include "LMBM.h"

#include "func_wrapper.h"
#include "lmbm_utils.h"

LMBM_setup::LMBM_setup()
{
  mc = 7;
  
  mit = 5000000;
  mfe = 5000000;
  mos = 0;
  mtesf = 0;
  iprint = 0;
  method = 0;
  iscale = 0;

  tolf = 0.0;
  tolf2 = 0.0;
  tolb = 1e-5;
  tolg = 0.0;
  tolg2 = 0.5;
  eta = 0.0;
  epsl = 0.0;
  xmax = 0.0;
}

LMBM::LMBM() { }

LMBM::~LMBM() { }

SolverResults LMBM::solve(const Function &objFunc,
                          const vector< double > &x0,
                          const SolverSetup &solverSetup,
                          const Constraints &C,
                          const StoppingCriterion *stopCrit,
                          bool timeTest)
{
  std::list< vector< double > > iterates;
  SolverResults results;
  
  //Solver::setup(objFunc, x0, solverSetup);
  //const LMBM_setup &setup = dynamic_cast< const LMBM_setup & >(solverSetup);
  
  int n = objFunc.getN();
  int na = 2;
  int mcu = 7;
  int nw = 1 + 9*n + 2*n*na + 3*na + 2*n*(mcu+1) + 
           3*(mcu+2)*(mcu+1)/2 + 9*(mcu+1);
  int mc = 7;
  
  int ipar[7];
  double rpar[8];
  
  for (int i = 1; i <= 7; ++i)
    ipar[i - 1] = 0;

  ipar[4] = -1;
  ipar[5] = 0;
  ipar[6] = 0;
  ipar[1] = 5000000;
  ipar[2] = 5000000;
  
  for (int i = 1; i <= 8; ++i)
    rpar[i - 1] = 0.;
  rpar[3] = 1e-5;
  rpar[5] = .5;
  
  double *x = new double[x0.size()];
  for(int i = 0; i < x0.size(); i++)
    x[i] = x0[i];
  double *xr = new double[1000*x0.size()];
  double f = objFunc(x0);
  
  float rtim[2];
  float time = 0.0;
  int iout[3];
  double *w = new double[nw];
  
  set_obj_func(&objFunc);
  
  lmbmu_(&n, &na, &mcu, &mc, &nw, x, xr, &f, rpar, ipar,
         iout, &time, rtim, w);
  
  vector< double > xi(n);
  vector< double > xMin(n);
  for(int i = 0; i < iout[0]; i++)
  {
    for(int j = 0; j < n; j++)
      xi[j] = xr[j * 1000 + i];
    iterates.push_back(xi);
    if(i == iout[0] - 1)
      xMin = xi;
  }
  
  results.fMin        = f;
  results.iterates    = iterates;
  results.numIter     = iout[0];
  results.numFuncEval = iout[1];
  results.numGradEval = 0;
  results.time        = rtim[0];
  results.xMin        = xMin;
  
  delete[] w;
  delete[] xr;
  delete[] x;
  
  return results;
}
