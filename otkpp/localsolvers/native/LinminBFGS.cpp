
#include "BFGSUpdater.h"
#include "Fletcher.h"
#include "LinminBFGS.h"
#include "MoreThuente.h"

#include <typeinfo>

LinminBFGSSetup::LinminBFGSSetup(const LinminSetup &lmSetup,
                                 const matrix< double > &H0)
{
  this->lmSetup = lmSetup.clone();
  if(H0.size1() > 0)
    this->H0 = new matrix< double >(H0);
  else
    this->H0 = NULL;
}

LinminBFGSSetup::~LinminBFGSSetup()
{
  delete lmSetup;
  delete H0;
}

bool LinminBFGSSetup::isCompatibleWith(const Solver &s) const
{
  return (typeid(s) == typeid(const LinminBFGS &));
}

LinminBFGS::LinminBFGS(LinMinType lmType, int iterHistLen) : 
  lmType_(lmType), iterHistLen_(iterHistLen)
{
  if(lmType == LinminBFGS::FLETCHER)
    lineMinimizer_ = new Fletcher();
  else
    lineMinimizer_ = new MoreThuente();
  matrixUpdater_ = new BFGSUpdater(BFGSUpdater::INVERSE);
}

LinminBFGS::~LinminBFGS()
{
  delete lineMinimizer_;
  delete matrixUpdater_;
}

std::string LinminBFGS::getName() const
{
  if(lmType_ == LinminBFGS::FLETCHER)
  {
    if(iterHistLen_ == 0)
      return "BFGS/F";
    else
      return "L-BFGS/F";
  }
  else if(lmType_ == LinminBFGS::MORE_THUENTE)
  {
    if(iterHistLen_ == 0)
      return "BFGS/MT";
    else
      return "L-BFGS/MT";
  }
}

bool LinminBFGS::isGSLSolver() const
{
  return false;
}

bool LinminBFGS::usesGradient() const
{
  return true;
}

bool LinminBFGS::usesHessian() const
{
  return false;
}

NativeSolver::IterationStatus LinminBFGS::iterate_()
{
  double alpha;
  double D;
  
  if(iterHistLen_ > 0)
  {
    if(nIter_ >= 1)
      dirUpdater_.update(g_, d_);
    else
      d_ = -g_;
  }
  else
    d_ = -prod(S_, g_);
  
  lineMinimizer_->minimize(x_, d_, 1.0, f_, g_,
                           alpha, xPlus_, fPlus_, gPlus_);
  
  p_ = xPlus_ - x_;
  q_ = gPlus_ - g_;
  
  D = inner_prod(p_, q_);
  if(iterHistLen_ > 0)
  {
    if(D <= 0.0)
      d_ = -g_;
    
    dirUpdater_.storePair(p_, q_);
    dirUpdater_.removeOldestPair();
  }
  else
  {
    if(D <= 0.0)
      S_ = identity_matrix< double >(n_);
    else
      matrixUpdater_->update(p_, q_, S_);
  }
  
  x_ = xPlus_;
  f_ = fPlus_;
  g_ = gPlus_;
  
  return NativeSolver::ITERATION_CONTINUE;
}

void LinminBFGS::setup_(const Function &objFunc,
                        const vector< double > &x0,
                        const SolverSetup &solverSetup,
                        const Constraints &C)
{
  const int n = objFunc.getN();
  
  GradientSolver::setup_(objFunc, x0, solverSetup, C);
  
  if(typeid(solverSetup) == typeid(const DefaultSolverSetup &))
  {
    S_ = identity_matrix< double >(n);
    
    if(lmType_ == LinminBFGS::FLETCHER)
      lineMinimizer_->setup(objFunc_, FletcherSetup());
    else if(lmType_ == LinminBFGS::MORE_THUENTE)
      lineMinimizer_->setup(objFunc_, MoreThuenteSetup());
  }
  else
  {
    const LinminBFGSSetup &setup = 
      dynamic_cast< const LinminBFGSSetup & >(solverSetup);
    if(setup.H0 != NULL)
    {
      if(setup.H0->size1() == n && setup.H0->size2() == n)
        S_ = *setup.H0;
      else
        throw std::invalid_argument("dimension mismatch");
    }
    else
      S_ = identity_matrix< double >(n);
    
    if(lmType_ == LinminBFGS::FLETCHER)
      lineMinimizer_->setup(objFunc_, *setup.lmSetup);
    else if(lmType_ == LinminBFGS::MORE_THUENTE)
      lineMinimizer_->setup(objFunc_, *setup.lmSetup);
  }
  
  if(iterHistLen_ > 0)
    dirUpdater_ = InvLBFGSUpdater(iterHistLen_, objFunc.getN());
}
