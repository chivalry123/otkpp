
#include "LBFGSB.h"
#include "lbfgsb_utils.h"

#include <cstring>
#include <typeinfo>

LBFGSB::Setup::Setup(int m) : m(m) { }

bool LBFGSB::Setup::isCompatibleWith(const Solver &s) const
{
  return (typeid(s) == typeid(const LBFGSB &));
}

LBFGSB::LBFGSB(Function::DerivEvalType gEvalType) : 
  GradientSolver(gEvalType) { }

const vector< double > LBFGSB::getGradient() const
{
  return state_.g;
}

std::string LBFGSB::getName() const
{
  return "L-BFGS-B";
}

bool LBFGSB::hasBuiltInStoppingCriterion() const
{
  return false;
}

bool LBFGSB::isGSLSolver() const
{
  return false;
}

bool LBFGSB::supportsConstraints(const Constraints &C)
{
  return (typeid(C) == typeid(BoundConstraints));
}

bool LBFGSB::usesGradient() const
{
  return true;
}

bool LBFGSB::usesHessian() const
{
  return false;
}

NativeSolver::IterationStatus LBFGSB::iterate_()
{
  int iprint = -1;
  
  setulb_(&n_, &m_, &state_.x[0], &constraints_.L[0], &constraints_.U[0],
          &nbd_[0], &state_.f, &state_.g[0], &factr_, &pgtol_, &wa_[0],
          &iwa_[0], task_, &iprint, csave_, lsave_, isave_,
          dsave_, 60, 60);
  
  if(task_[0] == 'F' && task_[1] == 'G')
  {
    state_.f = objFunc_(state_.x);
    objFunc_.g(state_.x, state_.g);
  }
  
  return NativeSolver::ITERATION_CONTINUE;
}

void LBFGSB::setup_(const Function &objFunc,
                    const vector< double > &x0,
                    const Solver::Setup &solverSetup,
                    const Constraints &C)
{
  GradientSolverBase::setup_(objFunc, x0, solverSetup, C);
  
  if(typeid(solverSetup) == typeid(const Solver::DefaultSetup &))
  {
    m_ = 10;
  }
  else
  {
    const LBFGSB::Setup &setup = 
      dynamic_cast< const LBFGSB::Setup & >(solverSetup);
    m_ = setup.m;
  }
  
  state_.g.resize(n_);
  
  nbd_.resize(n_);
  if(typeid(C) == typeid(const NoConstraints &))
  {
    for(int i = 0; i < n_; i++)
      nbd_[i] = 0;
  }
  else if(typeid(C) == typeid(const BoundConstraints &))
  {
    const BoundConstraints &BC = 
      dynamic_cast< const BoundConstraints & >(C);
    constraints_ = BC;
    for(int i = 0; i < n_; i++)
    {
      if(BC.types[i] == BoundConstraints::NONE)
        nbd_[i] = 0;
      else if(BC.types[i] == BoundConstraints::LOWER)
        nbd_[i] = 1;
      else if(BC.types[i] == BoundConstraints::BOTH)
        nbd_[i] = 2;
      else if(BC.types[i] == BoundConstraints::UPPER)
        nbd_[i] = 3;
    }
  }
  else
    throw std::invalid_argument("Unsupported constraints type.");
  
  factr_ = 0.0; //1e6;
  pgtol_ = 0.0; //1e-5;
  wa_.resize((2*m_ + 4)*n_ + 12*m_*m_ + 12*m_);
  iwa_.resize(3*n_);
  
  csave_[0] = '\0';
  strcpy(task_, "START");
  for(int i = 5; i < 60; i++)
    task_[i] = ' ';
  
  objFunc_.g(state_.x, state_.g);
}
