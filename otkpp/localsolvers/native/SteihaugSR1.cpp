
#include "SteihaugSR1.h"

SteihaugSR1::SteihaugSR1() { }

SteihaugSR1::~SteihaugSR1() { }

std::string SteihaugSR1::getName() const
{
  return "SR1/STE";
}

bool SteihaugSR1::isGSLSolver() const
{
  return false;
}

bool SteihaugSR1::usesGradient() const
{
  return true;
}

bool SteihaugSR1::usesHessian() const
{
  return true;
}

NativeSolver::IterationStatus SteihaugSR1::iterate_()
{
  bool nonzeroStep;
  
  trSolver_.computeStep(x_, f_, g_, H_, p_, nonzeroStep, xPlus_, fPlus_);
  
  /*if(!nonzeroStep)
  {
    H_ = identity_matrix< double >(n_);
    return Solver::ITERATION_SUCCESS;
  }*/
  
  objFunc_.g(xPlus_, gPlus_);
  
  q_ = gPlus_ - g_;
  matrixUpdater_.update(p_, q_, H_);
  
  x_ = xPlus_;
  f_ = fPlus_;
  g_ = gPlus_;
  
  return NativeSolver::ITERATION_CONTINUE;
}

void SteihaugSR1::setup_(const Function &objFunc,
                      const vector< double > &x0,
                      const Solver::Setup &solverSetup,
                      const Constraints &C)
{
  const int n = objFunc.getN();
  
  GradientSolver::setup_(objFunc, x0, solverSetup, C);
  trSolver_.setup(objFunc_);
  
  g_.resize(n);
  gPlus_.resize(n);
  p_.resize(n);
  H_ = identity_matrix< double >(n, n);
}
