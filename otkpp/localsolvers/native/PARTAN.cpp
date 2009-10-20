
#include "MoreThuente.h"
#include "PARTAN.h"

PARTAN::PARTAN()
{
  lineMinimizer_ = new MoreThuente();
}

PARTAN::~PARTAN()
{
  delete lineMinimizer_; 
}

std::string PARTAN::getName() const
{
  return "PARTAN";
}

bool PARTAN::isGSLSolver() const
{
  return false;
}

bool PARTAN::usesGradient() const
{
  return true;
}

bool PARTAN::usesHessian() const
{
  return false;
}

NativeSolver::IterationStatus PARTAN::iterate_()
{
  double alpha;
  
  if(state_ == 1 || state_ == 2)
  {
    d_ = -g_;
    lineMinimizer_->minimize(x_, d_, 1.0, f_, g_,
                             alpha, xPlus_, fPlus_, gPlus_);
    
    if(state_ == 1)
    {
      xMinus_ = x_;
      fMinus_ = f_;
      gMinus_ = g_;
    }
  }
  else
  {
    d_ = x_ - xMinus_;
    lineMinimizer_->minimize(xMinus_, d_, 1.0, fMinus_, gMinus_,
                             alpha, xPlus_, fPlus_, gPlus_);
    j_++;
    if(j_ == n_)
    {
      j_ = 1;
      state_ = 0;
    }
    else
    {
      xMinus_ = xPlus_;
      fMinus_ = fPlus_;
      gMinus_ = gPlus_;
    }
  }
  
  state_++;
  if(state_ == 4)
    state_ = 1;

  x_ = xPlus_;
  f_ = fPlus_;
  g_ = gPlus_;
  
  return NativeSolver::ITERATION_CONTINUE;
}

void PARTAN::setup_(const Function &objFunc,
                    const vector< double > &x0,
                    const SolverSetup &solverSetup,
                    const Constraints &C)
{
  GradientSolver::setup_(objFunc, x0, solverSetup, C);
  lineMinimizer_->setup(objFunc_, MoreThuenteSetup());
  
  j_ = 1;
  d_ = -g_;
  state_ = 1;
}
