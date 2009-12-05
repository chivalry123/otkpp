
#include "BFGSUpdater.h"
#include "Fletcher.h"
#include "LinminBFGS.h"
#include "MoreThuente.h"

#include <typeinfo>

LinminBFGS::Setup::Setup(const LineMinimizer::Setup &lmSetup,
                         const matrix< double > &H0)
{
  this->lmSetup = lmSetup.clone();
  if(H0.size1() > 0)
    this->H0 = new matrix< double >(H0);
  else
    this->H0 = NULL;
}

LinminBFGS::Setup::~Setup()
{
  delete lmSetup;
  delete H0;
}

bool LinminBFGS::Setup::isCompatibleWith(const Solver &s) const
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
      dirUpdater_.update(state_.g, d_);
    else
      d_ = -state_.g;
  }
  else
    d_ = -prod(S_, state_.g);
  
  lineMinimizer_->minimize(state_.x, d_, 1.0, state_.f, state_.g,
                           alpha, xPlus_, fPlus_, gPlus_);
  
  p_ = xPlus_ - state_.x;
  q_ = gPlus_ - state_.g;
  
  D = inner_prod(p_, q_);
  if(iterHistLen_ > 0)
  {
    if(D <= 0.0)
      d_ = -state_.g;
    
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
  
  state_.x = xPlus_;
  state_.f = fPlus_;
  state_.g = gPlus_;
  
  return NativeSolver::ITERATION_CONTINUE;
}

void LinminBFGS::setup_(const Function &objFunc,
                        const vector< double > &x0,
                        const Solver::Setup &solverSetup,
                        const Constraints &C)
{
  const int n = objFunc.getN();
  
  GradientSolver::setup_(objFunc, x0, solverSetup, C);
  
  if(typeid(solverSetup) == typeid(const Solver::DefaultSetup &))
  {
    S_ = identity_matrix< double >(n);
    
    if(lmType_ == LinminBFGS::FLETCHER)
      lineMinimizer_->setup(objFunc_, Fletcher::Setup());
    else if(lmType_ == LinminBFGS::MORE_THUENTE)
      lineMinimizer_->setup(objFunc_, MoreThuente::Setup());
  }
  else
  {
    const LinminBFGS::Setup &setup = 
      dynamic_cast< const LinminBFGS::Setup & >(solverSetup);
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
