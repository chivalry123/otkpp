
#include "GradientSolver.h"

GradientSolver::GradientSolver(bool useFDiffGradient) : 
  AbstractGradientSolver(Function::DERIV_SYMBOLIC) { }

const vector< double > GradientSolver::getGradient() const
{
  return g_;
}

bool GradientSolver::hasBuiltInStoppingCriterion() const
{
  return false;
}

void GradientSolver::setup_(const Function &objFunc,
                            const vector< double > &x0,
                            const SolverSetup &solverSetup,
                            const Constraints &C)
{
  AbstractGradientSolver::setup_(objFunc, x0, solverSetup, C);
  g_.resize(objFunc.getN());
  objFunc.g(x0, g_);
}
