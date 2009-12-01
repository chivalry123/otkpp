
#include "GradientSolver.h"

#ifdef WITH_LIBMATHEVAL
GradientSolver::GradientSolver(bool useFDiffGradient) : 
  AbstractGradientSolver(useFDiffGradient == false ? Function::DERIV_SYMBOLIC : Function::DERIV_FDIFF_CENTRAL_2) { }
#else
GradientSolver::GradientSolver(bool useFDiffGradient) : 
  AbstractGradientSolver(Function::DERIV_FDIFF_CENTRAL_2) { }
#endif

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
                            const Solver::Setup &solverSetup,
                            const Constraints &C)
{
  AbstractGradientSolver::setup_(objFunc, x0, solverSetup, C);
  g_.resize(objFunc.getN());
  objFunc.g(x0, g_);
}
