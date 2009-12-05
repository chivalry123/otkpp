
#include "ConjGradMT.h"

ConjGradMT::ConjGradMT(Type type)
{
  type_ = type;
}

std::string ConjGradMT::getName() const
{
  if(type_ == FLETCHER_REEVES)
    return "ConjGrad-FR/MT";
  else
    return "ConjGrad-PR/MT";
}

bool ConjGradMT::isGSLSolver() const
{
  return false;
}

bool ConjGradMT::usesGradient() const
{
  return true;
}

bool ConjGradMT::usesHessian() const
{
  return false;
}

NativeSolver::IterationStatus ConjGradMT::iterate_()
{
  double alpha, alpha0;
  double gamma;
  double gd;
  double gg;
  
  gg = inner_prod(state_.g, state_.g);
  gd = inner_prod(state_.g, d_);
  if(gd >= 0.0)
  {
    d_ = -state_.g;
    gd = -gg;
  }
  
  alpha0 = LineMinimizer::fletcherInitStep(gd, deltaF_);
  lineMinimizer_.minimize(state_.x, d_, alpha0, state_.f, state_.g,
                          alpha, xPlus_, fPlus_, gPlus_);
  
  if(type_ == FLETCHER_REEVES)
  {
    i_++;
    if(i_ == n_ + 1)
    {
      gamma = 0.0;
      i_ = 0;
    }
    else
      gamma = inner_prod(gPlus_, gPlus_ ) / gg;
  }
  else
  {
    gamma = std::max(inner_prod(gPlus_, gPlus_ - state_.g) / gg, 0.0);
    lineMinimizer_.setGamma(gamma);
  }
  d_ = -gPlus_ + gamma*d_;
  
  deltaF_ = state_.f - fPlus_;
  
  state_.x = xPlus_;
  state_.f = fPlus_;
  state_.g = gPlus_;
  
  return NativeSolver::ITERATION_CONTINUE;
}

void ConjGradMT::setup_(const Function &objFunc,
                        const vector< double > &x0,
                        const Solver::Setup &solverSetup,
                        const Constraints &C)
{
  GradientSolver::setup_(objFunc, x0, solverSetup, C);
  lineMinimizer_.setup(objFunc_, MoreThuente::Setup());
  d_ = -state_.g;
  deltaF_ = 0.0;
  i_ = 0;
}
