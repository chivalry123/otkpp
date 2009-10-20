
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
  
  gg = inner_prod(g_, g_);
  gd = inner_prod(g_, d_);
  if(gd >= 0.0)
  {
    d_ = -g_;
    gd = -gg;
  }
  
  alpha0 = LineMinimizer::fletcherInitStep(gd, deltaF_);
  lineMinimizer_.minimize(x_, d_, alpha0, f_, g_,
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
    gamma = std::max(inner_prod(gPlus_, gPlus_ - g_) / gg, 0.0);
    lineMinimizer_.setGamma(gamma);
  }
  d_ = -gPlus_ + gamma*d_;
  
  deltaF_ = f_ - fPlus_;
  
  x_ = xPlus_;
  f_ = fPlus_;
  g_ = gPlus_;
  
  return NativeSolver::ITERATION_CONTINUE;
}

void ConjGradMT::setup_(const Function &objFunc,
                        const vector< double > &x0,
                        const SolverSetup &solverSetup,
                        const Constraints &C)
{
  GradientSolver::setup_(objFunc, x0, solverSetup, C);
  lineMinimizer_.setup(objFunc_, MoreThuenteSetup());
  d_ = -g_;
  deltaF_ = 0.0;
  i_ = 0;
}
