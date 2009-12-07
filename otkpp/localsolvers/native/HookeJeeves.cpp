
#include "HookeJeeves.h"

std::string HookeJeeves::getName() const
{
  return "HookeJeeves";
}

bool HookeJeeves::hasBuiltInStoppingCriterion() const
{
  return true;
}

bool HookeJeeves::usesGradient() const
{
  return false;
}

bool HookeJeeves::usesHessian() const
{
  return false;
}

NativeSolver::IterationStatus HookeJeeves::iterate_()
{
  vector< double > d = zero_vector< double >(n_);
  double ft, fy;
  vector< double > yt;
  
  for(int j = 0; j < n_; j++)
  {
    d[j] = 1.0;
    
    fy = objFunc_(y_);
    yt = y_ + delta_*d;
    ft = objFunc_(yt);
    
    if(!isnan(ft) && !isinf(ft) && ft < fy)
      y_ = yt;
    else
    {
      yt = y_ - delta_*d;
      ft = objFunc_(yt);
      if(ft < fy)
        y_ = yt;
    }
    
    d[j] = 0.0;
  }
  
  if(!isnan(fy) && !isinf(fy) && fy < state_.f)
  {
    xPlus_ = y_;
    y_ = xPlus_ + alpha_ * (xPlus_ - state_.x);
    state_.x = xPlus_;
    state_.f = objFunc_(state_.x);
  }
  else
  {
    if(delta_ < eps_)
      return NativeSolver::ITERATION_SUCCESS;
    else
    {
      delta_ *= rho_;
      y_ = state_.x;
    }
  }
  
  return NativeSolver::ITERATION_CONTINUE;
}

void HookeJeeves::setup_(const Function &objFunc,
                         const vector< double > &x0,
                         const Solver::Setup &solverSetup,
                         const Constraints &C)
{
  NativeSolver::setup_(objFunc, x0, solverSetup, C);
  state_.x = x0;
  state_.f = objFunc(x0);

  delta_ = 1.0;
  eps_ = 1e-10;
  alpha_ = 1.0;
  rho_ = 0.75;
  y_ = x0;
}
